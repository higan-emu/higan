//HVC-FMR-03
//HVC-FMR-04

struct HVC_FMR : Board {
  HVC_FMR(Markup::Node& document) : Board(document) {
    fds.present = 1;
  }

  auto readPRG(uint address) -> uint8 {
    if(address >= 0x6000 && address <= 0xdfff) return prgram.read(address);
    if(address >= 0xe000 && address <= 0xffff) return prgrom.read(address);
    return cpu.mdr();
  }

  auto writePRG(uint address, uint8 data) -> void {
    if(address >= 0x6000 && address <= 0xdfff) return prgram.write(address, data);
  }

  auto readCHR(uint address) -> uint8 {
    if(address & 0x2000) {
      if(fds.mirroring() == 0) address = ((address & 0x8000) >> 1) | (address & 0x03ff);
      return ppu.readCIRAM(address & 0x07ff);
    }
    return chrram.read(address);
  }

  auto writeCHR(uint address, uint8 data) -> void {
    if(address & 0x2000) {
      if(fds.mirroring() == 0) address = ((address & 0x8000) >> 1) | (address & 0x03ff);
      return ppu.writeCIRAM(address & 0x07ff, data);
    }
    return chrram.write(address, data);
  }

  auto power() -> void {
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);
  }
};
