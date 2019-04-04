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
};

FamicomDisk::FamicomDisk(vector<uint8_t>& data, string location) : data(data), location(location) {
  if(size() % 65500 == 16) {
    //iNES and fwNES headers are unnecessary
    memory::move(&data[0], &data[16], size() - 16);
    data.resize(size() - 16);
  }
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
  output.append("  sha256: ", Hash::SHA256(data).digest(), "\n");
  return output;
}

}
