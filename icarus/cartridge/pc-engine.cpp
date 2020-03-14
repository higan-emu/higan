struct PCEngine : Cartridge {
  auto name() -> string override { return "PC Engine"; }
  auto export(string location) -> vector<uint8_t> override;
  auto heuristics(vector<uint8_t>& data, string location) -> string override;
};

auto PCEngine::export(string location) -> vector<uint8_t> {
  vector<uint8_t> data;
  append(data, {location, "program.rom"});
  return data;
}

auto PCEngine::heuristics(vector<uint8_t>& data, string location) -> string {
  if((data.size() & 0x1fff) == 512) {
    //remove header if present
    memory::move(&data[0], &data[512], data.size() - 512);
    data.resize(data.size() - 512);
  }

  string board = "Linear";
  if(data.size() ==  0x60000) board = "Split";
  if(data.size() == 0x280000) board = "Banked";
  if(data.size() >=   0x2000 && !memory::compare(&data[0x1f26], "POPULOUS", 8)) board = "RAM";

  string s;
  s += "game\n";
  s +={"  name:  ", Media::name(location), "\n"};
  s +={"  label: ", Media::name(location), "\n"};
  s +={"  board: ", board, "\n"};
  s += "    memory\n";
  s += "      type: ROM\n";
  s +={"      size: 0x", hex(data.size()), "\n"};
  s += "      content: Program\n";
  if(board == "RAM") {
  s += "    memory\n";
  s += "      type: RAM\n";
  s += "      size: 0x8000\n";
  s += "      content: Save\n";
  }
  return s;
}
