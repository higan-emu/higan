auto CPU::Interrupt::operator=(bool value) -> void {
  set(value);
}

auto CPU::Interrupt::iff() const -> uint3 {
  if(!dmaAllowed) return priority;
  if(vector == cpu.dma0.vector) return 6;
  if(vector == cpu.dma1.vector) return 6;
  if(vector == cpu.dma2.vector) return 6;
  if(vector == cpu.dma3.vector) return 6;
  return priority;
}

auto CPU::Interrupt::test(maybe<Interrupt&>& i) const -> void {
  if(enable && request) {
    if(!maskable) {
      i = *this;
    } else {
      if(iff() != 7 && iff() >= i->iff()) i = *this;
    }
  }
}

auto CPU::Interrupt::fire() -> void {
  if(!dma()) cpu.interrupt(vector, priority);
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
}

auto CPU::Interrupt::lower() -> void {
  if(!enable || line == 0) return;
  line = 0;
  request |= level.low || edge.falling;
}

auto CPU::Interrupt::dma() -> bool {
  if(!dmaAllowed || cpu.r.iff == 7) return false;  //DMA always runs at priority 6

  if(vector == cpu.dma0.vector) {
    if(cpu.dma(0)) {
      cpu.dma0.vector = 0;
      cpu.inttc0 = 1;
      cpu.inttc0 = 0;
    }
    return true;
  }

  if(vector == cpu.dma1.vector) {
    if(cpu.dma(1)) {
      cpu.dma1.vector = 0;
      cpu.inttc1 = 1;
      cpu.inttc1 = 0;
    }
    return true;
  }

  if(vector == cpu.dma2.vector) {
    if(cpu.dma(2)) {
      cpu.dma2.vector = 0;
      cpu.inttc2 = 1;
      cpu.inttc2 = 0;
    }
    return true;
  }

  if(vector == cpu.dma3.vector) {
    if(cpu.dma(3)) {
      cpu.dma3.vector = 0;
      cpu.inttc3 = 1;
      cpu.inttc3 = 0;
    }
    return true;
  }

  return false;
}
