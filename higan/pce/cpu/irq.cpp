auto CPU::IRQ::pending() const -> bool {
  return pendingIRQ;
}

auto CPU::IRQ::vector() const -> uint16 {
  return pendingVector;
}

auto CPU::IRQ::poll() -> void {
  pendingIRQ = false;
  if(cpu.r.p.i) return;

  if(!disableExternal && 0) {
    pendingIRQ = 1;
    pendingVector = 0xfff6;
  }

  if(!disableVDC && (vdp.vdc0.irqLine() | vdp.vdc1.irqLine())) {
    pendingIRQ = 1;
    pendingVector = 0xfff8;
  }

  if(!disableTimer && cpu.timer.irqLine()) {
    pendingIRQ = 1;
    pendingVector = 0xfffa;
  }
}
