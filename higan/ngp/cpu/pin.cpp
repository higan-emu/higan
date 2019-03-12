//PA1
//TI0
auto CPU::setPin33(bool line) -> void {
  if(pins.p33 == line) return;
  pins.p33 = line;
  timers.input0(line);
}

//PA2
//TO1
auto CPU::setPin34(bool line) -> void {
  if(pins.p34 == line) return;
  pins.p34 = line;
}

//PA3
//TO3
auto CPU::setPin35(bool line) -> void {
  if(pins.p35 == line) return;
  pins.p35 = line;
  if(line) apu.nmi.line = 1;
}

//PB0
//TI4
//INT4
auto CPU::setPin36(bool line) -> void {
  if(pins.p36 == line) return;
  pins.p36 = line;
  int4.set(line);
  timers.input4(line);
}

//PB1
//TI5
//INT5
auto CPU::setPin37(bool line) -> void {
  if(pins.p37 == line) return;
  pins.p37 = line;
  int5.set(line);
  timers.input5(line);
}

//PB2
//TO4
auto CPU::setPin38(bool line) -> void {
  if(pins.p38 == line) return;
  pins.p38 = line;
}

//PB3
//TO5
auto CPU::setPin39(bool line) -> void {
  if(pins.p39 == line) return;
  pins.p39 = line;
}

//PB4
//TI6
//INT6
auto CPU::setPin40(bool line) -> void {
  if(pins.p40 == line) return;
  pins.p40 = line;
  int6.set(line);
  timers.input6(line);
}

//PB5
//TI7
//INT7
auto CPU::setPin41(bool line) -> void {
  if(pins.p41 == line) return;
  pins.p41 = line;
  int7.set(line);
  timers.input7(line);
}

//PB6
//TO6
auto CPU::setPin42(bool line) -> void {
  if(pins.p42 == line) return;
  pins.p42 = line;
}
