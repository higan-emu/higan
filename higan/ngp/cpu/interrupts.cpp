auto CPU::Interrupt::operator=(bool value) -> void {
  set(value);
}

auto CPU::Interrupt::test(maybe<Interrupt&>& i) const -> void {
  if(enable && request) {
    if(!maskable) {
      i = *this;
    } else {
      if(priority != 7 && priority >= i->priority) i = *this;
    }
  }
}

auto CPU::Interrupt::fire() -> void {
  cpu.interrupt(vector, priority);
  if(line == 0 && !level.low ) request = 0;
  if(line == 1 && !level.high) request = 0;
}

auto CPU::Interrupt::set(bool line) -> void {
  line ? raise() : lower();
}

auto CPU::Interrupt::clear() -> void {
  request = 0;
}

auto CPU::Interrupt::raise() -> void {
  if(!enable || line == 1) return;
  line = 1;
  request |= level.high || edge.rising;
  if(request) testDMA();
}

auto CPU::Interrupt::lower() -> void {
  if(!enable || line == 0) return;
  line = 0;
  request |= level.low || edge.falling;
  if(request) testDMA();
}

auto CPU::Interrupt::testDMA() -> void {
  if(!dmaAllowed || cpu.r.iff == 7) return;  //DMA always runs at priority 6

  if(vector == cpu.dma0.vector) {
    if(cpu.dma(0)) {
      cpu.dma0.vector = 0;
      cpu.inttc0.raise();
    }
    request = 0;
    return;
  }

  if(vector == cpu.dma1.vector) {
    if(cpu.dma(1)) {
      cpu.dma1.vector = 0;
      cpu.inttc1.raise();
    }
    request = 0;
    return;
  }

  if(vector == cpu.dma2.vector) {
    if(cpu.dma(2)) {
      cpu.dma2.vector = 0;
      cpu.inttc2.raise();
    }
    request = 0;
    return;
  }

  if(vector == cpu.dma3.vector) {
    if(cpu.dma(3)) {
      cpu.dma3.vector = 0;
      cpu.inttc3.raise();
    }
    request = 0;
    return;
  }
}
