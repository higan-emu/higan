auto CPU::Interrupt::test(maybe<Interrupt&>& i) const -> void {
  if(request && priority != 7 && priority >= i->priority) i = *this;
}

auto CPU::Interrupt::fired() -> void {
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
  line = 1;
  request = level.high || edge.rising;
}

auto CPU::Interrupt::lower() -> void {
  line = 0;
  request = level.low || edge.falling;
}
