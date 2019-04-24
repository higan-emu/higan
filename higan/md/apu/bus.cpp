auto APU::read(uint16 address) -> uint8 {
  if((address & 0xe000) == 0x0000) return ram[address];

  if(address == 0x4000) return ym2612.readStatus();
  if(address == 0x4001) return ym2612.readStatus();
  if(address == 0x4002) return ym2612.readStatus();
  if(address == 0x4003) return ym2612.readStatus();

  if((address & 0x8000) == 0x8000) {
    if(cartridge.node) return cartridge.read(Byte, io.bank << 15 | (uint15)address, 0x00);
    if(expansion.node) return expansion.read(Byte, io.bank << 15 | (uint15)address, 0x00);
    return 0x00;
  }

  return 0x00;
}

auto APU::write(uint16 address, uint8 data) -> void {
  if((address & 0xe000) == 0x0000) return (void)(ram[address] = data);

  if(address == 0x4000) return ym2612.writeAddress(0 << 8 | data);
  if(address == 0x4001) return ym2612.writeData(data);
  if(address == 0x4002) return ym2612.writeAddress(1 << 8 | data);
  if(address == 0x4003) return ym2612.writeData(data);

  if(address == 0x6000) {
    //1-bit shift register
    io.bank = data.bit(0) << 8 | io.bank >> 1;
    return;
  }

  if(address == 0x7f11) return psg.write(data);
  if(address == 0x7f13) return psg.write(data);
  if(address == 0x7f15) return psg.write(data);
  if(address == 0x7f17) return psg.write(data);

  if((address & 0x8000) == 0x8000) {
    //todo: do 8-bit writes mirror to 16-bits?
    if(cartridge.node) return cartridge.write(Byte, io.bank << 15 | (uint15)address, data);
    if(expansion.node) return expansion.write(Byte, io.bank << 15 | (uint15)address, data);
    return;
  }
}

//unused on Mega Drive
auto APU::in(uint16 address) -> uint8 {
  return 0x00;
}

//unused on Mega Drive
auto APU::out(uint16 address, uint8 data) -> void {
}
