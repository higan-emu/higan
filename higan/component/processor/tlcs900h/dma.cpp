auto TLCS900H::dma(uint2 channel) -> bool {
  auto& source = r.dmas[channel].l.l0;
  auto& target = r.dmad[channel].l.l0;
  auto& length = r.dmam[channel].w.w0;  //0 = 65536
  auto& config = r.dmam[channel].w.w1;

  uint32 mode = config.bits(2,4);
  uint32 size;
  switch(config.bits(0,1)) {
  case 0: size = 1; break;
  case 1: size = 2; break;
  case 2: size = 4; break;
  case 3: size = 4; break;  //unknown behavior
  }

  uint32 data;
  if(mode <= 4)
  switch(size) {
  case 1:
    data.byte(0) = read(source + 0);
    break;
  case 2:
    data.byte(0) = read(source + 0);
    data.byte(1) = read(source + 1);
    break;
  case 4:
    data.byte(0) = read(source + 0);
    data.byte(1) = read(source + 1);
    data.byte(2) = read(source + 2);
    data.byte(3) = read(source + 3);
    break;
  }

  if(mode <= 4)
  switch(size) {
  case 1:
    write(target + 0, data.byte(0));
    break;
  case 2:
    write(target + 0, data.byte(0));
    write(target + 1, data.byte(1));
    break;
  case 4:
    write(target + 0, data.byte(0));
    write(target + 1, data.byte(1));
    write(target + 2, data.byte(2));
    write(target + 3, data.byte(3));
    break;
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

  if(mode <= 4) step(size == 4 ? 12 : 8);
  if(mode == 5) step(5);
  if(mode >= 6) step(5);  //unknown behavior

  return --length == 0;  //true indicates the transfer has completed
}
