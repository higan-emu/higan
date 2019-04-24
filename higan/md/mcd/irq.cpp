auto MCD::IRQ::raise() -> void {
  pending = enable;
  synchronize();
}

auto MCD::IRQ::lower() -> void {
  pending = 0;
  synchronize();
}

auto MCD::IRQ::acknowledge() -> bool {
  if(!pending) return false;
  return lower(), true;
}

auto MCD::IRQ::synchronize() -> void {
  mcd.irq.pending |= mcd.irq.reset.pending;
  mcd.irq.pending |= mcd.irq.graphics.pending;
  mcd.irq.pending |= mcd.irq.external.pending;
  mcd.irq.pending |= mcd.timer.irq.pending;
  mcd.irq.pending |= mcd.cdd.irq.pending;
  mcd.irq.pending |= mcd.cdc.irq.pending;
  mcd.irq.pending |= mcd.irq.subcode.pending;
}
