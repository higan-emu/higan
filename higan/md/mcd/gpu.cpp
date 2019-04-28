//Graphics Processing Unit

auto MCD::GPU::step(uint clocks) -> void {
  if(!active) return;

  counter += clocks;
  while(counter >= period) {
    counter -= period;
    if(!--image.vdots) render();
  }
}

auto MCD::GPU::render() -> void {
  active = 0;
  irq.raise();
}

auto MCD::GPU::start() -> void {
  if(mcd.io.wramMode) return;  //must be in 2mbit WRAM mode

  period = 4 * 5 * image.hdots;
  active = 1;
}

auto MCD::GPU::power(bool reset) -> void {
}
