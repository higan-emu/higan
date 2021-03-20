auto MCD::Timer::clock() -> void {
  if(setpoint && !--counter) {
    irq.raise();
    counter = setpoint;
  }
}

auto MCD::Timer::power(bool reset) -> void {
  irq = {};
  counter = setpoint = 0;
}
