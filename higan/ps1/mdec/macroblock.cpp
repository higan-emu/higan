auto MDEC::decodeMacroBlock(const vector<uint>& mb_ids) -> bool {
  for (auto& id : mb_ids) {
    if(io.currentBlock == id) {
      if(!decodeRLE(macroBlock[id], quantTable[id / 4])) return false;  //queue is empty
      IDCT(ycbcrBlock[id], macroBlock[id]);
      io.currentBlock = (io.currentBlock + 1) % mb_ids.size();
    }
  }

  auto& fifo = this->fifo.mdecOut;
  queue<u8> rgbBlock;
  switch(io.dataOutputDepth) {
  case 0: //4bit
  case 1: //8bit
    YtoGrayscale(rgbBlock, ycbcrBlock[0]);
    fifo.append(rgbBlock);
    break;
  case 2:   //24bit
  case 3:   //15bit
    uint id = 0;
    for(uint y : vector<uint>{0,8}) {
      for(uint x : vector<uint>{0,8}) {
        YUVtoRGB(rgbBlock, ycbcrBlock[id++], ycbcrBlock[4], ycbcrBlock[5], x, y);
        fifo.append(rgbBlock);
      }
    }
    break;
  }

  return true;
}
