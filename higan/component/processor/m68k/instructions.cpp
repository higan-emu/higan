auto M68K::instructionABCD(EffectiveAddress from, EffectiveAddress with) -> void {
  auto source = read<Byte>(from);
  auto target = read<Byte, Hold>(with);
  auto result = source + target + r.x;
  bool c = false;
  bool v = false;

  if(((target ^ source ^ result) & 0x10) || (result & 0x0f) >= 0x0a) {
    auto previous = result;
    result += 0x06;
    v |= ((~previous & 0x80) & (result & 0x80));
  }

  if(result >= 0xa0) {
    auto previous = result;
    result += 0x60;
    c  = true;
    v |= ((~previous & 0x80) & (result & 0x80));
  }

  write<Byte>(with, result);

  r.c = c;
  r.v = v;
  r.z = clip<Byte>(result) ? 0 : r.z;
  r.n = sign<Byte>(result) < 0;
  r.x = r.c;
}

template<uint Size> auto M68K::instructionADD(EffectiveAddress from, DataRegister with) -> void {
  if constexpr(Size == Long) {
    if(from.mode == DataRegisterDirect || from.mode == AddressRegisterDirect || from.mode == Immediate) {
      idle(4);
    } else {
      idle(2);
    }
  }
  auto source = read<Size>(from);
  auto target = read<Size>(with);
  auto result = ADD<Size>(source, target);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionADD(DataRegister from, EffectiveAddress with) -> void {
  auto source = read<Size>(from);
  auto target = read<Size, Hold>(with);
  auto result = ADD<Size>(source, target);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionADDA(EffectiveAddress from, AddressRegister with) -> void {
  if(from.mode == DataRegisterDirect || from.mode == AddressRegisterDirect || from.mode == Immediate) {
    idle(4);
  } else {
    idle(2);
  }
  auto source = sign<Size>(read<Size>(from));
  auto target = read<Long>(with);
  write<Long>(with, source + target);
}

template<uint Size> auto M68K::instructionADDI(EffectiveAddress with) -> void {
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(4);
  }
  auto source = prefetch<Size>();
  auto target = read<Size, Hold>(with);
  auto result = ADD<Size>(source, target);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionADDQ(uint4 immediate, EffectiveAddress with) -> void {
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(4);
  }
  auto source = immediate;
  auto target = read<Size, Hold>(with);
  auto result = ADD<Size>(source, target);
  write<Size>(with, result);
}

//Size is ignored: always uses Long
template<uint Size> auto M68K::instructionADDQ(uint4 immediate, AddressRegister with) -> void {
  idle(4);
  auto result = read<Long>(with) + immediate;
  write<Long>(with, result);
}

template<uint Size> auto M68K::instructionADDX(EffectiveAddress from, EffectiveAddress with) -> void {
  auto source = read<Size>(from);
  auto target = read<Size, Hold>(with);
  auto result = ADD<Size, Extend>(source, target);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionAND(EffectiveAddress from, DataRegister with) -> void {
  if constexpr(Size == Long) {
    if(from.mode == DataRegisterDirect || from.mode == AddressRegisterDirect || from.mode == Immediate) {
      idle(4);
    } else {
      idle(2);
    }
  }
  auto source = read<Size>(from);
  auto target = read<Size>(with);
  auto result = AND<Size>(source, target);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionAND(DataRegister from, EffectiveAddress with) -> void {
  auto source = read<Size>(from);
  auto target = read<Size, Hold>(with);
  auto result = AND<Size>(source, target);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionANDI(EffectiveAddress with) -> void {
  if constexpr(Size == Long) {
    //note: m68000um.pdf erroneously lists ANDI.L #,Dn as 14(3/0), but is in fact 16(3/0)
    if(with.mode == DataRegisterDirect) idle(4);
  }
  auto source = prefetch<Size>();
  auto target = read<Size, Hold>(with);
  auto result = AND<Size>(source, target);
  write<Size>(with, result);
}

auto M68K::instructionANDI_TO_CCR() -> void {
  auto data = prefetch<Word>();
  writeCCR(readCCR() & data);
  idle(8);
  read<Word>(r.pc);
}

auto M68K::instructionANDI_TO_SR() -> void {
  if(!supervisor()) return;

  auto data = prefetch<Word>();
  writeSR(readSR() & data);
  idle(8);
  read<Word>(r.pc);
}

template<uint Size> auto M68K::instructionASL(uint4 count, DataRegister with) -> void {
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = ASL<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionASL(DataRegister from, DataRegister with) -> void {
  auto count = read<Long>(from) & 63;
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = ASL<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

auto M68K::instructionASL(EffectiveAddress with) -> void {
  auto result = ASL<Word>(read<Word, Hold>(with), 1);
  write<Word>(with, result);
}

template<uint Size> auto M68K::instructionASR(uint4 count, DataRegister with) -> void {
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = ASR<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionASR(DataRegister from, DataRegister with) -> void {
  auto count = read<Long>(from) & 63;
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = ASR<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

auto M68K::instructionASR(EffectiveAddress with) -> void {
  auto result = ASR<Word>(read<Word, Hold>(with), 1);
  write<Word>(with, result);
}

auto M68K::instructionBCC(uint4 test, uint8 displacement) -> void {
  auto offset = displacement ? (int8_t)displacement : (int16_t)prefetch<Word>() - 2;
  if(test >= 2 && !condition(test)) return;
  idle(2);
  r.pc -= 2;
  if(test == 1) push<Long>(r.pc);
  r.pc += offset;
  prefetch<Word>();
}

template<uint Size> auto M68K::instructionBCHG(DataRegister bit, EffectiveAddress with) -> void {
  auto index = read<Size>(bit) & bits<Size>() - 1;
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(index < 16 ? 2 : 4);
  }
  auto test = read<Size, Hold>(with);
  r.z = test.bit(index) == 0;
  test.bit(index) ^= 1;
  write<Size>(with, test);
}

template<uint Size> auto M68K::instructionBCHG(EffectiveAddress with) -> void {
  auto index = prefetch<Word>() & bits<Size>() - 1;
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(index < 16 ? 2 : 4);
  }
  auto test = read<Size, Hold>(with);
  r.z = test.bit(index) == 0;
  test.bit(index) ^= 1;
  write<Size>(with, test);
}

template<uint Size> auto M68K::instructionBCLR(DataRegister bit, EffectiveAddress with) -> void {
  auto index = read<Size>(bit) & bits<Size>() - 1;
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(index < 16 ? 4 : 6);
  }
  auto test = read<Size, Hold>(with);
  r.z = test.bit(index) == 0;
  test.bit(index) = 0;
  write<Size>(with, test);
}

template<uint Size> auto M68K::instructionBCLR(EffectiveAddress with) -> void {
  auto index = prefetch<Word>() & bits<Size>() - 1;
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(index < 16 ? 4 : 6);
  }
  auto test = read<Size, Hold>(with);
  r.z = test.bit(index) == 0;
  test.bit(index) = 0;
  write<Size>(with, test);
}

template<uint Size> auto M68K::instructionBSET(DataRegister bit, EffectiveAddress with) -> void {
  auto index = read<Size>(bit) & bits<Size>() - 1;
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(index < 16 ? 2 : 4);
  }
  auto test = read<Size, Hold>(with);
  r.z = test.bit(index) == 0;
  test.bit(index) = 1;
  write<Size>(with, test);
}

template<uint Size> auto M68K::instructionBSET(EffectiveAddress with) -> void {
  auto index = prefetch<Word>() & bits<Size>() - 1;
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(index < 16 ? 2 : 4);
  }
  auto test = read<Size, Hold>(with);
  r.z = test.bit(index) == 0;
  test.bit(index) = 1;
  write<Size>(with, test);
}

template<uint Size> auto M68K::instructionBTST(DataRegister bit, EffectiveAddress with) -> void {
  auto index = read<Size>(bit) & bits<Size>() - 1;
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(2);
  }
  auto test = read<Size>(with);
  r.z = test.bit(index) == 0;
}

template<uint Size> auto M68K::instructionBTST(EffectiveAddress with) -> void {
  auto index = prefetch<Word>() & bits<Size>() - 1;
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(2);
  }
  auto test = read<Size>(with);
  r.z = test.bit(index) == 0;
}

auto M68K::instructionCHK(DataRegister compare, EffectiveAddress maximum) -> void {
  idle(6);
  auto source = read<Word>(maximum);
  auto target = read<Word>(compare);

  r.z = clip<Word>(target) == 0;
  r.n = sign<Word>(target) < 0;
  if(r.n) return exception(Exception::BoundsCheck, Vector::BoundsCheck);

  auto result = (uint64)target - source;
  r.c = sign<Word>(result >> 1) < 0;
  r.v = sign<Word>((target ^ source) & (target ^ result)) < 0;
  r.z = clip<Word>(result) == 0;
  r.n = sign<Word>(result) < 0;
  if(r.n == r.v && !r.z) return exception(Exception::BoundsCheck, Vector::BoundsCheck);
}

template<uint Size> auto M68K::instructionCLR(EffectiveAddress with) -> void {
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect || with.mode == AddressRegisterDirect) idle(2);
  }
  read<Size, Hold>(with);
  write<Size>(with, 0);

  r.c = 0;
  r.v = 0;
  r.z = 1;
  r.n = 0;
}

template<uint Size> auto M68K::instructionCMP(EffectiveAddress from, DataRegister with) -> void {
  if constexpr(Size == Long) idle(2);
  auto source = read<Size>(from);
  auto target = read<Size>(with);
  CMP<Size>(source, target);
}

template<uint Size> auto M68K::instructionCMPA(EffectiveAddress from, AddressRegister with) -> void {
  idle(2);
  auto source = sign<Size>(read<Size>(from));
  auto target = read<Long>(with);
  CMP<Long>(source, target);
}

template<uint Size> auto M68K::instructionCMPI(EffectiveAddress with) -> void {
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(2);
  }
  auto source = prefetch<Size>();
  auto target = read<Size>(with);
  CMP<Size>(source, target);
}

template<uint Size> auto M68K::instructionCMPM(EffectiveAddress from, EffectiveAddress with) -> void {
  auto source = read<Size>(from);
  auto target = read<Size>(with);
  CMP<Size>(source, target);
}

auto M68K::instructionDBCC(uint4 test, DataRegister with) -> void {
  auto displacement = prefetch<Word>();
  if(condition(test)) {
    idle(4);
  } else {
    uint16 result = read<Word>(with);
    write<Word>(with, result - 1);
    if(result) {
      idle(2);
      r.pc -= 4;
      r.pc += sign<Word>(displacement);
      prefetch<Word>();
    } else {
      idle(2);
    }
  }
}

auto M68K::instructionDIVS(EffectiveAddress from, DataRegister with) -> void {
  uint32 dividend = read<Long>(with), odividend = dividend;
  uint32 divisor  = read<Word>(from) << 16, odivisor = divisor;

  if(divisor == 0) {
    return exception(Exception::DivisionByZero, Vector::DivisionByZero);
  }

  if(divisor >> 31) {
    divisor = -divisor;
  }

  if(dividend >> 31) {
    dividend = -dividend;
    idle(2);
  }

  r.c = 0;
  if(r.v = dividend >= divisor) {
    r.z = 0;
    r.n = 1;
    return idle(14);
  }

  uint16 quotient = 0;
  bool carry = 0;
  uint ticks = 12;
  for(uint index : range(15)) {
    dividend = dividend << 1;
    quotient = quotient << 1 | carry;
    if(carry = dividend >= divisor) dividend -= divisor;
    ticks += !carry ? 8 : 6;
  }
  quotient = quotient << 1 | carry;
  dividend = dividend << 1;
  if(carry = dividend >= divisor) dividend -= divisor;
  quotient = quotient << 1 | carry;
  ticks += 4;

  if(odivisor >> 31) {
    ticks += 16;
    if(odividend >> 31) {
      if(quotient >> 15) r.v = 1;
      dividend = -dividend;
    } else {
      quotient = -quotient;
      if(quotient && !(quotient >> 15)) r.v = 1;
    }
  } else if(odividend >> 31) {
    ticks += 18;
    quotient = -quotient;
    if(quotient && !(quotient >> 15)) r.v = 1;
    dividend = -dividend;
  } else {
    ticks += 14;
    if(quotient >> 15) r.v = 1;
  }

  if(r.v) {
    r.z = 0;
    r.n = 1;
    return idle(ticks);
  }

  r.z = quotient == 0;
  r.n = quotient < 0;

  idle(ticks);
  write<Long>(with, dividend | quotient);
}

auto M68K::instructionDIVU(EffectiveAddress from, DataRegister with) -> void {
  uint32 dividend = read<Long>(with);
  uint32 divisor  = read<Word>(from) << 16;

  if(divisor == 0) {
    return exception(Exception::DivisionByZero, Vector::DivisionByZero);
  }

  r.c = 0;
  if(r.v = dividend >= divisor) {
    r.z = 0;
    r.n = 1;
    return idle(10);
  }

  uint16 quotient = 0;
  bool force = 0;
  bool carry = 0;
  uint ticks = 6;
  for(uint index : range(16)) {
    force = dividend >> 31;
    dividend = dividend << 1;
    quotient = quotient << 1 | carry;
    if(carry = force || dividend >= divisor) dividend -= divisor;
    ticks += !carry ? 8 : !force ? 6 : 4;
  }
  ticks += force ? 6 : carry ? 4 : 2;
  quotient = quotient << 1 | carry;

  r.z = quotient == 0;
  r.n = quotient < 0;

  idle(ticks);
  write<Long>(with, dividend | quotient);
}

template<uint Size> auto M68K::instructionEOR(DataRegister from, EffectiveAddress with) -> void {
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(4);
  }
  auto source = read<Size>(from);
  auto target = read<Size, Hold>(with);
  auto result = EOR<Size>(source, target);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionEORI(EffectiveAddress with) -> void {
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(4);
  }
  auto source = prefetch<Size>();
  auto target = read<Size, Hold>(with);
  auto result = EOR<Size>(source, target);
  write<Size>(with, result);
}

auto M68K::instructionEORI_TO_CCR() -> void {
  auto data = prefetch<Word>();
  writeCCR(readCCR() ^ data);
  idle(8);
  read<Word>(r.pc);
}

auto M68K::instructionEORI_TO_SR() -> void {
  if(!supervisor()) return;

  auto data = prefetch<Word>();
  writeSR(readSR() ^ data);
  idle(8);
  read<Word>(r.pc);
}

auto M68K::instructionEXG(DataRegister x, DataRegister y) -> void {
  auto z = read<Long>(x);
  write<Long>(x, read<Long>(y));
  write<Long>(y, z);
}

auto M68K::instructionEXG(AddressRegister x, AddressRegister y) -> void {
  auto z = read<Long>(x);
  write<Long>(x, read<Long>(y));
  write<Long>(y, z);
}

auto M68K::instructionEXG(DataRegister x, AddressRegister y) -> void {
  auto z = read<Long>(x);
  write<Long>(x, read<Long>(y));
  write<Long>(y, z);
}

template<> auto M68K::instructionEXT<Word>(DataRegister with) -> void {
  auto result = (int8)read<Byte>(with);
  write<Word>(with, result);

  r.c = 0;
  r.v = 0;
  r.z = clip<Word>(result) == 0;
  r.n = sign<Word>(result) < 0;
}

template<> auto M68K::instructionEXT<Long>(DataRegister with) -> void {
  auto result = (int16)read<Word>(with);
  write<Long>(with, result);

  r.c = 0;
  r.v = 0;
  r.z = clip<Long>(result) == 0;
  r.n = sign<Long>(result) < 0;
}

auto M68K::instructionILLEGAL(uint16 code) -> void {
  idle(6);
  r.pc -= 2;
  if(code.bits(12,15) == 0xa) return exception(Exception::Illegal, Vector::IllegalLineA);
  if(code.bits(12,15) == 0xf) return exception(Exception::Illegal, Vector::IllegalLineF);
  return exception(Exception::Illegal, Vector::IllegalInstruction);
}

auto M68K::instructionJMP(EffectiveAddress from) -> void {
  r.pc = fetch<Long>(from);
  prefetch<Word>();
}

auto M68K::instructionJSR(EffectiveAddress from) -> void {
  auto pc = fetch<Long>(from);
  push<Long>(r.pc - 2);
  r.pc = pc;
  prefetch<Word>();
}

auto M68K::instructionLEA(EffectiveAddress from, AddressRegister to) -> void {
  if(from.mode == AddressRegisterIndirectWithIndex) idle(2);
  write<Long>(to, fetch<Long>(from));
}

auto M68K::instructionLINK(AddressRegister with) -> void {
  auto displacement = (int16)prefetch<Word>();
  auto sp = AddressRegister{7};
  push<Long>(read<Long>(with));
  write<Long>(with, read<Long>(sp));
  write<Long>(sp, read<Long>(sp) + displacement);
}

template<uint Size> auto M68K::instructionLSL(uint4 count, DataRegister with) -> void {
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = LSL<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionLSL(DataRegister from, DataRegister with) -> void {
  auto count = read<Long>(from) & 63;
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = LSL<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

auto M68K::instructionLSL(EffectiveAddress with) -> void {
  auto result = LSL<Word>(read<Word, Hold>(with), 1);
  write<Word>(with, result);
}

template<uint Size> auto M68K::instructionLSR(uint4 count, DataRegister with) -> void {
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = LSR<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionLSR(DataRegister from, DataRegister with) -> void {
  auto count = read<Long>(from) & 63;
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = LSR<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

auto M68K::instructionLSR(EffectiveAddress with) -> void {
  auto result = LSR<Word>(read<Word, Hold>(with), 1);
  write<Word>(with, result);
}

template<uint Size> auto M68K::instructionMOVE(EffectiveAddress from, EffectiveAddress to) -> void {
  auto data = read<Size>(from);
  write<Size>(to, data);

  r.c = 0;
  r.v = 0;
  r.z = clip<Size>(data) == 0;
  r.n = sign<Size>(data) < 0;
}

template<uint Size> auto M68K::instructionMOVEA(EffectiveAddress from, AddressRegister to) -> void {
  auto data = sign<Size>(read<Size>(from));
  write<Long>(to, data);
}

template<uint Size> auto M68K::instructionMOVEM_TO_MEM(EffectiveAddress to) -> void {
  auto list = prefetch<Word>();
  auto addr = fetch<Long>(to);

  for(uint n : range(16)) {
    if(!list.bit(n)) continue;
    //pre-decrement mode traverses registers in reverse order {A7-A0, D7-D0}
    uint index = to.mode == AddressRegisterIndirectWithPreDecrement ? 15 - n : n;

    if(to.mode == AddressRegisterIndirectWithPreDecrement) addr -= bytes<Size>();
    auto data = index < 8 ? read<Size>(DataRegister{index}) : read<Size>(AddressRegister{index});
    write<Size>(addr, data);
    if(to.mode != AddressRegisterIndirectWithPreDecrement) addr += bytes<Size>();
  }

  AddressRegister with{to.reg};
  if(to.mode == AddressRegisterIndirectWithPreDecrement ) write<Long>(with, addr);
  if(to.mode == AddressRegisterIndirectWithPostIncrement) write<Long>(with, addr);
}

template<uint Size> auto M68K::instructionMOVEM_TO_REG(EffectiveAddress from) -> void {
  auto list = prefetch<Word>();
  auto addr = fetch<Long>(from);

  for(uint n : range(16)) {
    if(!list.bit(n)) continue;
    uint index = from.mode == AddressRegisterIndirectWithPreDecrement ? 15 - n : n;

    if(from.mode == AddressRegisterIndirectWithPreDecrement) addr -= bytes<Size>();
    auto data = read<Size>(addr);
    data = sign<Size>(data);
    index < 8 ? write<Long>(DataRegister{index}, data) : write<Long>(AddressRegister{index}, data);
    if(from.mode != AddressRegisterIndirectWithPreDecrement) addr += bytes<Size>();
  }

  //spurious extra word read cycle exclusive to MOVEM memory->register
  if(from.mode == AddressRegisterIndirectWithPreDecrement) addr -= 2;
  read<Word>(addr);

  AddressRegister with{from.reg};
  if(from.mode == AddressRegisterIndirectWithPreDecrement ) write<Long>(with, addr);
  if(from.mode == AddressRegisterIndirectWithPostIncrement) write<Long>(with, addr);
}

template<uint Size> auto M68K::instructionMOVEP(DataRegister from, EffectiveAddress to) -> void {
  auto address = fetch<Size>(to);
  auto data = read<Long>(from);
  uint shift = bits<Size>();
  for(auto _ : range(bytes<Size>())) {
    shift -= 8;
    write<Byte>(address, data >> shift);
    address += 2;
  }
}

template<uint Size> auto M68K::instructionMOVEP(EffectiveAddress from, DataRegister to) -> void {
  auto address = fetch<Size>(from);
  auto data = read<Long>(to);
  uint shift = bits<Size>();
  for(auto _ : range(bytes<Size>())) {
    shift -= 8;
    data &= ~(0xff << shift);
    data |= read<Byte>(address) << shift;
    address += 2;
  }
  write<Long>(to, data);
}

auto M68K::instructionMOVEQ(uint8 immediate, DataRegister to) -> void {
  write<Long>(to, sign<Byte>(immediate));

  r.c = 0;
  r.v = 0;
  r.z = clip<Byte>(immediate) == 0;
  r.n = sign<Byte>(immediate) < 0;
}

auto M68K::instructionMOVE_FROM_SR(EffectiveAddress to) -> void {
  if(to.mode == DataRegisterDirect) idle(2);
  auto data = readSR();
  write<Word>(to, data);
  if(to.mode == AddressRegisterIndirectWithPreDecrement) idle(2);
  if(to.mode != DataRegisterDirect) read<Word>(r.pc);
}

auto M68K::instructionMOVE_TO_CCR(EffectiveAddress from) -> void {
  idle(8);
  auto data = read<Word>(from);
  writeCCR(data);
}

auto M68K::instructionMOVE_TO_SR(EffectiveAddress from) -> void {
  if(!supervisor()) return;

  idle(8);
  auto data = read<Word>(from);
  writeSR(data);
}

auto M68K::instructionMOVE_FROM_USP(AddressRegister to) -> void {
  if(!supervisor()) return;

  write<Long>(to, r.sp);
}

auto M68K::instructionMOVE_TO_USP(AddressRegister from) -> void {
  if(!supervisor()) return;

  r.sp = read<Long>(from);
}

auto M68K::instructionMULS(EffectiveAddress from, DataRegister with) -> void {
  auto source = read<Word>(from);
  auto target = read<Word>(with);
  auto result = (int16)source * (int16)target;
  //+2 cycles per 0<>1 bit transition
  auto cycles = bit::count(uint16(source << 1) ^ source);
  idle(34 + cycles * 2);
  write<Long>(with, result);

  r.c = 0;
  r.v = 0;
  r.z = clip<Long>(result) == 0;
  r.n = sign<Long>(result) < 0;
}

auto M68K::instructionMULU(EffectiveAddress from, DataRegister with) -> void {
  auto source = read<Word>(from);
  auto target = read<Word>(with);
  auto result = source * target;
  //+2 cycles per bit set
  auto cycles = bit::count(source);
  idle(34 + cycles * 2);
  write<Long>(with, result);

  r.c = 0;
  r.v = 0;
  r.z = clip<Long>(result) == 0;
  r.n = sign<Long>(result) < 0;
}

auto M68K::instructionNBCD(EffectiveAddress with) -> void {
  if(with.mode == DataRegisterDirect || with.mode == AddressRegisterDirect) idle(2);
  auto source = read<Byte, Hold>(with);
  auto target = 0u;
  auto result = target - source - r.x;
  bool c = false;
  bool v = false;

  const bool adjustLo = (target ^ source ^ result) & 0x10;
  const bool adjustHi = result & 0x100;

  if(adjustLo) {
    auto previous = result;
    result -= 0x06;
    c  = (~previous & 0x80) & ( result & 0x80);
    v |= ( previous & 0x80) & (~result & 0x80);
  }

  if(adjustHi) {
    auto previous = result;
    result -= 0x60;
    c  = true;
    v |= (previous & 0x80) & (~result & 0x80);
  }

  write<Byte>(with, result);

  r.c = c;
  r.v = v;
  r.z = clip<Byte>(result) ? 0 : r.z;
  r.n = sign<Byte>(result) < 0;
  r.x = r.c;
}

template<uint Size> auto M68K::instructionNEG(EffectiveAddress with) -> void {
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect || with.mode == AddressRegisterDirect) idle(2);
  }
  auto result = SUB<Size>(read<Size, Hold>(with), 0);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionNEGX(EffectiveAddress with) -> void {
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect || with.mode == AddressRegisterDirect) idle(2);
  }
  auto result = SUB<Size, Extend>(read<Size, Hold>(with), 0);
  write<Size>(with, result);
}

auto M68K::instructionNOP() -> void {
}

template<uint Size> auto M68K::instructionNOT(EffectiveAddress with) -> void {
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect || with.mode == AddressRegisterDirect) idle(2);
  }
  auto result = ~read<Size, Hold>(with);
  write<Size>(with, result);

  r.c = 0;
  r.v = 0;
  r.z = clip<Size>(result) == 0;
  r.n = sign<Size>(result) < 0;
}

template<uint Size> auto M68K::instructionOR(EffectiveAddress from, DataRegister with) -> void {
  if constexpr(Size == Long) idle(4);
  auto source = read<Size>(from);
  auto target = read<Size>(with);
  auto result = OR<Size>(source, target);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionOR(DataRegister from, EffectiveAddress with) -> void {
  auto source = read<Size>(from);
  auto target = read<Size, Hold>(with);
  auto result = OR<Size>(source, target);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionORI(EffectiveAddress with) -> void {
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(4);
  }
  auto source = prefetch<Size>();
  auto target = read<Size, Hold>(with);
  auto result = OR<Size>(source, target);
  write<Size>(with, result);
}

auto M68K::instructionORI_TO_CCR() -> void {
  auto data = prefetch<Word>();
  writeCCR(readCCR() | data);
  idle(8);
  read<Word>(r.pc);
}

auto M68K::instructionORI_TO_SR() -> void {
  if(!supervisor()) return;

  auto data = prefetch<Word>();
  writeSR(readSR() | data);
  idle(8);
  read<Word>(r.pc);
}

auto M68K::instructionPEA(EffectiveAddress from) -> void {
  if(from.mode == AddressRegisterIndirectWithIndex) idle(2);
  auto data = fetch<Long>(from);
  push<Long>(data);
}

auto M68K::instructionRESET() -> void {
  if(!supervisor()) return;

  r.reset = 1;
  idle(128);
  r.reset = 0;
}

template<uint Size> auto M68K::instructionROL(uint4 count, DataRegister with) -> void {
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = ROL<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionROL(DataRegister from, DataRegister with) -> void {
  auto count = read<Long>(from) & 63;
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = ROL<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

auto M68K::instructionROL(EffectiveAddress with) -> void {
  auto result = ROL<Word>(read<Word, Hold>(with), 1);
  write<Word>(with, result);
}

template<uint Size> auto M68K::instructionROR(uint4 count, DataRegister with) -> void {
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = ROR<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionROR(DataRegister from, DataRegister with) -> void {
  auto count = read<Long>(from) & 63;
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = ROR<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

auto M68K::instructionROR(EffectiveAddress with) -> void {
  auto result = ROR<Word>(read<Word, Hold>(with), 1);
  write<Word>(with, result);
}

template<uint Size> auto M68K::instructionROXL(uint4 count, DataRegister with) -> void {
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = ROXL<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionROXL(DataRegister from, DataRegister with) -> void {
  auto count = read<Long>(from) & 63;
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = ROXL<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

auto M68K::instructionROXL(EffectiveAddress with) -> void {
  auto result = ROXL<Word>(read<Word, Hold>(with), 1);
  write<Word>(with, result);
}

template<uint Size> auto M68K::instructionROXR(uint4 count, DataRegister with) -> void {
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = ROXR<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionROXR(DataRegister from, DataRegister with) -> void {
  auto count = read<Long>(from) & 63;
  idle((Size != Long ? 2 : 4) + count * 2);
  auto result = ROXR<Size>(read<Size>(with), count);
  write<Size>(with, result);
}

auto M68K::instructionROXR(EffectiveAddress with) -> void {
  auto result = ROXR<Word>(read<Word, Hold>(with), 1);
  write<Word>(with, result);
}

auto M68K::instructionRTE() -> void {
  if(!supervisor()) return;

  auto sr = pop<Word>();
  r.pc = pop<Long>();
  writeSR(sr);
  prefetch<Word>();
}

auto M68K::instructionRTR() -> void {
  writeCCR(pop<Word>());
  r.pc = pop<Long>();
  prefetch<Word>();
}

auto M68K::instructionRTS() -> void {
  r.pc = pop<Long>();
  prefetch<Word>();
}

auto M68K::instructionSBCD(EffectiveAddress from, EffectiveAddress with) -> void {
  auto source = read<Byte>(from);
  auto target = read<Byte, Hold>(with);
  auto result = target - source - r.x;
  bool c = false;
  bool v = false;

  const bool adjustLo = (target ^ source ^ result) & 0x10;
  const bool adjustHi = result & 0x100;

  if(adjustLo) {
    auto previous = result;
    result -= 0x06;
    c  = (~previous & 0x80) & ( result & 0x80);
    v |= ( previous & 0x80) & (~result & 0x80);
  }

  if(adjustHi) {
    auto previous = result;
    result -= 0x60;
    c  = true;
    v |= (previous & 0x80) & (~result & 0x80);
  }

  write<Byte>(with, result);

  r.c = c;
  r.v = v;
  r.z = clip<Byte>(result) ? 0 : r.z;
  r.n = sign<Byte>(result) < 0;
  r.x = r.c;
}

auto M68K::instructionSCC(uint4 test, EffectiveAddress to) -> void {
  if(!condition(test)) {
    write<Byte>(to, 0);
  } else {
    if(to.mode == DataRegisterDirect) idle(2);
    write<Byte>(to, ~0);
  }
}

auto M68K::instructionSTOP() -> void {
  if(!supervisor()) return;

  auto sr = prefetch<Word>();
  writeSR(sr);
  r.stop = true;
}

template<uint Size> auto M68K::instructionSUB(EffectiveAddress from, DataRegister with) -> void {
  if constexpr(Size == Long) {
    if(from.mode == DataRegisterDirect || from.mode == AddressRegisterDirect || from.mode == Immediate) {
      idle(4);
    } else {
      idle(2);
    }
  }
  auto source = read<Size>(from);
  auto target = read<Size>(with);
  auto result = SUB<Size>(source, target);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionSUB(DataRegister from, EffectiveAddress with) -> void {
  auto source = read<Size>(from);
  auto target = read<Size, Hold>(with);
  auto result = SUB<Size>(source, target);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionSUBA(EffectiveAddress from, AddressRegister to) -> void {
  if(from.mode == DataRegisterDirect || from.mode == AddressRegisterDirect || from.mode == Immediate) {
    idle(4);
  } else {
    idle(2);
  }
  auto source = sign<Size>(read<Size>(from));
  auto target = read<Long>(to);
  write<Long>(to, target - source);
}

template<uint Size> auto M68K::instructionSUBI(EffectiveAddress with) -> void {
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(4);
  }
  auto source = prefetch<Size>();
  auto target = read<Size, Hold>(with);
  auto result = SUB<Size>(source, target);
  write<Size>(with, result);
}

template<uint Size> auto M68K::instructionSUBQ(uint4 immediate, EffectiveAddress with) -> void {
  if constexpr(Size == Long) {
    if(with.mode == DataRegisterDirect) idle(4);
  }
  auto source = immediate;
  auto target = read<Size, Hold>(with);
  auto result = SUB<Size>(source, target);
  write<Size>(with, result);
}

//Size is ignored: always uses Long
template<uint Size> auto M68K::instructionSUBQ(uint4 immediate, AddressRegister with) -> void {
  idle(4);
  auto result = read<Long>(with) - immediate;
  write<Long>(with, result);
}

template<uint Size> auto M68K::instructionSUBX(EffectiveAddress from, EffectiveAddress with) -> void {
  auto source = read<Size>(from);
  auto target = read<Size, Hold>(with);
  auto result = SUB<Size, Extend>(source, target);
  write<Size>(with, result);
}

auto M68K::instructionSWAP(DataRegister with) -> void {
  auto result = read<Long>(with);
  result = result >> 16 | result << 16;
  write<Long>(with, result);

  r.c = 0;
  r.v = 0;
  r.z = clip<Long>(result) == 0;
  r.n = sign<Long>(result) < 0;
}

auto M68K::instructionTAS(EffectiveAddress with) -> void {
  uint32 data;

  if(with.mode == DataRegisterDirect) {
    data = read<Byte, Hold>(with);
    write<Byte>(with, data | 0x80);
  } else {
    //Mega Drive models 1&2 have a bug that prevents TAS write from taking effect
    //this bugged behavior is required for certain software to function correctly
    data = read<Byte>(with);
    idle(6);
  }

  r.c = 0;
  r.v = 0;
  r.z = clip<Byte>(data) == 0;
  r.n = sign<Byte>(data) < 0;
}

auto M68K::instructionTRAP(uint4 vector) -> void {
  idle(6);
  exception(Exception::Trap, 32 + vector, r.i);
}

auto M68K::instructionTRAPV() -> void {
  if(r.v) {
    idle(6);
    exception(Exception::Overflow, Vector::Overflow);
  }
}

template<uint Size> auto M68K::instructionTST(EffectiveAddress from) -> void {
  auto data = read<Size>(from);

  r.c = 0;
  r.v = 0;
  r.z = clip<Size>(data) == 0;
  r.n = sign<Size>(data) < 0;
}

auto M68K::instructionUNLK(AddressRegister with) -> void {
  auto sp = AddressRegister{7};
  write<Long>(sp, read<Long>(with));
  write<Long>(with, pop<Long>());
}
