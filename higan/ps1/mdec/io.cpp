auto MDEC::readByte(u32 address) -> u8 {
  print("* read byte ", hex(address, 8L), "\n");
  return 0;
}

auto MDEC::readHalf(u32 address) -> u16 {
  print("* read half ", hex(address, 8L), "\n");
  return 0;
}

auto MDEC::readWord(u32 address) -> u32 {
  uint32 data;

  if(address == 0x1f80'1820) {
    auto& fifo = this->fifo.mdecOut;
    if(!fifo) return 0;

    data = 0;
    switch(io.dataOutputDepth) {
    case 0: //4bit
      for(uint byte : range(4)) {
        u8 lower = fifo[0].read() >> 4;
        u8 upper = fifo[0].read() >> 4;
        data |= ((lower << 0) | (upper << 4)) << byte*8;
      }
      break;
    case 1: //8bit
    case 2: //24bit
      for(uint byte : range(4)) {
        data |= fifo[0].read() << byte*8;
      }
      break;
    case 3: //15bit
      for(uint half : range(2)) {
        u8 r = fifo[0].read() >> 3;
        u8 g = fifo[0].read() >> 3;
        u8 b = fifo[0].read() >> 3;
        u8 a = io.dataOutputBit15;
        data |= ((r << 0) | (g << 5) | (b << 10) | (a << 15)) << half*16;
      }
      break;
    }
    if(fifo[0].empty()) fifo.remove(0);
    return data;
  }

  if(address == 0x1f80'1824) {
    data.bit( 0,15) = (io.numOfParamWords - 1) & 0xffff;
    data.bit(16,18) = io.currentBlock;
    data.bit(19,22) = 0;
    data.bit(23)    = io.dataOutputBit15;
    data.bit(24)    = io.dataOutputSigned;
    data.bit(25,26) = io.dataOutputDepth;
    data.bit(27)    = io.enableDataOut;
    data.bit(28)    = io.enableDataIn;
    data.bit(29)    = io.command != 0x00 ? 1 : 0;
    data.bit(30)    = fifo.mdecIn.full() ? 1 : 0;
    data.bit(31)    = fifo.mdecOut.size() == 0 ? 1 : 0;
    return data;
  }

  print("* read word ", hex(address, 8L), "\n");
  return 0;
}

auto MDEC::writeByte(u32 address, u8 value) -> void {
  print("* write byte ", hex(address, 8L), " = ", hex(value, 2L), "\n");
}

auto MDEC::writeHalf(u32 address, u16 value) -> void {
  print("* write half ", hex(address, 8L), " = ", hex(value, 4L), "\n");
}

auto MDEC::writeWord(u32 address, u32 value) -> void {
  uint32 data = value;

  if(address == 0x1f80'1820) {
    uint3 command = data.bit(29,31);

    io.command = command;

    //decode macroblocks
    if(command == 0x01) {
      io.numOfParamWords  = data.bit( 0,15);
      io.dataOutputBit15  = data.bit(25);
      io.dataOutputSigned = data.bit(26);
      io.dataOutputDepth  = data.bit(27,28);

      switch(io.dataOutputDepth) {
      case 0: //4bit
      case 1: //8bit
        io.currentBlock = 0;  //Y1
        break;
      case 2: //24bit
      case 3: //15bit
        io.currentBlock = 4;  //Cr
        break;
      }
      return;
    }

    //set quant tables
    if(command == 0x02) {
      io.colorQuantTable  = data.bit(0);
      return;
    }

    //set scale tables
    if(command == 0x03) {
      return;
    }
  }

  if(address == 0x1f80'1824) {
    uint1 reset = data.bit(31);
    if(reset) {
      fifo.mdecIn.flush();
      fifo.mdecOut.reset();
      fifo.counter = 0;
      io = {};
      currentMatrixID = 0;
      currentQualityFactor = 0;
      return;
    }
    io.enableDataOut = data.bit(29);
    io.enableDataIn  = data.bit(30);
    return;
  }

  print("* write word ", hex(address, 8L), " = ", hex(value, 4L), "\n");
}

auto MDEC::readDMA() -> u32 {
  auto& fifo = this->fifo.mdecOut;
  if(!fifo) {
    print("* MDEC(fifo.Out) = empty\n");
    return 0;
  }

  //On DMA, 24bit & 15bit require reorder 8x8 to 16x16 pixel
  // 3(byte) x 8(pixel) = 24, 3(byte) x 16(pixel) = 48
  this->fifo.counter %= 48;
  uint fifoID = this->fifo.counter < 24 ? 0 : 1;

  u32 data = 0;
  switch(io.dataOutputDepth) {
  case 0: //4bit
    for(uint byte : range(4)) {
      u8 lower = fifo[0].read() >> 4;
      u8 upper = fifo[0].read() >> 4;
      data |= ((lower << 0) | (upper << 4)) << byte*8;
    }
    if(fifo[0].empty()) fifo.remove(0);
    break;
  case 1: //8bit
    for(uint byte : range(4)) {
      data |= fifo[0].read() << byte*8;
    }
    if(fifo[0].empty()) fifo.remove(0);
    break;
  case 2: //24bit
    for(uint byte : range(4)) {
      data |= fifo[fifoID].read() << byte*8;
      this->fifo.counter++;
    }
    if(fifo[1].empty()) fifo.removeLeft(2);
    break;
  case 3: //15bit
    for(uint half : range(2)) {
      u8 r = fifo[fifoID].read() >> 3;
      u8 g = fifo[fifoID].read() >> 3;
      u8 b = fifo[fifoID].read() >> 3;
      u8 a = io.dataOutputBit15;
      data |= ((r << 0) | (g << 5) | (b << 10) | (a << 15)) << half*16;
      this->fifo.counter += 3;
    }
    if(fifo[1].empty()) fifo.removeLeft(2);
    break;
  }
  return data;
}

auto MDEC::writeDMA(u32 value) -> void {
  auto& fifo = this->fifo.mdecIn;
  for([[maybe_unused]] uint half : range(2)) {
    if(fifo.full()) {
      print("* MDEC(fifo.In) = full\n");
      return;
    }
    fifo.write(value);
    value >>= 16;
  }

  //decode macroblocks
  if(io.command == 0x01) {
    if(fifo.size<u32>() < 32) return;
    auto size = fifo.size<u32>();
    switch(io.dataOutputDepth) {
    case 0:  //4bit
    case 1:  //8bit
      while (decodeMacroBlock({0})); //Y1 only
      break;
    case 2:  //24bit
    case 3:  //15bit
      while (decodeMacroBlock({4, 5, 0, 1, 2, 3}));  //Cr, Cb, Y1, Y2, Y3, Y4
      break;
    }
    auto decoded = size - fifo.size<u32>();
    io.numOfParamWords -= decoded;
    if(io.numOfParamWords == 0) io.command = 0x00;
  }

  //set quant table
  if(io.command == 0x02) {
    uint numFifo = io.colorQuantTable ? 32 : 16;
    if(fifo.size<u32>() < numFifo) return;
    for(uint YorC : range(io.colorQuantTable + 1)) {
      for(uint half : range(32)) {
        u16 data = fifo.read();
        for(uint byte : range(2)) {
          quantTable[YorC][half * 2 + byte] = data;
          data >>= 8;
        }
      }
    }
    io.command = 0x00;
  }

  //set scale table
  if(io.command == 0x03) {
    if(fifo.size<u32>() < 32) return;
    //transpose x <-> y here
    for(uint x : range(8)) {
      for(uint y : range(8)) {
        scaleTable[y][x] = fifo.read();
      }
    }
    io.command = 0x00;
  }
}
