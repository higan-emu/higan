auto CPU::read(uint16 address) -> uint8 {
  uint8 data = 0xff;
  if(auto result = cartridge.read(address)) {
    data = result();
  } else if(address >= 0xc000) {
    data = ram.read(address);
  }
  if(auto result = cheat.find(address, data)) data = result();
  return data;
}

auto CPU::write(uint16 address, uint8 data) -> void {
  if(cartridge.write(address, data)) {
  } else if(address >= 0xc000) {
    ram.write(address, data);
  }
}

auto CPU::in(uint16 address) -> uint8 {
  switch(address.bits(6,7)) {

  case 0: {
    if(Model::GameGear()) {
      platform->input(controls.start);
      bool start = !controls.start->value;
      return start << 7 | 0x7f;
    }

    return 0xff;  //SMS1 = MDR, SMS2 = 0xff
  }

  case 1: {
    return !address.bit(0) ? vdp.vcounter() : vdp.hcounter();
  }

  case 2: {
    return !address.bit(0) ? vdp.data() : vdp.status();
  }

  case 3: {
    if(Model::MasterSystem()) {
      platform->input(controls.reset);
      bool reset = !controls.reset->value;
      auto port1 = controllerPort1.read();
      auto port2 = controllerPort2.read();
      if(address.bit(0) == 0) {
        return port1.bits(0,5) << 0 | port2.bits(0,1) << 6;
      } else {
        return port2.bits(2,5) << 0 | reset << 4 | 1 << 5 | port1.bit(6) << 6 | port2.bit(6) << 7;
      }
    }

    if(Model::GameGear()) {
      if(address.bit(0) == 0) {
        controls.poll();
        bool up    = !controls.upLatch;
        bool down  = !controls.downLatch;
        bool left  = !controls.leftLatch;
        bool right = !controls.rightLatch;
        bool one   = !controls.one->value;
        bool two   = !controls.two->value;
        return up << 0 | down << 1 | left << 2 | right << 3 | one << 4 | two << 5 | 1 << 6 | 1 << 7;
      } else {
        return 0xff;
      }
    }

    return 0xff;
  }

  }

  return 0xff;
}

auto CPU::out(uint16 address, uint8 data) -> void {
  if((uint8)address == 0x06) {
    if(Model::GameGear()) return psg.balance(data);
  }

  switch(address.bits(6,7)) {

  case 1: {
    return psg.write(data);
  }

  case 2: {
    return !address.bit(0) ? vdp.data(data) : vdp.control(data);
  }

  }
}
