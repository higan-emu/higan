auto CPU::Watchdog::step(uint clocks) -> void {
  if(!enable) return;
  timeout -= clocks;
  if(timeout <= 0) {
    cpu.intwd = 1;
    cpu.intwd = 0;
  }
}

auto CPU::Watchdog::disable() -> void {
  enable = false;
}

auto CPU::Watchdog::reload() -> void {
  timeout = cpu.frequency() >> 3;
}
