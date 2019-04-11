namespace Heuristics {

struct MSX2Plus {
  MSX2Plus(vector<uint8_t>& data, string location);
  explicit operator bool() const;
  auto manifest() const -> string;

private:
  vector<uint8_t>& data;
  string location;
};

MSX2Plus::MSX2Plus(vector<uint8_t>& data, string location) : data(data), location(location) {
}

MSX2Plus::operator bool() const {
  return (bool)data;
}

auto MSX2Plus::manifest() const -> string {
  string output;
  output.append("game\n");
  output.append("  sha256: ", Hash::SHA256(data).digest(), "\n");
  output.append("  label:  ", Location::prefix(location), "\n");
  output.append("  name:   ", Location::prefix(location), "\n");
  output.append("  board\n");
  output.append(Memory{}.type("ROM").size(data.size()).content("Program").text());
  return output;
}

}
