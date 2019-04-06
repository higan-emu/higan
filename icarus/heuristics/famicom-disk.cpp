namespace Heuristics {

struct FamicomDisk {
  FamicomDisk(vector<uint8_t>& data, string location);
  explicit operator bool() const;
  auto manifest() const -> string;

private:
  auto size() const -> uint { return data.size(); }
  auto sides() const -> uint;
  auto disks() const -> uint;

  vector<uint8_t>& data;
  string location;
  boolean vandalized;
};

FamicomDisk::FamicomDisk(vector<uint8_t>& data, string location) : data(data), location(location) {
  if(size() % 65500 == 16) {
    //iNES and fwNES headers are unnecessary
    memory::move(&data[0], &data[16], size() - 16);
    data.resize(size() - 16);
  }

  //attempt to reverse "hCON by hal9999" header vandalization
  for(uint index : range(size() / 65500)) {
    auto side = &data[index * 65500];
    if(memory::compare(&side[0x20], "hCON by hal9999", 15)) vandalized = true;
  }
  if(!vandalized) return;

  //attempt to rebuild block 1 header for all disk sides
  for(uint index : range(size() / 65500)) {
    auto side = &data[index * 65500];

    //zero out the vandalism
    for(uint index = 0x20; index <= 0x37; index++) side[index] = 0x00;

    //repair damage as best we can: the values won't be correct, but at least they won't be invalid
    side[0x1f] = 0x61;  //1986 (Shouwa 61)
    side[0x20] = 0x01;  //January
    side[0x21] = 0x01;  //1st
    side[0x22] = 0x49;  //country code: Japan
    side[0x23] = 0x61;  //unknown (constant?)
    side[0x26] = 0x02;  //unknown (constant?)
    side[0x2c] = side[0x1f];  //rewritten date
    side[0x2d] = side[0x20];  //same as manufactured date if never rewritten
    side[0x2e] = side[0x21];
    side[0x2f] = 0xff;
    side[0x30] = 0xff;
    side[0x31] = 0xff;
    side[0x32] = 0xff;
    side[0x33] = 0xff;
    side[0x35] = index & 1;   //disk side (0x00 = side A, 0x01 = side B)
  }

  print("[icarus] vandalization repair attempted:\n");
  print(location, "\n\n");
}

FamicomDisk::operator bool() const {
  return size() % 65500 == 0;
}

auto FamicomDisk::sides() const -> uint {
  if(!operator bool()) return 0;
  return size() / 65500;
}

auto FamicomDisk::disks() const -> uint {
  if(!operator bool()) return 0;
  return size() / 65500 * 0.5;
}

auto FamicomDisk::manifest() const -> string {
  if(!operator bool()) return {};

  string output;
  output.append("game\n");
  if(vandalized) output.append("  note: vandalization repair attempted by icarus\n");
  return output;
}

}
