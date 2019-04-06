struct FamicomDisk {
  static auto transform(array_view<uint8_t> data) -> vector<uint8_t>;
};

auto Icarus::famicomDiskManifest(string location) -> string {
  string output;
  output.append("game\n");
  return output;
}

auto Icarus::famicomDiskManifest(vector<uint8_t>& buffer, string location) -> string {
  if(settings.useDatabase) {
    auto digest = Hash::SHA256(buffer).digest();
    for(auto game : Database::FamicomDisk.find("game")) {
      if(game["sha256"].text() == digest) return BML::serialize(game);
    }
  }

  if(settings.useHeuristics) {
    Heuristics::FamicomDisk game{buffer, location};
    if(auto manifest = game.manifest()) return manifest;
  }

  return {};
}

auto Icarus::famicomDiskImport(vector<uint8_t>& buffer, string location) -> string {
  auto name = Location::prefix(location);
  auto source = Location::path(location);
  string target{settings.famicomDisk, name, "/"};

  auto manifest = famicomDiskManifest(buffer, location);
  if(!manifest) return failure("failed to parse ROM image");

  if(!create(target)) return failure("library path unwritable");

  array_view<uint8_t> input = buffer;
  uint disk = 0, side = 0;
  while(auto output = FamicomDisk::transform(input)) {
    string name{"disk", 1 + disk, ".", "side", !side ? "A" : "B"};
    write({target, name}, output);

    side ^= 1;
    if(!side) disk++;
    input += 65500;
  }

  return success(target);
}

auto FamicomDisk::transform(array_view<uint8_t> input) -> vector<uint8_t> {
  if(input.size() < 65500) return {};

  array_view<uint8_t> data{input.data(), 65500};
  if(data[0x00] != 0x01) return {};
  if(data[0x38] != 0x02) return {};
  if(data[0x3a] != 0x03) return {};
  if(data[0x4a] != 0x04) return {};

  vector<uint8_t> output;
  uint16_t crc16 = 0;
  auto hash = [&](uint8_t byte) {
    for(uint bit : range(8)) {
      bool carry = crc16 & 1;
      crc16 = crc16 >> 1 | bool(byte & 1 << bit) << 15;
      if(carry) crc16 ^= 0x8408;
    }
  };
  auto write = [&](uint8_t byte) {
    hash(byte);
    output.append(byte);
  };
  auto flush = [&] {
    hash(0x00);
    hash(0x00);
    output.append(crc16 >> 0);
    output.append(crc16 >> 8);
    crc16 = 0;
  };

  //block 1
  for(uint n : range(0xe00)) write(0x00);  //pregap
  write(0x80);
  for(uint n : range(0x38)) write(*data++);
  flush();

  //block 2
  for(uint n : range(0x80)) write(0x00);  //gap
  write(0x80);
  for(uint n : range(0x02)) write(*data++);
  flush();

  while(true) {
    if(data[0x00] != 0x03 || data.size() < 0x11) break;
    uint16_t size = data[0x0d] << 0 | data[0x0e] << 8;
    if(data[0x10] != 0x04 || data.size() < 0x11 + size) break;

    //block 3
    for(uint n : range(0x80)) write(0x00);  //gap
    write(0x80);
    for(uint n : range(0x10)) write(*data++);
    flush();

    //block 4
    for(uint n : range(0x80)) write(0x00);  //gap
    write(0x80);
    for(uint n : range(1 + size)) write(*data++);
    flush();
  }

  //note: actual maximum capacity of a Famicom Disk is currently unknown
  while(output.size() < 0x12000) output.append(0x00);  //expand if too small
  output.resize(0x12000);  //shrink if too large
  return output;
}
