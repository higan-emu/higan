auto Cartridge::MBC5::load(Node::Object parent, Node::Object from) -> void {
  rumble = Node::append<Node::Rumble>(parent, from, "Rumble");
}

auto Cartridge::MBC5::read(uint16 address) -> uint8 {
  if((address & 0xc000) == 0x0000) {  //$0000-3fff
    return cartridge.rom.read(address(0,13));
  }

  if((address & 0xc000) == 0x4000) {  //$4000-7fff
    return cartridge.rom.read(io.rom.bank << 14 | address(0,13));
  }

  if((address & 0xe000) == 0xa000) {  //$a000-bfff
    if(!io.ram.enable) return 0xff;
    return cartridge.ram.read(io.ram.bank << 13 | address(0,12));
  }

  return 0xff;
}

auto Cartridge::MBC5::write(uint16 address, uint8 data) -> void {
  if((address & 0xe000) == 0x0000) {  //$0000-1fff
    io.ram.enable = data(0,3) == 0x0a;
    return;
  }

  if((address & 0xf000) == 0x2000) {  //$2000-2fff
    io.rom.bank(0,7) = data(0,7);
    return;
  }

  if((address & 0xf000) == 0x3000) {  //$3000-3fff
    io.rom.bank(8) = data(0);
    return;
  }

  if((address & 0xe000) == 0x4000) {  //$4000-5fff
    if(cartridge.rumble) {
      //todo: add rumble timeout?
      rumble->enable = data(3);
      platform->input(rumble);
    }
    io.ram.bank = data(0,3);
    return;
  }

  if((address & 0xe000) == 0xa000) {  //$a000-bfff
    if(!io.ram.enable) return;
    return cartridge.ram.write(io.ram.bank << 13 | address(0,12), data);
  }
}

auto Cartridge::MBC5::power() -> void {
  io = {};
}

auto Cartridge::MBC5::serialize(serializer& s) -> void {
  s.integer(io.rom.bank);
  s.integer(io.ram.enable);
  s.integer(io.ram.bank);
}
