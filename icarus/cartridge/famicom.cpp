struct Famicom : Cartridge {
  auto name() -> string override { return "Famicom"; }
  auto export(string location) -> vector<uint8_t> override;
  auto heuristics(vector<uint8_t>& data, string location) -> string override;
};

auto Famicom::export(string location) -> vector<uint8_t> {
  vector<uint8_t> data;
  append(data, {location, "ines.rom"});
  append(data, {location, "program.rom"});
  append(data, {location, "character.rom"});
  return data;
}

auto Famicom::heuristics(vector<uint8_t>& data, string location) -> string {
  string digest = Hash::SHA256(data).digest();

  bool isFDS = false;
  //Nintendo Famicom Disk System (Japan)
  if(digest == "99c18490ed9002d9c6d999b9d8d15be5c051bdfa7cc7e73318053c9a994b0178") isFDS = true;
  //Sharp Twin Famicom (Japan)
  if(digest == "a0a9d57cbace21bf9c85c2b85e86656317f0768d7772acc90c7411ab1dbff2bf") isFDS = true;

  if(isFDS) {
    string s;
    s += "game\n";
    s += "  board: HVC-FMR\n";
    s += "    memory\n";
    s += "      type: ROM\n";
    s += "      size: 0x2000\n";
    s += "      content: Program\n";
    s += "    memory\n";
    s += "      type: RAM\n";
    s += "      size: 0x8000\n";
    s += "      content: Save\n";
    s += "      volatile\n";
    s += "    memory\n";
    s += "      type: RAM\n";
    s += "      size: 0x2000\n";
    s += "      content: Character\n";
    s += "      volatile\n";
    return s;
  }

  if(data.size() < 16) return {};
  if(data[0] != 'N') return {};
  if(data[1] != 'E') return {};
  if(data[2] != 'S') return {};
  if(data[3] != 0x1a) return {};

  uint mapper = ((data[7] >> 4) << 4) | (data[6] >> 4);
  uint mirror = ((data[6] & 0x08) >> 2) | (data[6] & 0x01);
  uint prgrom = data[4] * 0x4000;
  uint chrrom = data[5] * 0x2000;
  uint prgram = 0u;
  uint chrram = chrrom == 0u ? 8192u : 0u;

  string s;
  s += "game\n";

  switch(mapper) {
  default:
    s += "  board:  NES-NROM-256\n";
    s +={"    mirror mode=", !mirror ? "horizontal" : "vertical", "\n"};
    break;

  case   1:
    s += "  board:  NES-SXROM\n";
    s += "    chip type=MMC1B2\n";
    prgram = 8192;
    break;

  case   2:
    s += "  board:  NES-UOROM\n";
    s +={"    mirror mode=", !mirror ? "horizontal" : "vertical", "\n"};
    break;

  case   3:
    s += "  board:  NES-CNROM\n";
    s +={"    mirror mode=", !mirror ? "horizontal" : "vertical", "\n"};
    break;

  case   4:
    //MMC3
    s += "  board:  NES-TLROM\n";
    s += "    chip type=MMC3B\n";
    prgram = 8192;
    //MMC6
  //s += "  board:  NES-HKROM\n";
  //s += "    chip type=MMC6\n";
  //prgram = 1024;
    break;

  case   5:
    s += "  board:  NES-ELROM\n";
    s += "    chip type=MMC5\n";
    prgram = 65536;
    break;

  case   7:
    s += "  board:  NES-AOROM\n";
    break;

  case   9:
    s += "  board:  NES-PNROM\n";
    s += "    chip type=MMC2\n";
    prgram = 8192;
    break;

  case  10:
    s += "  board:  NES-FKROM\n";
    s += "    chip type=MMC4\n";
    prgram = 8192;
    break;

  case  16:
    s += "  board:  BANDAI-FCG\n";
    s += "    chip type=LZ93D50\n";
    break;

  case  21:
  case  23:
  case  25:
    //VRC4
    s += "  board:  KONAMI-VRC-4\n";
    s += "    chip type=VRC4\n";
    s += "      pinout a0=1 a1=0\n";
    prgram = 8192;
    break;

  case  22:
    //VRC2
    s += "  board:  KONAMI-VRC-2\n";
    s += "    chip type=VRC2\n";
    s += "      pinout a0=0 a1=1\n";
    break;

  case  24:
    s += "  board:  KONAMI-VRC-6\n";
    s += "    chip type=VRC6\n";
    break;

  case  26:
    s += "  board:  KONAMI-VRC-6\n";
    s += "    chip type=VRC6\n";
    prgram = 8192;
    break;

  case  34:
    s += "  board:  NES-BNROM\n";
    s +={"    mirror mode=", !mirror ? "horizontal" : "vertical", "\n"};
    break;

  case  66:
    s += "  board:  NES-GNROM\n";
    s +={"    mirror mode=", !mirror ? "horizontal" : "vertical", "\n"};
    break;

  case  69:
    s += "  board:  SUNSOFT-5B\n";
    s += "    chip type=5B\n";
    prgram = 8192;
    break;

  case  73:
    s += "  board:  KONAMI-VRC-3\n";
    s += "    chip type=VRC3\n";
    s +={"    mirror mode=", !mirror ? "horizontal" : "vertical", "\n"};
    prgram = 8192;
    break;

  case  75:
    s += "  board:  KONAMI-VRC-1\n";
    s += "    chip type=VRC1\n";
    break;

  case  85:
    s += "  board:  KONAMI-VRC-7\n";
    s += "    chip type=VRC7\n";
    prgram = 8192;
    break;
  }

  s += "    memory\n";
  s += "      type: ROM\n";
  s += "      size: 0x10\n";
  s += "      content: iNES\n";

  if(prgrom) {
    s += "    memory\n";
    s += "      type: ROM\n";
    s +={"      size: 0x", hex(prgrom), "\n"};
    s += "      content: Program\n";
  }

  if(prgram) {
    s += "    memory\n";
    s += "      type: RAM\n";
    s +={"      size: 0x", hex(prgram), "\n"};
    s += "      content: Save\n";
  }

  if(chrrom) {
    s += "    memory\n";
    s += "      type: ROM\n";
    s +={"      size: 0x", hex(chrrom), "\n"};
    s += "      content: Character\n";
  }

  if(chrram) {
    s += "    memory\n";
    s += "      type: RAM\n";
    s +={"      size: 0x", hex(chrram), "\n"};
    s += "      content: Character\n";
    s += "      volatile\n";
  }

  return s;
}
