struct APU : Z80, Z80::Bus, Thread {
  Memory::Writable<uint8> ram;

  //apu.cpp
  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto synchronizing() const -> bool override;
  auto power() -> void;

  //memory.cpp
  auto read(uint16 address) -> uint8 override { return 0; }
  auto write(uint16 address, uint8 data) -> void override {}

  auto in(uint8 address) -> uint8 override { return 0; }
  auto out(uint8 address, uint8 data) -> void override {}

  //serialization.cpp
  auto serialize(serializer&) -> void;
};

extern APU apu;
