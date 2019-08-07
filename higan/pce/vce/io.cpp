auto VCE::read(uint3 address) -> uint8 {
  if(address == 0x04) {
    //CTR
    uint8 data = cram.read(cram.address).bit(0,7);
    return data;
  }

  if(address == 0x05) {
    //CTR
    uint1 data = cram.read(cram.address).bit(8);
    cram.address++;
    return 0xfe | data;
  }

  return 0xff;
}

auto VCE::write(uint3 address, uint8 data) -> void {
  if(address == 0x00) {
    //CR
    if(data.bit(0,1) == 0) io.clock = 4;
    if(data.bit(0,1) == 1) io.clock = 3;
    if(data.bit(0,1) == 2) io.clock = 2;
    if(data.bit(0,1) == 3) io.clock = 2;
    io.extraLine = data.bit(2);
    io.grayscale = data.bit(7);
    return;
  }

  if(address == 0x02) {
    //CTA
    cram.address.bit(0,7) = data.bit(0,7);
    return;
  }

  if(address == 0x03) {
    //CTA
    cram.address.bit(8) = data.bit(0);
    return;
  }

  if(address == 0x04) {
    //CTW
    cram.write(cram.address, 0, data.bit(0,7));
    return;
  }

  if(address == 0x05) {
    //CTW
    cram.write(cram.address, 1, data.bit(0));
    cram.address++;
    return;
  }
}
