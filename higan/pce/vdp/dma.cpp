auto VDC::DMA::step(uint clocks) -> void {
  do {
    if(!vramActive && !satbActive) break;

    if(vramActive) {
      uint16 data = vdc->vram.read(source);
      vdc->vram.write(target, data);
      sourceIncrementMode == 0 ? source++ : source--;
      targetIncrementMode == 0 ? target++ : target--;
      if(!--length) {
        vramActive = 0;
        vdc->irq.raise(VDC::IRQ::Line::TransferVRAM);
      }
      continue;
    }

    if(satbActive) {
      uint16 data = vdc->vram.read(satbSource + satbOffset);
      vdc->satb.write(satbOffset, data);
      if(!++satbOffset) {
        satbActive = 0;
        vdc->irq.raise(VDC::IRQ::Line::TransferSATB);
      }
      continue;
    }
  } while(--clocks);
}

auto VDC::DMA::vramStart() -> void {
  vramActive = 1;
}

auto VDC::DMA::satbStart() -> void {
  if(satbPending || satbRepeat) {
    satbPending = 0;
    satbActive = 1;
    satbOffset = 0;
  }
}

auto VDC::DMA::satbQueue() -> void {
  satbPending = 1;
}
