// PA0
// /WAIT

CPU::Pin32::operator bool() const {
  return pin & !flow;
}

auto CPU::Pin32::operator=(bool input) -> void {
  if(!flow || pin == input) return;
  pin = input;
}

// PA1
// TI0
// (/HBLANK)

CPU::Pin33::operator bool() const {
  return pin & !flow;
}

auto CPU::Pin33::operator=(bool input) -> void {
  if(!flow || pin == input) return;
  pin = input;
  cpu.timers.input0(pin);
}

// PA2
// TO1

CPU::Pin34::operator bool() const {
  return pin & !flow;
}

auto CPU::Pin34::operator=(bool input) -> void {
  if(!flow || pin == input) return;
  pin = input;
}

// PA3
// TO3

CPU::Pin35::operator bool() const {
  return pin & !flow;
}

auto CPU::Pin35::operator=(bool input) -> void {
  if(!flow || pin == input) return;
  pin = input;
  if(pin) apu.irq.line = 1;
}

// PB0
// INT4
// TI4
// (/VBLANK)

CPU::Pin36::operator bool() const {
  return pin & !flow;
}

auto CPU::Pin36::operator=(bool input) -> void {
  if(!flow || pin == input) return;
  pin = input;
  cpu.int4.set(pin);
  cpu.timers.input4(pin);
}

// PB1
// INT5
// TI5
// (APU)

CPU::Pin37::operator bool() const {
  return pin & !flow;
}

auto CPU::Pin37::operator=(bool input) -> void {
  if(!flow || pin == input) return;
  pin = input;
  cpu.int5.set(pin);
  cpu.timers.input5(pin);
}

//PB2
//TO4

CPU::Pin38::operator bool() const {
  return pin & !flow;
}

auto CPU::Pin38::operator=(bool input) -> void {
  if(!flow || pin == input) return;
  pin = input;
}

//PB3
//TO5

CPU::Pin39::operator bool() const {
  return pin & !flow;
}

auto CPU::Pin39::operator=(bool input) -> void {
  if(!flow || pin == input) return;
  pin = input;
}

//PB4
//INT6
//TI6

CPU::Pin40::operator bool() const {
  return pin & !flow;
}

auto CPU::Pin40::operator=(bool input) -> void {
  if(!flow || pin == input) return;
  pin = input;
  cpu.int6.set(pin);
  cpu.timers.input6(pin);
}

//PB5
//INT7
//TI7

CPU::Pin41::operator bool() const {
  return pin & !flow;
}

auto CPU::Pin41::operator=(bool input) -> void {
  if(!flow || pin == input) return;
  pin = input;
  cpu.int7.set(pin);
  cpu.timers.input7(pin);
}

//PB6
//TO6

CPU::Pin42::operator bool() const {
  return pin & !flow;
}

auto CPU::Pin42::operator=(bool input) -> void {
  if(!flow || pin == input) return;
  pin = input;
}

// PB7
// INT0

CPU::Pin43::operator bool() const {
  return pin & !flow;
}

auto CPU::Pin43::operator=(bool input) -> void {
  if(!flow || pin == input) return;
  pin = input;
  cpu.int0.set(pin);
}
