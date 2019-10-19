auto Cartridge::read(uint16 addr) -> maybe<uint8> {
  if(!node) return nothing;

  if(addr < 0x8000) return rom.read(addr);
  else if(addr >= 0x8000) return ram.read(addr & 0x7fff);
  else return nothing;

  unreachable;
}

auto Cartridge::write(uint16 addr, uint8 data) -> bool {
  if(!node) return false;

  if(addr >= 0x8000)
  {
    ram.write(addr & 0x7fff, data);
    return true;
  }
  else return false;

  unreachable;
}
