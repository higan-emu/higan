/* Oscillator frequency = 6,144,000.0hz
 * Prescaler frequency  =   384,000.0hz ( d3)
 * Prescaler divisor    =    96,000.0hz ( d5)
 * clock0 frequency     =    48,000.0hz ( d6)
 * clock1 frequency     =    12,000.0hz ( d8)
 * clock2 frequency     =     3,000.0hz (d10)
 * clock3 frequency     =       187.5hz (d14)
 */

auto CPU::Timers::step(uint clocks) -> void {
  if(!enable) return;
  while(clocks--) {
    auto latch = clock++;
    if(!latch.bit( 6) || clock.bit( 6)) continue; timer01.clock0(); timer23.clock0(); timer4.clock0(); timer5.clock0();
    if(!latch.bit( 8) || clock.bit( 8)) continue; timer01.clock1(); timer23.clock1(); timer4.clock1(); timer5.clock1();
    if(!latch.bit(10) || clock.bit(10)) continue; timer01.clock2(); timer23.clock2(); timer4.clock2(); timer5.clock2();
    if(!latch.bit(14) || clock.bit(14)) continue; timer01.clock3(); timer23.clock3();
  }
}

auto CPU::Timers::input0(bool line) -> void {
  if(line) timer01.clockExternal();
}

auto CPU::TimerPair::clockExternal() -> void {
  if(mode == 0) {
    if(mode == 0) clockLo();
    if(mode == 1) clockPair();
    if(mode == 2) clockPPG();
    if(mode == 3) clockPWM();
  }
}

auto CPU::TimerPair::clock0() -> void {
  if(lo.mode == 1) {
    if(mode == 0) clockLo();
    if(mode == 1) clockPair();
    if(mode == 2) clockPPG();
    if(mode == 3) clockPWM();
  }
  if(hi.mode == 1) {
    if(mode == 0) clockHi();
    if(mode == 3) clockHi();
  }
}

auto CPU::TimerPair::clock1() -> void {
  if(lo.mode == 2) {
    if(mode == 0) clockLo();
    if(mode == 1) clockPair();
    if(mode == 2) clockPPG();
    if(mode == 3) clockPWM();
  }
}

auto CPU::TimerPair::clock2() -> void {
  if(lo.mode == 3) {
    if(mode == 0) clockLo();
    if(mode == 1) clockPair();
    if(mode == 2) clockPPG();
    if(mode == 3) clockPWM();
  }
  if(hi.mode == 2) {
    if(mode == 0) clockHi();
    if(mode == 3) clockHi();
  }
}

auto CPU::TimerPair::clock3() -> void {
  if(hi.mode == 3) {
    if(mode == 0) clockHi();
    if(mode == 3) clockHi();
  }
}

auto CPU::TimerPair::clockLo() -> void {
  if(!lo.enable) return;
  ++lo.counter;
  bool equal = lo.counter == lo.compare;
  lo.interrupt->set(equal);
  if(equal) {
    if(ff.enable && ff.select == 0) ff.invert();
    if(hi.mode == 0) clockHi();
    lo.counter = 0;
  }
}

auto CPU::TimerPair::clockHi() -> void {
  if(!hi.enable) return;
  ++hi.counter;
  bool equal = hi.counter == hi.compare;
  hi.interrupt->set(equal);
  if(equal) {
    if(ff.enable && ff.select == 1) ff.invert();
    hi.counter = 0;
  }
}

auto CPU::TimerPair::clockPair() -> void {
  if(!lo.enable || !hi.enable) return;
  if(!++lo.counter) ++hi.counter;
  bool equal = lo.counter == lo.compare && hi.counter == hi.compare;
  hi.interrupt->set(equal);
  if(equal) {
    if(ff.enable) ff.invert();
    lo.counter = 0;
    hi.counter = 0;
  }
}

//programmable pulse generation
auto CPU::TimerPair::clockPPG() -> void {
  if(!lo.enable) return;
  ++lo.counter;
  bool equalLo = lo.counter == lo.compare;
  bool equalHi = lo.counter == hi.compare;
  lo.interrupt->set(equalLo);
  hi.interrupt->set(equalHi);
  if(equalLo || equalHi) {
    if(ff.enable) ff.invert();
  }
  if(equalHi) {
    if(buffer.enable) lo.compare = buffer.compare;
    lo.counter = 0;
  }
}

//pulse width modulation
auto CPU::TimerPair::clockPWM() -> void {
  if(!lo.enable) return;
  ++lo.counter;
  if(pwm == 1) lo.counter = (uint6)lo.counter;
  if(pwm == 2) lo.counter = (uint7)lo.counter;
  if(pwm == 3) lo.counter = (uint8)lo.counter;
  bool equal = lo.counter == lo.compare;
  bool pulse = lo.counter == 0;
  lo.interrupt->set(equal || pulse);
  if(equal || pulse) {
    if(ff.enable) ff.invert();
  }
  if(pulse) {
    if(buffer.enable) lo.compare = buffer.compare;
    lo.counter = 0;
  }
}

auto CPU::TimerFlipFlop::invert() -> void {
  edge(output ^= 1);
}

auto CPU::TimerFlipFlop::raise() -> void {
  if(!output) edge(output = 1);
}

auto CPU::TimerFlipFlop::lower() -> void {
  if(output) edge(output = 0);
}

auto CPU::TimerByte::enableIf(bool state) -> void {
  enable = state;
  if(!enable) {
    counter = 0;
    interrupt->set(0);
  }
}

//

auto CPU::Timers::input4(bool line) -> void {
  if(timer4.captureMode == 1 && line == 1) {
    timer4.captureA = timer4.counter;
    if(timer4.ffA.flipOnLoadA) timer4.outputA(timer4.ffA.output ^= 1);
  }
  if(timer4.captureMode == 2 && line == 1) {
    timer4.captureA = timer4.counter;
    if(timer4.ffA.flipOnLoadA) timer4.outputA(timer4.ffA.output ^= 1);
  }
  if(timer4.captureMode == 2 && line == 0) {
    timer4.captureB = timer4.counter;
    if(timer4.ffA.flipOnLoadB) timer4.outputA(timer4.ffA.output ^= 1);
    if(timer4.ffB.flipOnLoadB) timer4.outputB(timer4.ffB.output ^= 1);
  }
}

auto CPU::Timers::input5(bool line) -> void {
  if(timer4.captureMode == 1 && line == 1) {
    timer4.captureB = timer4.counter;
    if(timer4.ffA.flipOnLoadB) timer4.outputA(timer4.ffA.output ^= 1);
    if(timer4.ffB.flipOnLoadB) timer4.outputB(timer4.ffB.output ^= 1);
  }
}

auto CPU::Timers::input6(bool line) -> void {
  if(timer5.captureMode == 1 && line == 1) {
    timer5.captureA = timer5.counter;
    if(timer5.ffA.flipOnLoadA) timer5.outputA(timer5.ffA.output ^= 1);
  }
  if(timer5.captureMode == 2 && line == 1) {
    timer5.captureA = timer5.counter;
    if(timer5.ffA.flipOnLoadA) timer5.outputA(timer5.ffA.output ^= 1);
  }
  if(timer5.captureMode == 2 && line == 0) {
    timer5.captureB = timer5.counter;
    if(timer5.ffA.flipOnLoadB) timer5.outputA(timer5.ffA.output ^= 1);
    if(timer5.ffB.flipOnLoadB) timer5.outputB(timer5.ffB.output ^= 1);
  }
}

auto CPU::Timers::input7(bool line) -> void {
  if(timer5.captureMode == 1 && line == 1) {
    timer5.captureB = timer5.counter;
    if(timer5.ffA.flipOnLoadB) timer5.outputA(timer5.ffA.output ^= 1);
    if(timer5.ffB.flipOnLoadB) timer5.outputB(timer5.ffB.output ^= 1);
  }
}

auto CPU::TimerWord::enableIf(bool state) -> void {
  enable = state;
  if(!enable) {
    counter = 0;
    interruptA->set(0);
    interruptB->set(0);
  }
}

auto CPU::TimerWord::clock0() -> void {
  if(!enable || mode != 1) return;
  clock();
}

auto CPU::TimerWord::clock1() -> void {
  if(!enable || mode != 2) return;
  clock();
}

auto CPU::TimerWord::clock2() -> void {
  if(!enable || mode != 3) return;
  clock();
}

auto CPU::TimerWord::clock() -> void {
  ++counter;
  if(counter == compareA) {
    interruptA->set(1);
    if(ffA.flipOnMatchA) outputA(ffA.output ^= 1);
  } else {
    interruptA->set(0);
  }
  if(counter == compareB) {
    interruptB->set(1);
    if(ffA.flipOnMatchB) outputA(ffA.output ^= 1);
    if(ffB.flipOnMatchB) outputB(ffB.output ^= 1);
    if(buffer.enable) compareA = buffer.compare;
    if(clearOnMatch) counter = 0;
  } else {
    interruptB->set(0);
  }
}
