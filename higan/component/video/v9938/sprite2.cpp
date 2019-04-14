auto V9938::sprite2(uint8 voffset) -> void {
  if(sprite.disable) return;

  sprite.collision = 0;
  sprite.overflow = 0;
  sprite.last = 0;

  uint4 valid = 0;
  uint4 sizeLimit = !sprite.size ? 7 : 15;
  for(uint index : range(8)) sprites[index].y = 0xd8;

  uint17 address = table.spriteAttribute & 0x1fe00;
  for(uint index : range(32)) {
    uint8 y = videoRAM.read(address++);
    if(y == 0xd8) break;

    uint8 x = videoRAM.read(address++);
    uint8 pattern = videoRAM.read(address++);
    uint8 reserved = videoRAM.read(address++);

    y += 1;
    if(voffset < y) continue;
    if(voffset > y + sizeLimit) continue;

    //16x16 sprites use four patterns; thus lower two pattern bits are ignored
    if(sprite.size) pattern.bits(0,1) = 0;

    if(valid == 8) {
      sprite.overflow = 1;
      sprite.last = index;
      break;
    }

    uint17 colorTable = table.spriteAttribute & 0x1fc00;
    colorTable += index << 4;
    colorTable += voffset - y;

    uint8 attributes = videoRAM.read(colorTable);
    uint4 color = attributes.bits(0,3);
    uint1 collision = attributes.bit(5);
    uint1 priority = attributes.bit(6);
    if(attributes.bit(7)) x -= 32;

    sprites[valid++] = {x, y, pattern, color, collision, priority};
  }
}

auto V9938::sprite2(uint4& color, uint8 hoffset, uint8 voffset) -> void {
  if(sprite.disable) return;

  uint4 output;
  uint4 sizeLimit = !sprite.size ? 7 : 15;

  for(uint index : range(8)) {
    auto& o = sprites[index];
    if(o.y == 0xd8) break;
    if(hoffset < o.x) continue;
    if(hoffset > o.x + sizeLimit) continue;

    uint4 x = hoffset - o.x;
    uint4 y = voffset - o.y;

    uint17 address = table.spritePatternGenerator;
    address += (o.pattern << 3) + (x >> 3 << 4) + (y & sizeLimit);

    if(videoRAM.read(address).bit(~x & 7)) {
      if(output) { sprite.collision = 1; break; }
      output = o.color;
    }
  }

  if(output) color = output;
}
