auto CPU::serialize(serializer& s) -> void {
  HuC6280::serialize(s);
  Thread::serialize(s);

  ram.serialize(s);
  bram.serialize(s);

  s.integer(irq.disableExternal);
  s.integer(irq.disableVDC);
  s.integer(irq.disableTimer);
  s.integer(irq.pendingIRQ);
  s.integer(irq.pendingVector);

  s.integer(timer.line);
  s.integer(timer.enable);
  s.integer(timer.reload);
  s.integer(timer.value);
  s.integer(timer.counter);

  s.integer(io.mdr);
}
