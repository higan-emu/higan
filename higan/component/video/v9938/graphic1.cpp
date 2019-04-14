auto V9938::graphic1(uint4& color, uint8 hoffset, uint8 voffset) -> void {
  uint14 patternLayout;
  patternLayout.bits( 0, 4) = hoffset.bits(3,7);
  patternLayout.bits( 5, 9) = voffset.bits(3,7);
  patternLayout.bits(10,13) = table.patternLayout.bits(10,13);
  uint8 pattern = videoRAM.read(patternLayout);

  uint14 patternGenerator;
  patternGenerator.bits( 0, 2) = voffset.bits(0,2);
  patternGenerator.bits( 3,10) = pattern;
  patternGenerator.bits(11,13) = table.patternGenerator.bits(11,13);

  uint14 colorAddress;  //d5 = 0
  colorAddress.bits(0, 4) = pattern.bits(3,7);
  colorAddress.bits(6,13) = table.color.bits(6,13);

  uint8 output = videoRAM.read(colorAddress);
  if(videoRAM.read(patternGenerator).bit(~hoffset & 7)) output >>= 4;
  color = output.bits(0,3);
}
