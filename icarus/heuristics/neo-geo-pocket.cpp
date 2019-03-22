namespace Heuristics {

struct NeoGeoPocket {
  NeoGeoPocket(vector<uint8_t>& data, string location);
  explicit operator bool() const;
  auto manifest() const -> string;
  auto title() const -> string;

private:
  vector<uint8_t>& data;
  string location;
};

NeoGeoPocket::NeoGeoPocket(vector<uint8_t>& data, string location) : data(data), location(location) {
}

NeoGeoPocket::operator bool() const {
  //expand ROMs that are smaller than valid flash chip sizes (homebrew games)
  if(data.size() <= 0x080000) return data.resize(0x080000), true;  // 4mbit
  if(data.size() <= 0x100000) return data.resize(0x100000), true;  // 8mbit
  if(data.size() <= 0x200000) return data.resize(0x200000), true;  //16mbit
  if(data.size() <= 0x280000) return data.resize(0x280000), true;  //16mbit +  4mbit
  if(data.size() <= 0x300000) return data.resize(0x300000), true;  //16mbit +  8mbit
  if(data.size() <= 0x400000) return data.resize(0x400000), true;  //16mbit + 16mbit
  return false;
}

auto NeoGeoPocket::manifest() const -> string {
  if(!operator bool()) return {};

  string output;
  output.append("game\n");
  output.append("  sha256: ", Hash::SHA256(data).digest(), "\n");
  output.append("  label:  ", Location::prefix(location), "\n");
  output.append("  name:   ", Location::prefix(location), "\n");
  output.append("  title:  ", title(), "\n");
  output.append("  board\n");
  output.append(Memory{}.type("ROM").size(data.size()).content("Program").text());
  return output;
}

auto NeoGeoPocket::title() const -> string {
  if(!operator bool()) return {};

  string title;
  title.size(12);
  for(uint index : range(12)) {
    char letter = data[0x24 + index];
    if(letter >= 0x20 && letter <= 0x7e) title.get()[index] = letter;
  }
  return title.strip();
}

}
