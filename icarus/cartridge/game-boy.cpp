struct GameBoy : Cartridge {
  auto name() -> string override { return "Game Boy"; }
  auto extensions() -> vector<string> override { return {"gb"}; }
  auto export(string location) -> vector<uint8_t> override;
  auto heuristics(vector<uint8_t>& data, string location) -> string override;
};

auto GameBoy::export(string location) -> vector<uint8_t> {
  vector<uint8_t> data;
  append(data, {location, "program.rom"});
  return data;
}

auto GameBoy::heuristics(vector<uint8_t>& data, string location) -> string {
  if(data.size() < 0x4000) return {};

  uint headerAddress = data.size() < 0x8000 ? data.size() : data.size() - 0x8000;
  auto read = [&](uint offset) { return data[headerAddress + offset]; };

  if(read(0x0104) == 0xce && read(0x0105) == 0xed && read(0x0106) == 0x66 && read(0x0107) == 0x66
  && read(0x0108) == 0xcc && read(0x0109) == 0x0d && read(0x0147) >= 0x0b && read(0x0147) <= 0x0d
  ) {
    //MMM01 stores header at bottom of data[]
  } else {
    //all other mappers store header at top of data[]
    headerAddress = 0;
  }

  bool black = (read(0x0143) & 0xc0) == 0x80;  //cartridge works in DMG+CGB mode
  bool clear = (read(0x0143) & 0xc0) == 0xc0;  //cartridge works in CGB mode only

  bool ram = false;
  bool battery = false;
  bool eeprom = false;
  bool flash = false;
  bool rtc = false;
  bool accelerometer = false;
  bool rumble = false;

  uint romSize = 0;
  uint ramSize = 0;
  uint eepromSize = 0;
  uint eepromWidth = 0;
  uint flashSize = 0;
  uint rtcSize = 0;

  string mapper = "MBC0";

  switch(read(0x0147)) {
  case 0x00:
    mapper = "MBC0";
    break;

  case 0x01:
    mapper = "MBC1";
    break;

  case 0x02:
    mapper = "MBC1";
    ram = true;
    break;

  case 0x03:
    mapper = "MBC1";
    battery = true;
    ram = true;
    break;

  case 0x05:
    mapper = "MBC2";
    ram = true;
    break;

  case 0x06:
    mapper = "MBC2";
    battery = true;
    ram = true;
    break;

  case 0x08:
    mapper = "MBC0";
    ram = true;
    break;

  case 0x09:
    mapper = "MBC0";
    battery = true;
    ram = true;
    break;

  case 0x0b:
    mapper = "MMM01";
    break;

  case 0x0c:
    mapper = "MMM01";
    ram = true;
    break;

  case 0x0d:
    mapper = "MMM01";
    battery = true;
    ram = true;
    break;

  case 0x0f:
    mapper = "MBC3";
    battery = true;
    rtc = true;
    break;

  case 0x10:
    mapper = "MBC3";
    battery = true;
    ram = true;
    rtc = true;
    break;

  case 0x11:
    mapper = "MBC3";
    break;

  case 0x12:
    mapper = "MBC3";
    ram = true;
    break;

  case 0x13:
    mapper = "MBC3";
    battery = true;
    ram = true;
    break;

  case 0x19:
    mapper = "MBC5";
    break;

  case 0x1a:
    mapper = "MBC5";
    ram = true;
    break;

  case 0x1b:
    mapper = "MBC5";
    battery = true;
    ram = true;
    break;

  case 0x1c:
    mapper = "MBC5";
    rumble = true;
    break;

  case 0x1d:
    mapper = "MBC5";
    ram = true;
    rumble = true;
    break;

  case 0x1e:
    mapper = "MBC5";
    battery = true;
    ram = true;
    rumble = true;
    break;

  case 0x20:
    mapper = "MBC6";
    flash = true;
    battery = true;
    ram = true;
    break;

  case 0x22:
    mapper = "MBC7";
    battery = true;
    eeprom = true;
    accelerometer = true;
    rumble = true;
    break;

  case 0xfc:
    mapper = "CAMERA";
    break;

  case 0xfd:
    mapper = "TAMA";
    battery = true;
    ram = true;
    rtc = true;
    break;

  case 0xfe:
    mapper = "HuC3";
    break;

  case 0xff:
    mapper = "HuC1";
    battery = true;
    ram = true;
    break;
  }

  //Game Boy: title = $0134-0143
  //Game Boy Color (early games): title = $0134-0142; model = $0143
  //Game Boy Color (later games): title = $0134-013e; serial = $013f-0142; model = $0143
  string title;
  for(uint n : range(black || clear ? 15 : 16)) {
    char byte = read(0x0134 + n);
    if(byte < 0x20 || byte > 0x7e) byte = ' ';
    title.append(byte);
  }

  string serial = title.slice(-4);
  if(!black && !clear) serial = "";
  for(auto& byte : serial) {
    if(byte >= 'A' && byte <= 'Z') continue;
    //invalid serial
    serial = "";
    break;
  }
  title.trimRight(serial, 1L);  //remove the serial from the title, if it exists
  title.strip();  //remove any excess whitespace from the title

  switch(read(0x0148)) { default:
  case 0x00: romSize =   2 * 16 * 1024; break;
  case 0x01: romSize =   4 * 16 * 1024; break;
  case 0x02: romSize =   8 * 16 * 1024; break;
  case 0x03: romSize =  16 * 16 * 1024; break;
  case 0x04: romSize =  32 * 16 * 1024; break;
  case 0x05: romSize =  64 * 16 * 1024; break;
  case 0x06: romSize = 128 * 16 * 1024; break;
  case 0x07: romSize = 256 * 16 * 1024; break;
  case 0x52: romSize =  72 * 16 * 1024; break;
  case 0x53: romSize =  80 * 16 * 1024; break;
  case 0x54: romSize =  96 * 16 * 1024; break;
  }

  switch(read(0x0149)) { default:
  case 0x00: ramSize =   0 * 1024; break;
  case 0x01: ramSize =   2 * 1024; break;
  case 0x02: ramSize =   8 * 1024; break;
  case 0x03: ramSize =  32 * 1024; break;
  case 0x04: ramSize = 128 * 1024; break;
  case 0x05: ramSize =  64 * 1024; break;
  }

  if(mapper == "MBC2" && ram) ramSize = 256;
  if(mapper == "MBC6" && ram) ramSize =  32 * 1024;
  if(mapper == "TAMA" && ram) ramSize =  32;

  if(mapper == "MBC6" && flash) flashSize = 1024 * 1024;

  //Game Boy header does not specify EEPROM size: detect via game title instead
  //Command Master:        EEPROM = 512 bytes
  //Kirby Tilt 'n' Tumble: EEPROM = 256 bytes
  //Korokoro Kirby:        EEPROM = 256 bytes
  if(mapper == "MBC7" && eeprom) {
    eepromSize = 256;  //fallback guess; supported values are 128, 256, 512, 1024, 2048
    eepromWidth = 16;  //93LCx6 supports 8-bit mode, but no licensed games use it
    if(title == "CMASTER"     && serial == "KCEJ") eepromSize = 512;
    if(title == "KIRBY TNT"   && serial == "KTNE") eepromSize = 256;
    if(title == "KORO2 KIRBY" && serial == "KKKJ") eepromSize = 256;
  }

  if(mapper == "MBC3" && rtc) rtcSize = 13;
  if(mapper == "TAMA" && rtc) rtcSize = 21;

  string s;
  s += "game\n";
  s +={"  name:   ", Media::name(location), "\n"};
  s +={"  label:  ", Media::name(location), "\n"};
  s +={"  title:  ", title, "\n"};
  if(serial)
  s +={"  serial: ", serial, "\n"};
  s +={"  board:  ", mapper, "\n"};

  s += "    memory\n";
  s += "      type: ROM\n";
  s +={"      size: 0x", hex(data.size()), "\n"};
  s += "      content: Program\n";

  if(ram && ramSize) {
    s += "    memory\n";
    s += "      type: RAM\n";
    s +={"      size: 0x", hex(ramSize), "\n"};
    s += "      content: Save\n";
    if(!battery)
    s += "      volatile\n";
  }

  if(eeprom && eepromSize) {
    s += "    memory\n";
    s += "      type: EEPROM\n";
    s +={"      size: 0x", hex(eepromSize), "\n"};
    s +={"      width: ", eepromWidth, "\n"};
    s += "      content: Save\n";
  }

  if(flash && flashSize) {
    s += "    memory\n";
    s += "      type: Flash\n";
    s +={"      size: 0x", hex(flashSize), "\n"};
    s += "      content: Download\n";
  }

  if(rtc && rtcSize) {
    s += "    memory\n";
    s += "      type: RTC\n";
    s +={"      size: 0x", hex(rtcSize), "\n"};
    s += "      content: Time\n";
  }

  if(accelerometer)
  s += "    accelerometer\n";

  if(rumble)
  s += "    rumble\n";

  return s;
}
