template<uint Bits> auto TLCS900H::stepBW(uint b, uint w) -> void {
  if constexpr(Bits ==  8) return step(b);
  if constexpr(Bits == 16) return step(w);
}

template<uint Bits> auto TLCS900H::stepWL(uint w, uint l) -> void {
  if constexpr(Bits == 16) return step(w);
  if constexpr(Bits == 32) return step(l);
}

template<uint Bits> auto TLCS900H::stepBWL(uint b, uint w, uint l) -> void {
  if constexpr(Bits ==  8) return step(b);
  if constexpr(Bits == 16) return step(w);
  if constexpr(Bits == 32) return step(l);
}

template<> auto TLCS900H::toRegister3<uint8>(uint3 code) const -> Register<uint8> {
  static const Register< uint8> lookup[] = {W, A, B, C, D, E, H, L};
  return lookup[code];
}

template<> auto TLCS900H::toRegister3<uint16>(uint3 code) const -> Register<uint16> {
  static const Register<uint16> lookup[] = {WA, BC, DE, HL, IX, IY, IZ, SP};
  return lookup[code];
}

template<> auto TLCS900H::toRegister3<uint32>(uint3 code) const -> Register<uint32> {
  static const Register<uint32> lookup[] = {XWA, XBC, XDE, XHL, XIX, XIY, XIZ, XSP};
  return lookup[code];
}

template<typename T> auto TLCS900H::toRegister8(uint8 code) const -> Register<T> { return {code}; }
template<typename T> auto TLCS900H::toControlRegister(uint8 code) const -> ControlRegister<T> { return {code}; }
template<typename T> auto TLCS900H::toMemory(uint32 address) const -> Memory<T> { return {address}; }
template<typename T> auto TLCS900H::toImmediate(uint32 constant) const -> Immediate<T> { return {constant}; }
template<typename T> auto TLCS900H::toImmediate3(natural constant) const -> Immediate<T> { return {constant.clip(3) ? constant.clip(3) : 8}; }

//note: much of this code is split to multiple statements due to C++ not guaranteeing
//the order of evaluations of function arguments. fetch() ordering is critical.

auto TLCS900H::undefined() -> void {
  instructionSoftwareInterrupt(2);
  return step(1);
}

auto TLCS900H::instruction() -> void {
  auto data = fetch();

  switch(r.prefix = data) {
  case 0x00:
    instructionNoOperation();
    return step(2);
  case 0x01:
    return undefined();  //NORMAL (not present on 900/H)
  case 0x02:
    instructionPush(SR);
    return step(3);
  case 0x03:
    instructionPop(SR);
    return step(4);
  case 0x04:
    return undefined();  //MAX or MIN (not present on 900/H)
  case 0x05:
    instructionHalt();
    return step(6);
  case 0x06:
    instructionSetInterruptFlipFlop((uint3)fetch());
    return step(3);
  case 0x07:
    instructionReturnInterrupt();
    return step(12);
  case 0x08: {
    auto memory = fetchMemory<uint8, uint8>();
    instructionLoad(memory, fetchImmediate<uint8>());
    return step(5); }
  case 0x09:
    instructionPush(fetchImmediate<uint8>());
    return step(4);
  case 0x0a: {
    auto memory = fetchMemory<uint16, uint8>();
    instructionLoad(memory, fetchImmediate<uint16>());
    return step(6); }
  case 0x0b:
    instructionPush(fetchImmediate<uint16>());
    return step(5);
  case 0x0c:
    instructionSetRegisterFilePointer(RFP + 1);
    return step(2);
  case 0x0d:
    instructionSetRegisterFilePointer(RFP - 1);
    return step(2);
  case 0x0e:
    instructionReturn();
    return step(9);
  case 0x0f:
    instructionReturnDeallocate(fetchImmediate<int16>());
    return step(11);
  case 0x10:
    instructionSetFlag(CF, 0);
    return step(2);
  case 0x11:
    instructionSetFlag(CF, 1);
    return step(2);
  case 0x12:
    instructionSetFlag(CF, !CF);
    return step(2);
  case 0x13:
    instructionSetFlag(CF, !ZF);
    return step(2);
  case 0x14:
    instructionPush(A);
    return step(3);
  case 0x15:
    instructionPop(A);
    return step(4);
  case 0x16:
    instructionExchange(F, FP);
    return step(2);
  case 0x17:
    instructionSetRegisterFilePointer((uint2)fetch());
    return step(2);
  case 0x18:
    instructionPush(F);
    return step(3);
  case 0x19:
    instructionPop(F);
    return step(4);
  case 0x1a:
    instructionJump(fetchImmediate<uint16>());
    return step(5);
  case 0x1b:
    instructionJump(fetchImmediate<uint24>());
    return step(6);
  case 0x1c:
    instructionCall(fetchImmediate<uint16>());
    return step(9);
  case 0x1d:
    instructionCall(fetchImmediate<uint24>());
    return step(10);
  case 0x1e:
    instructionCallRelative(fetchImmediate<int16>());
    return step(10);
  case 0x1f:
    return undefined();
  case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
    instructionLoad(toRegister3<uint8>(data), fetchImmediate<uint8>());
    return step(2);
  case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f:
    instructionPush(toRegister3<uint16>(data));
    return step(3);
  case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
    instructionLoad(toRegister3<uint16>(data), fetchImmediate<uint16>());
    return step(3);
  case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f:
    instructionPush(toRegister3<uint32>(data));
    return step(5);
  case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    instructionLoad(toRegister3<uint32>(data), fetchImmediate<uint32>());
    return step(5);
  case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
    instructionPop(toRegister3<uint16>(data));
    return step(4);
  case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    return undefined();
  case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
    instructionPop(toRegister3<uint32>(data));
    return step(6);
  case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
  case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f: {
    auto immediate = fetchImmediate<int8>();
    if(!condition((uint4)data)) return step(2);
    instructionJumpRelative(immediate);
    return step(5); }
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
  case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f: {
    auto immediate = fetchImmediate<int16>();
    if(!condition((uint4)data)) return step(2);
    instructionJumpRelative(immediate);
    return step(5); }
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    return instructionSourceMemory(toMemory<uint8>(load(toRegister3<uint32>(data))));
  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    step(1);
    return instructionSourceMemory(toMemory<uint8>(load(toRegister3<uint32>(data)) + fetch<int8>()));
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    return instructionSourceMemory(toMemory<uint16>(load(toRegister3<uint32>(data))));
  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    step(1);
    return instructionSourceMemory(toMemory<uint16>(load(toRegister3<uint32>(data)) + fetch<int8>()));
  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    return instructionSourceMemory(toMemory<uint32>(load(toRegister3<uint32>(data))));
  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    step(1);
    return instructionSourceMemory(toMemory<uint32>(load(toRegister3<uint32>(data)) + fetch<int8>()));
  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    return instructionTargetMemory(load(toRegister3<uint32>(data)));
  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    step(1);
    return instructionTargetMemory(load(toRegister3<uint32>(data)) + fetch<int8>());
  case 0xc0:
    step(1);
    return instructionSourceMemory(fetchMemory<uint8,  uint8>());
  case 0xc1:
    step(2);
    return instructionSourceMemory(fetchMemory<uint8, uint16>());
  case 0xc2:
    step(3);
    return instructionSourceMemory(fetchMemory<uint8, uint24>());
  case 0xc3: {
    data = fetch();
    if((data & 3) == 0) {
      step(1);
      return instructionSourceMemory(toMemory<uint8>(load(toRegister8<uint32>(data))));
    }
    if((data & 3) == 1) {
      step(3);
      return instructionSourceMemory(toMemory<uint8>(load(toRegister8<uint32>(data)) + fetch<int16>()));
    }
    if(data == 0x03) {
      auto r32 = load(fetchRegister<uint32>());
      auto  r8 = load(fetchRegister< uint8>());
      step(3);
      return instructionSourceMemory(Memory<uint8>{r32 + (int8)r8});
    }
    if(data == 0x07) {
      auto r32 = load(fetchRegister<uint32>());
      auto r16 = load(fetchRegister<uint16>());
      step(3);
      return instructionSourceMemory(Memory<uint8>{r32 + (int16)r16});
    }
    return undefined(); }
  case 0xc4: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    auto location = load(register);
    if((data & 3) == 0) store(register, location -= 1);
    if((data & 3) == 1) store(register, location -= 2);
    if((data & 3) == 2) store(register, location -= 4);
    if((data & 3) == 3) Undefined;
    step(1);
    return instructionSourceMemory(toMemory<uint8>(location)); }
  case 0xc5: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    auto location = load(register);
    if((data & 3) == 0) store(register, location + 1);
    if((data & 3) == 1) store(register, location + 2);
    if((data & 3) == 2) store(register, location + 4);
    if((data & 3) == 3) Undefined;
    step(1);
    return instructionSourceMemory(toMemory<uint8>(location)); }
  case 0xc6:
    return undefined();
  case 0xc7:
    return instructionRegister(fetchRegister<uint8>());
  case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
    return instructionRegister(toRegister3<uint8>(data));
  case 0xd0:
    step(1);
    return instructionSourceMemory(fetchMemory<uint16,  uint8>());
  case 0xd1:
    step(2);
    return instructionSourceMemory(fetchMemory<uint16, uint16>());
  case 0xd2:
    step(3);
    return instructionSourceMemory(fetchMemory<uint16, uint24>());
  case 0xd3: {
    data = fetch();
    if((data & 3) == 0) {
      step(1);
      return instructionSourceMemory(toMemory<uint16>(load(toRegister8<uint32>(data))));
    }
    if((data & 3) == 1) {
      step(3);
      return instructionSourceMemory(toMemory<uint16>(load(toRegister8<uint32>(data)) + fetch<int16>()));
    }
    if(data == 0x03) {
      auto r32 = load(fetchRegister<uint32>());
      auto  r8 = load(fetchRegister< uint8>());
      step(3);
      return instructionSourceMemory(toMemory<uint16>(r32 + (int8)r8));
    }
    if(data == 0x07) {
      auto r32 = load(fetchRegister<uint32>());
      auto r16 = load(fetchRegister<uint16>());
      step(3);
      return instructionSourceMemory(toMemory<uint16>(r32 + (int16)r16));
    }
    return undefined(); }
  case 0xd4: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    auto location = load(register);
    if((data & 3) == 0) store(register, location -= 1);
    if((data & 3) == 1) store(register, location -= 2);
    if((data & 3) == 2) store(register, location -= 4);
    if((data & 3) == 3) Undefined;
    step(1);
    return instructionSourceMemory(toMemory<uint16>(location)); }
  case 0xd5: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    auto location = load(register);
    if((data & 3) == 0) store(register, location + 1);
    if((data & 3) == 1) store(register, location + 2);
    if((data & 3) == 2) store(register, location + 4);
    if((data & 3) == 3) Undefined;
    step(1);
    return instructionSourceMemory(toMemory<uint16>(location)); }
  case 0xd6:
    return undefined();
  case 0xd7:
    return instructionRegister(fetchRegister<uint16>());
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    return instructionRegister(toRegister3<uint16>(data));
  case 0xe0:
    step(1);
    return instructionSourceMemory(fetchMemory<uint32,  uint8>());
  case 0xe1:
    step(2);
    return instructionSourceMemory(fetchMemory<uint32, uint16>());
  case 0xe2:
    step(3);
    return instructionSourceMemory(fetchMemory<uint32, uint24>());
  case 0xe3: {
    data = fetch();
    if((data & 3) == 0) {
      step(1);
      return instructionSourceMemory(toMemory<uint32>(load(toRegister8<uint32>(data))));
    }
    if((data & 3) == 1) {
      step(3);
      return instructionSourceMemory(toMemory<uint32>(load(toRegister8<uint32>(data)) + fetch<int16>()));
    }
    if(data == 0x03) {
      auto r32 = load(fetchRegister<uint32>());
      auto  r8 = load(fetchRegister< uint8>());
      step(3);
      return instructionSourceMemory(toMemory<uint32>(r32 + (int8)r8));
    }
    if(data == 0x07) {
      auto r32 = load(fetchRegister<uint32>());
      auto r16 = load(fetchRegister<uint16>());
      step(3);
      return instructionSourceMemory(toMemory<uint32>(r32 + (int16)r16));
    }
    return undefined(); }
  case 0xe4: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    auto location = load(register);
    if((data & 3) == 0) store(register, location -= 1);
    if((data & 3) == 1) store(register, location -= 2);
    if((data & 3) == 2) store(register, location -= 4);
    if((data & 3) == 3) Undefined;
    step(1);
    return instructionSourceMemory(toMemory<uint32>(location)); }
  case 0xe5: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    auto location = load(register);
    if((data & 3) == 0) store(register, location + 1);
    if((data & 3) == 1) store(register, location + 2);
    if((data & 3) == 2) store(register, location + 4);
    if((data & 3) == 3) Undefined;
    step(1);
    return instructionSourceMemory(toMemory<uint32>(location)); }
  case 0xe6:
    return undefined();
  case 0xe7:
    return instructionRegister(fetchRegister<uint32>());
  case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
    return instructionRegister(toRegister3<uint32>(data));
  case 0xf0:
    step(1);
    return instructionTargetMemory(fetch< uint8>());
  case 0xf1:
    step(2);
    return instructionTargetMemory(fetch<uint16>());
  case 0xf2:
    step(3);
    return instructionTargetMemory(fetch<uint24>());
  case 0xf3: {
    data = fetch();
    if((data & 3) == 0) {
      step(1);
      return instructionTargetMemory(load(toRegister8<uint32>(data)));
    }
    if((data & 3) == 1) {
      step(3);
      return instructionTargetMemory(load(toRegister8<uint32>(data)) + fetch<int16>());
    }
    if(data == 0x03) {
      auto r32 = load(fetchRegister<uint32>());
      auto  r8 = load(fetchRegister< uint8>());
      step(3);
      return instructionTargetMemory(r32 + (int8)r8);
    }
    if(data == 0x07) {
      auto r32 = load(fetchRegister<uint32>());
      auto r16 = load(fetchRegister<uint16>());
      step(3);
      return instructionTargetMemory(r32 + (int16)r16);
    }
    return undefined(); }
  case 0xf4: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    auto location = load(register);
    if((data & 3) == 0) store(register, location -= 1);
    if((data & 3) == 1) store(register, location -= 2);
    if((data & 3) == 2) store(register, location -= 4);
    if((data & 3) == 3) Undefined;
    step(1);
    return instructionTargetMemory(location); }
  case 0xf5: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    auto location = load(register);
    if((data & 3) == 0) store(register, location + 1);
    if((data & 3) == 1) store(register, location + 2);
    if((data & 3) == 2) store(register, location + 4);
    if((data & 3) == 3) Undefined;
    step(1);
    return instructionTargetMemory(location); }
  case 0xf6:
    return undefined();
  case 0xf7: {
    if(fetch()) Undefined;
    auto memory = fetchMemory<uint8, uint8>();
    if(fetch()) Undefined;
    auto immediate = fetchImmediate<uint8>();
    if(fetch()) Undefined;
    instructionLoad(memory, immediate);
    return step(8); }
  case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff:
    instructionSoftwareInterrupt((uint3)data);
    return step(1);
  }
}

template<typename R>
auto TLCS900H::instructionRegister(R register) -> void {
  using T = typename R::type;
  enum : uint { bits = R::bits };
  auto data = fetch();

  switch(data) {
  case 0x00: case 0x01: case 0x02:
    return undefined();
  case 0x03:
    instructionLoad(register, fetchImmediate<T>());
    return stepBWL<bits>(3, 4, 6);
  case 0x04:
    instructionPush(register);
    return stepBWL<bits>(4, 4, 6);
  case 0x05:
    instructionPop(register);
    return stepBWL<bits>(5, 5, 7);
  case 0x06:
    if constexpr(bits == 32) return undefined(); else {
    instructionComplement(register);
    return step(2); }
  case 0x07:
    if constexpr(bits == 32) return undefined(); else {
    instructionNegate(register);
    return step(2); }
  case 0x08:
    if constexpr(bits == 32) return undefined(); else {
    instructionMultiply(register, fetchImmediate<T>());
    return stepBW<bits>(12, 15); }
  case 0x09:
    if constexpr(bits == 32) return undefined(); else {
    instructionMultiplySigned(register, fetchImmediate<T>());
    return stepBW<bits>(10, 13); }
  case 0x0a:
    if constexpr(bits == 32) return undefined(); else {
    instructionDivide(register, fetchImmediate<T>());
    return stepBW<bits>(15, 23); }
  case 0x0b:
    if constexpr(bits == 32) return undefined(); else {
    instructionDivideSigned(register, fetchImmediate<T>());
    return stepBW<bits>(18, 26); }
  case 0x0c:
    if constexpr(bits != 32) return undefined(); else {
    instructionLink(register, fetchImmediate<int16>());
    return step(8); }
  case 0x0d:
    if constexpr(bits != 32) return undefined(); else {
    instructionUnlink(register);
    return step(7); }
  case 0x0e:
    if constexpr(bits != 16) return undefined(); else {
    instructionBitSearch1Forward(register);
    return step(3); }
  case 0x0f:
    if constexpr(bits != 16) return undefined(); else {
    instructionBitSearch1Backward(register);
    return step(3); }
  case 0x10:
    if constexpr(bits != 8) return undefined(); else {
    instructionDecimalAdjustAccumulator(register);
    return step(4); }
  case 0x11:
    return undefined();
  case 0x12:
    if constexpr(bits == 8) return undefined(); else {
    instructionExtendZero(register);
    return step(3); }
  case 0x13:
    if constexpr(bits == 8) return undefined(); else {
    instructionExtendSign(register);
    return step(3); }
  case 0x14:
    if constexpr(bits == 8) return undefined(); else {
    instructionPointerAdjustAccumulator(register);
    return step(4); }
  case 0x15:
    return undefined();
  case 0x16:
    if constexpr(bits != 16) return undefined(); else {
    instructionMirror(register);
    return step(3); }
  case 0x17: case 0x18:
    return undefined();
  case 0x19:
    if constexpr(bits != 16) return undefined(); else {
    instructionMultiplyAdd(register);
    return step(19); }
  case 0x1a: case 0x1b:
    return undefined();
  case 0x1c:
    if constexpr(bits == 32) return undefined(); else {
    bool jump = instructionDecrementJumpNotZero(register, fetchImmediate<int8>());
    return step(!jump ? 4 : 6); }
  case 0x1d: case 0x1e: case 0x1f:
    return undefined();
  case 0x20:
    if constexpr(bits == 32) return undefined(); else {
    instructionAndCarry(register, fetchImmediate<uint8>());
    return step(3); }
  case 0x21:
    if constexpr(bits == 32) return undefined(); else {
    instructionOrCarry(register, fetchImmediate<uint8>());
    return step(3); }
  case 0x22:
    if constexpr(bits == 32) return undefined(); else {
    instructionXorCarry(register, fetchImmediate<uint8>());
    return step(3); }
  case 0x23:
    if constexpr(bits == 32) return undefined(); else {
    instructionLoadCarry(register, fetchImmediate<uint8>());
    return step(3); }
  case 0x24:
    if constexpr(bits == 32) return undefined(); else {
    instructionStoreCarry(register, fetchImmediate<uint8>());
    return step(3); }
  case 0x25: case 0x26: case 0x27:
    return undefined();
  case 0x28:
    if constexpr(bits == 32) return undefined(); else {
    instructionAndCarry(register, A);
    return step(3); }
  case 0x29:
    if constexpr(bits == 32) return undefined(); else {
    instructionOrCarry(register, A);
    return step(3); }
  case 0x2a:
    if constexpr(bits == 32) return undefined(); else {
    instructionXorCarry(register, A);
    return step(3); }
  case 0x2b:
    if constexpr(bits == 32) return undefined(); else {
    instructionLoadCarry(register, A);
    return step(3); }
  case 0x2c:
    if constexpr(bits == 32) return undefined(); else {
    instructionStoreCarry(register, A);
    return step(3); }
  case 0x2d:
    return undefined();
  case 0x2e:
    data = fetch();
    instructionLoad(toControlRegister<T>(data), register);
    return step(3);
  case 0x2f:
    data = fetch();
    instructionLoad(register, toControlRegister<T>(data));
    return step(3);
  case 0x30:
    if constexpr(bits == 32) return undefined(); else {
    instructionReset(register, fetchImmediate<uint8>());
    return step(3); }
  case 0x31:
    if constexpr(bits == 32) return undefined(); else {
    instructionSet(register, fetchImmediate<uint8>());
    return step(3); }
  case 0x32:
    if constexpr(bits == 32) return undefined(); else {
    instructionChange(register, fetchImmediate<uint8>());
    return step(3); }
  case 0x33:
    if constexpr(bits == 32) return undefined(); else {
    instructionBit(register, fetchImmediate<uint8>());
    return step(3); }
  case 0x34:
    if constexpr(bits == 32) return undefined(); else {
    instructionTestSet(register, fetchImmediate<uint8>());
    return step(4); }
  case 0x35: case 0x36: case 0x37:
    return undefined();
  case 0x38:
    if constexpr(bits != 16) return undefined(); else {
    instructionModuloIncrement<1>(register, fetchImmediate<uint16>());
    return step(5); }
  case 0x39:
    if constexpr(bits != 16) return undefined(); else {
    instructionModuloIncrement<2>(register, fetchImmediate<uint16>());
    return step(5); }
  case 0x3a:
    if constexpr(bits != 16) return undefined(); else {
    instructionModuloIncrement<4>(register, fetchImmediate<uint16>());
    return step(5); }
  case 0x3b:
    return undefined();
  case 0x3c:
    if constexpr(bits != 16) return undefined(); else {
    instructionModuloDecrement<1>(register, fetchImmediate<uint16>());
    return step(4); }
  case 0x3d:
    if constexpr(bits != 16) return undefined(); else {
    instructionModuloDecrement<2>(register, fetchImmediate<uint16>());
    return step(4); }
  case 0x3e:
    if constexpr(bits != 16) return undefined(); else {
    instructionModuloDecrement<4>(register, fetchImmediate<uint16>());
    return step(4); }
  case 0x3f:
    return undefined();
  case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    if constexpr(bits == 32) return undefined(); else {
    instructionMultiply(toRegister3<T>(data), register);
    return stepBW<bits>(11, 14); }
  case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
    if constexpr(bits == 32) return undefined(); else {
    instructionMultiplySigned(toRegister3<T>(data), register);
    return stepBW<bits>(9, 12); }
  case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    if constexpr(bits == 32) return undefined(); else {
    instructionDivide(toRegister3<T>(data), register);
    return stepBW<bits>(15, 23); }
  case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
    if constexpr(bits == 32) return undefined(); else {
    instructionDivideSigned(toRegister3<T>(data), register);
    return stepBW<bits>(18, 26); }
  case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
    instructionIncrement(register, toImmediate<T>((uint3)data));
    return step(3);
  case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f:
    instructionDecrement(register, toImmediate<T>((uint3)data));
    return step(3);
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
  case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f:
    if constexpr(bits == 32) return undefined(); else {
    instructionSetConditionCode((uint4)data, register);
    return step(2); }
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    instructionAdd(toRegister3<T>(data), register);
    return step(2);
  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    instructionLoad(toRegister3<T>(data), register);
    return step(2);
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    instructionAddCarry(toRegister3<T>(data), register);
    return step(2);
  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    instructionLoad(register, toRegister3<T>(data));
    return step(2);
  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    instructionSubtract(toRegister3<T>(data), register);
    return step(2);
  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    instructionLoad(register, toImmediate<T>((uint3)data));
    return step(2);
  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    instructionSubtractBorrow(toRegister3<T>(data), register);
    return step(2);
  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    if constexpr(bits == 32) return undefined(); else {
    instructionExchange(toRegister3<T>(data), register);
    return step(3); }
  case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
    instructionAnd(toRegister3<T>(data), register);
    return step(2);
  case 0xc8:
    instructionAdd(register, fetchImmediate<T>());
    return stepBWL<bits>(3, 4, 6);
  case 0xc9:
    instructionAddCarry(register, fetchImmediate<T>());
    return stepBWL<bits>(3, 4, 6);
  case 0xca:
    instructionSubtract(register, fetchImmediate<T>());
    return stepBWL<bits>(3, 4, 6);
  case 0xcb:
    instructionSubtractBorrow(register, fetchImmediate<T>());
    return stepBWL<bits>(3, 4, 6);
  case 0xcc:
    instructionAnd(register, fetchImmediate<T>());
    return stepBWL<bits>(3, 4, 6);
  case 0xcd:
    instructionXor(register, fetchImmediate<T>());
    return stepBWL<bits>(3, 4, 6);
  case 0xce:
    instructionOr(register, fetchImmediate<T>());
    return stepBWL<bits>(3, 4, 6);
  case 0xcf:
    instructionCompare(register, fetchImmediate<T>());
    return stepBWL<bits>(3, 4, 6);
  case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
    instructionXor(toRegister3<T>(data), register);
    return step(2);
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    if constexpr(bits == 32) return undefined(); else {
    instructionCompare(register, toImmediate<T>((uint3)data));
    return step(2); }
  case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
    instructionOr(toRegister3<T>(data), register);
    return step(2);
  case 0xe8:
    instructionRotateLeftWithoutCarry(register, fetchImmediate<uint8>());
    return step(3);
  case 0xe9:
    instructionRotateRightWithoutCarry(register, fetchImmediate<uint8>());
    return step(3);
  case 0xea:
    instructionRotateLeft(register, fetchImmediate<uint8>());
    return step(3);
  case 0xeb:
    instructionRotateRight(register, fetchImmediate<uint8>());
    return step(3);
  case 0xec:
    instructionShiftLeftArithmetic(register, fetchImmediate<uint8>());
    return step(3);
  case 0xed:
    instructionShiftRightArithmetic(register, fetchImmediate<uint8>());
    return step(3);
  case 0xee:
    instructionShiftLeftLogical(register, fetchImmediate<uint8>());
    return step(3);
  case 0xef:
    instructionShiftRightLogical(register, fetchImmediate<uint8>());
    return step(3);
  case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7:
    instructionCompare(toRegister3<T>(data), register);
    return step(2);
  case 0xf8:
    instructionRotateLeftWithoutCarry(register, A);
    return step(3);
  case 0xf9:
    instructionRotateRightWithoutCarry(register, A);
    return step(3);
  case 0xfa:
    instructionRotateLeft(register, A);
    return step(3);
  case 0xfb:
    instructionRotateRight(register, A);
    return step(3);
  case 0xfc:
    instructionShiftLeftArithmetic(register, A);
    return step(3);
  case 0xfd:
    instructionShiftRightArithmetic(register, A);
    return step(3);
  case 0xfe:
    instructionShiftLeftLogical(register, A);
    return step(3);
  case 0xff:
    instructionShiftRightLogical(register, A);
    return step(3);
  }
}

template<typename M>
auto TLCS900H::instructionSourceMemory(M memory) -> void {
  using T = typename M::type;
  enum : uint { bits = M::bits };
  auto data = fetch();

  switch(data) {
  case 0x00: case 0x01: case 0x02: case 0x03:
    return undefined();
  case 0x04:
    if constexpr(bits == 32) return undefined(); else {
    instructionPush(memory);
    return step(6); }
  case 0x05:
    return undefined();
  case 0x06:
    if constexpr(bits != 8) return undefined(); else {
    instructionRotateLeftDigit(A, memory);
    return step(14); }
  case 0x07:
    if constexpr(bits != 8) return undefined(); else {
    instructionRotateRightDigit(A, memory);
    return step(14); }
  case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f:
    return undefined();
  case 0x10:
    if constexpr(bits ==  8) { instructionLoad<T, +1>(); return step(1); }
    if constexpr(bits == 16) { instructionLoad<T, +2>(); return step(1); }
    return undefined();
  case 0x11:
    if constexpr(bits ==  8) { instructionLoadRepeat<T, +1>(); return step(1); }
    if constexpr(bits == 16) { instructionLoadRepeat<T, +2>(); return step(1); }
    return undefined();
  case 0x12:
    if constexpr(bits ==  8) { instructionLoad<T, -1>(); return step(1); }
    if constexpr(bits == 16) { instructionLoad<T, -2>(); return step(1); }
    return undefined();
  case 0x13:
    if constexpr(bits ==  8) { instructionLoadRepeat<T, -1>(); return step(1); }
    if constexpr(bits == 16) { instructionLoadRepeat<T, -2>(); return step(1); }
    return undefined();
  case 0x14:
    if constexpr(bits ==  8) { instructionCompare<T, +1>( A); return; }
    if constexpr(bits == 16) { instructionCompare<T, +2>(WA); return; }
    return undefined();
  case 0x15:
    if constexpr(bits ==  8) { instructionCompareRepeat<T, +1>( A); return step(1); }
    if constexpr(bits == 16) { instructionCompareRepeat<T, +2>(WA); return step(1); }
    return undefined();
  case 0x16:
    if constexpr(bits ==  8) { instructionCompare<T, -1>( A); return; }
    if constexpr(bits == 16) { instructionCompare<T, -2>(WA); return; }
    return undefined();
  case 0x17:
    if constexpr(bits ==  8) { instructionCompareRepeat<T, -1>( A); return step(1); }
    if constexpr(bits == 16) { instructionCompareRepeat<T, -2>(WA); return step(1); }
    return undefined();
  case 0x18:
    return undefined();
  case 0x19:
    if constexpr(bits == 32) return undefined(); else {
    instructionLoad(fetchMemory<T, uint16>(), memory);
    return step(8); }
  case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f:
    return undefined();
  case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
    instructionLoad(toRegister3<T>(data), memory);
    return stepBWL<bits>(4, 4, 6);
  case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f:
    return undefined();
  case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
    if constexpr(bits == 32) return undefined(); else {
    instructionExchange(memory, toRegister3<T>(data));
    return step(6); }
  case 0x38:
    if constexpr(bits == 32) return undefined(); else {
    instructionAdd(memory, fetchImmediate<T>());
    return stepBW<bits>(7, 8); }
  case 0x39:
    if constexpr(bits == 32) return undefined(); else {
    instructionAddCarry(memory, fetchImmediate<T>());
    return stepBW<bits>(7, 8); }
  case 0x3a:
    if constexpr(bits == 32) return undefined(); else {
    instructionSubtract(memory, fetchImmediate<T>());
    return stepBW<bits>(7, 8); }
  case 0x3b:
    if constexpr(bits == 32) return undefined(); else {
    instructionSubtractBorrow(memory, fetchImmediate<T>());
    return stepBW<bits>(7, 8); }
  case 0x3c:
    if constexpr(bits == 32) return undefined(); else {
    instructionAnd(memory, fetchImmediate<T>());
    return stepBW<bits>(7, 8); }
  case 0x3d:
    if constexpr(bits == 32) return undefined(); else {
    instructionXor(memory, fetchImmediate<T>());
    return stepBW<bits>(7, 8); }
  case 0x3e:
    if constexpr(bits == 32) return undefined(); else {
    instructionOr(memory, fetchImmediate<T>());
    return stepBW<bits>(7, 8); }
  case 0x3f:
    if constexpr(bits == 32) return undefined(); else {
    instructionCompare(memory, fetchImmediate<T>());
    return stepBW<bits>(5, 6); }
  case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    if constexpr(bits == 32) return undefined(); else {
    instructionMultiply(toRegister3<T>(data), memory);
    return stepBW<bits>(13, 16); }
  case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
    if constexpr(bits == 32) return undefined(); else {
    instructionMultiplySigned(toRegister3<T>(data), memory);
    return stepBW<bits>(11, 14); }
  case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    if constexpr(bits == 32) return undefined(); else {
    instructionDivide(toRegister3<T>(data), memory);
    return stepBW<bits>(16, 24); }
  case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
    if constexpr(bits == 32) return undefined(); else {
    instructionDivideSigned(toRegister3<T>(data), memory);
    return stepBW<bits>(19, 27); }
  case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
    if constexpr(bits == 32) return undefined(); else {
    instructionIncrement(memory, toImmediate<T>((uint3)data));
    return step(6); }
  case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f:
    if constexpr(bits == 32) return undefined(); else {
    instructionDecrement(memory, toImmediate<T>((uint3)data));
    return step(6); }
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
    return undefined();
  case 0x78:
    if constexpr(bits == 32) return undefined(); else {
    instructionRotateLeftWithoutCarry(memory, toImmediate<uint4>(1));
    return step(6); }
  case 0x79:
    if constexpr(bits == 32) return undefined(); else {
    instructionRotateRightWithoutCarry(memory, toImmediate<uint4>(1));
    return step(6); }
  case 0x7a:
    if constexpr(bits == 32) return undefined(); else {
    instructionRotateLeft(memory, toImmediate<uint4>(1));
    return step(6); }
  case 0x7b:
    if constexpr(bits == 32) return undefined(); else {
    instructionRotateRight(memory, toImmediate<uint4>(1));
    return step(6); }
  case 0x7c:
    if constexpr(bits == 32) return undefined(); else {
    instructionShiftLeftArithmetic(memory, toImmediate<uint4>(1));
    return step(6); }
  case 0x7d:
    if constexpr(bits == 32) return undefined(); else {
    instructionShiftRightArithmetic(memory, toImmediate<uint4>(1));
    return step(6); }
  case 0x7e:
    if constexpr(bits == 32) return undefined(); else {
    instructionShiftLeftLogical(memory, toImmediate<uint4>(1));
    return step(6); }
  case 0x7f:
    if constexpr(bits == 32) return undefined(); else {
    instructionShiftRightLogical(memory, toImmediate<uint4>(1));
    return step(6); }
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    instructionAdd(toRegister3<T>(data), memory);
    return stepBWL<bits>(4, 4, 6);
  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    instructionAdd(memory, toRegister3<T>(data));
    return stepBWL<bits>(6, 6, 10);
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    instructionAddCarry(toRegister3<T>(data), memory);
    return stepBWL<bits>(4, 4, 6);
  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    instructionAddCarry(memory, toRegister3<T>(data));
    return stepBWL<bits>(6, 6, 10);
  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    instructionSubtract(toRegister3<T>(data), memory);
    return stepBWL<bits>(4, 4, 6);
  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    instructionSubtract(memory, toRegister3<T>(data));
    return stepBWL<bits>(6, 6, 10);
  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    instructionSubtractBorrow(toRegister3<T>(data), memory);
    return stepBWL<bits>(4, 4, 6);
  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    instructionSubtractBorrow(memory, toRegister3<T>(data));
    return stepBWL<bits>(6, 6, 10);
  case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
    instructionAnd(toRegister3<T>(data), memory);
    return stepBWL<bits>(4, 4, 6);
  case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
    instructionAnd(memory, toRegister3<T>(data));
    return stepBWL<bits>(6, 6, 10);
  case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
    instructionXor(toRegister3<T>(data), memory);
    return stepBWL<bits>(4, 4, 6);
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    instructionXor(memory, toRegister3<T>(data));
    return stepBWL<bits>(6, 6, 10);
  case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
    instructionOr(toRegister3<T>(data), memory);
    return stepBWL<bits>(4, 4, 6);
  case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
    instructionOr(memory, toRegister3<T>(data));
    return stepBWL<bits>(6, 6, 10);
  case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7:
    instructionCompare(toRegister3<T>(data), memory);
    return stepBWL<bits>(4, 4, 6);
  case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff:
    instructionCompare(memory, toRegister3<T>(data));
    return stepBWL<bits>(4, 4, 6);
  }
}

auto TLCS900H::instructionTargetMemory(uint32 address) -> void {
  auto data = fetch();

  switch(data) {
  case 0x00:
    instructionLoad(toMemory<uint8>(address), fetchImmediate<uint8>());
    return step(5);
  case 0x01:
    return undefined();
  case 0x02:
    instructionLoad(toMemory<uint16>(address), fetchImmediate<uint16>());
    return step(6);
  case 0x03:
    return undefined();
  case 0x04:
    instructionPop(toMemory<uint8>(address));
    return step(7);
  case 0x05:
    return undefined();
  case 0x06:
    instructionPop(toMemory<uint16>(address));
    return step(7);
  case 0x07:
    return undefined();
  case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f:
    return undefined();
  case 0x10: case 0x11: case 0x12: case 0x13:
    return undefined();
  case 0x14:
    instructionLoad(toMemory<uint8>(address), fetchMemory<uint8, uint16>());
    return step(8);
  case 0x15:
    return undefined();
  case 0x16:
    instructionLoad(toMemory<uint16>(address), fetchMemory<uint16, uint16>());
    return step(8);
  case 0x17:
    return undefined();
  case 0x18: case 0x19: case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f:
    return undefined();
  case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
    instructionLoad(toRegister3<uint16>(data), toImmediate<uint16>(address));
    return step(4);
  case 0x28:
    instructionAndCarry(toMemory<uint8>(address), A);
    return step(6);
  case 0x29:
    instructionOrCarry(toMemory<uint8>(address), A);
    return step(6);
  case 0x2a:
    instructionXorCarry(toMemory<uint8>(address), A);
    return step(6);
  case 0x2b:
    instructionLoadCarry(toMemory<uint8>(address), A);
    return step(6);
  case 0x2c:
    instructionStoreCarry(toMemory<uint8>(address), A);
    return step(7);
  case 0x2d: case 0x2e: case 0x2f:
    return undefined();
  case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
    instructionLoad(toRegister3<uint32>(data), toImmediate<uint32>(address));
    return step(4);
  case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f:
    return undefined();
  case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    instructionLoad(toMemory<uint8>(address), toRegister3<uint8>(data));
    return step(4);
  case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
    return undefined();
  case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    instructionLoad(toMemory<uint16>(address), toRegister3<uint16>(data));
    return step(4);
  case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
    return undefined();
  case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
    instructionLoad(toMemory<uint32>(address), toRegister3<uint32>(data));
    return step(6);
  case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f:
    return undefined();
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
    return undefined();
  case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f:
    return undefined();
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    instructionAndCarry(toMemory<uint8>(address), toImmediate<uint3>(data));
    return step(6);
  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    instructionOrCarry(toMemory<uint8>(address), toImmediate<uint3>(data));
    return step(6);
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    instructionXorCarry(toMemory<uint8>(address), toImmediate<uint3>(data));
    return step(6);
  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    instructionLoadCarry(toMemory<uint8>(address), toImmediate<uint3>(data));
    return step(6);
  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    instructionStoreCarry(toMemory<uint8>(address), toImmediate<uint3>(data));
    return step(7);
  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    instructionTestSet(toMemory<uint8>(address), toImmediate<uint3>(data));
    return step(7);
  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    instructionReset(toMemory<uint8>(address), toImmediate<uint3>(data));
    return step(7);
  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    instructionSet(toMemory<uint8>(address), toImmediate<uint3>(data));
    return step(7);
  case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
    instructionChange(toMemory<uint8>(address), toImmediate<uint3>(data));
    return step(7);
  case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
    instructionBit(toMemory<uint8>(address), toImmediate<uint3>(data));
    return step(6);
  case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    if(!condition((uint4)data)) return step(4);
    instructionJump(toImmediate<uint32>(address));
    return step(7);
  case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
  case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
    if(!condition((uint4)data)) return step(4);
    return instructionCall(toImmediate<uint32>(address));
    return step(12);
  case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7:
  case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff:
    if(!condition((uint4)data)) return step(4);
    instructionReturn();
    return step(12);
  }
}
