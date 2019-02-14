auto CPU::Keypad::run() -> void {
  if(!enable) return;
  controls.poll();

  static const bool lookup[] = {
    controls.a->value,
    controls.b->value,
    controls.select->value,
    controls.start->value,
    controls.rightLatch,
    controls.leftLatch,
    controls.upLatch,
    controls.downLatch,
    controls.r->value,
    controls.l->value,
  };

  bool test = condition;  //0 = OR, 1 = AND
  for(uint index : range(10)) {
    if(!flag[index]) continue;
    bool input = lookup[index];
    if(condition == 0) test |= input;
    if(condition == 1) test &= input;
  }
  if(test) cpu.irq.flag |= CPU::Interrupt::Keypad;
}
