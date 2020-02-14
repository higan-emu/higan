struct DAC {
  auto power() -> void;

  uint15 cgram[256];

  friend class PPU;
};
