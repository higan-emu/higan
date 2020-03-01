auto VDP::DMA::transfer() -> void {
  if(!io.enable || io.wait) return;
  if(!vdp.io.command.bit(5)) return;

  //load
  if(io.mode <= 1) {
    if(mcd.handle()
    && (io.mode.bit(0) << 23 | io.source << 1) >= 0x200000
    && (io.mode.bit(0) << 23 | io.source << 1) <= 0x23ffff
    ) {
      io.source.bit(0,15)++;
      io.length--;
    }

    do {
      auto data = cpu.read(1, 1, io.mode.bit(0) << 23 | io.source << 1);
      vdp.writeDataPort(data);
      io.source.bit(0,15)++;
    } while(--io.length);
    vdp.io.command.bit(5) = 0;
  }

  //fill
  if(io.mode == 2) {
    do {
      vdp.vram.writeByte(vdp.io.address, io.fill);
      io.source.bit(0,15)++;
      vdp.io.address += vdp.io.dataIncrement;
    } while(--io.length);
    vdp.io.command.bit(5) = 0;
  }

  //copy
  if(io.mode == 3) {
    if(!vdp.io.command.bit(4)) return;
    do {
      auto data = vdp.vram.readByte(io.source);
      vdp.vram.writeByte(vdp.io.address, data);
      io.source.bit(0,15)++;
      vdp.io.address += vdp.io.dataIncrement;
    } while(--io.length);
    vdp.io.command.bit(5) = 0;
  }
}
