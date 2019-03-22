auto TLCS900H::dma(uint2 channel) -> bool {
  auto& source = r.dmas[channel].l.l0;
  auto& target = r.dmad[channel].l.l0;
  auto& length = r.dmam[channel].w.w0;  //0 = 65536
  auto& config = r.dmam[channel].w.w1;

  uint32 mode = config.bits(2,4);
  uint32 size;
  switch(config.bits(0,1)) {
  case 0: size = Byte; break;
  case 1: size = Word; break;
  case 2: size = Long; break;
  case 3: size = Long; break;  //unknown behavior
  }

  if(mode <= 4) {
    auto data = read(size, source);
    write(size, target, data);
  }

  switch(mode) {
  case 0: target += size; break;
  case 1: target -= size; break;
  case 2: source += size; break;
  case 3: source -= size; break;
  case 4:                 break;
  case 5: source += size; break;
  case 6: break;  //unknown behavior
  case 7: break;  //unknown behavior
  }

  if(mode <= 4) step(size == Long ? 12 : 8);
  if(mode == 5) step(5);
  if(mode >= 6) step(5);  //unknown behavior

  return --length == 0;  //true indicates the transfer has completed
}
