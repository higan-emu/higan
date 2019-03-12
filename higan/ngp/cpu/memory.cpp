auto CPU::read(uint24 address) -> uint8 {
  if(address >= 0x000000 && address <= 0x0000ff) return cpu.readIO(address);
  if(address >= 0x004000 && address <= 0x006fff) return cpu.ram.read(address);
  if(address >= 0x007000 && address <= 0x007fff) return apu.ram.read(address);
  if(address >= 0x008000 && address <= 0x00bfff) return vpu.read(address);
  if(address >= 0x200000 && address <= 0x3fffff) return cartridge.read(0, address);
  if(address >= 0x800000 && address <= 0x9fffff) return cartridge.read(1, address);
  if(address >= 0xff0000 && address <= 0xffffff) return system.bios.read(address);
  return 0x00;
}

auto CPU::write(uint24 address, uint8 data) -> void {
  if(address >= 0x000000 && address <= 0x0000ff) return cpu.writeIO(address, data);
  if(address >= 0x004000 && address <= 0x006fff) return cpu.ram.write(address, data);
  if(address >= 0x007000 && address <= 0x007fff) return apu.ram.write(address, data);
  if(address >= 0x008000 && address <= 0x00bfff) return vpu.write(address, data);
  if(address >= 0x200000 && address <= 0x3fffff) return cartridge.write(0, address, data);
  if(address >= 0x800000 && address <= 0x9fffff) return cartridge.write(1, address, data);
  if(address >= 0xff0000 && address <= 0xffffff) return system.bios.write(address, data);
}
