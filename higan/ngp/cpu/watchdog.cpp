auto CPU::Watchdog::step(uint clocks) -> void {
  if(!enable) return;
  timeout -= clocks;
  if(timeout > 0) return;
  cpu.intwd.trigger();
}

auto CPU::Watchdog::disable() -> void {
  enable = false;
}

auto CPU::Watchdog::reload() -> void {
  timeout = cpu.frequency() >> 2;
}
