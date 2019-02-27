struct Cartridge : Thread, MMIO {
  Node::Port port;
  Node::Peripheral node;

  inline auto metadata() const -> string { return information.metadata; }

  //cartridge.cpp
  auto load(Node::Object, Node::Object) -> void;
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;
  auto save() -> void;

  auto main() -> void;
  auto step(uint clocks) -> void;
  auto power() -> void;
  auto second() -> void;

  auto readIO(uint16 address) -> uint8;
  auto writeIO(uint16 address, uint8 data) -> void;

  auto serialize(serializer&) -> void;

  struct Information {
    string metadata;
  } information;

  struct Memory {
    auto read(uint address) const -> uint8;
    auto write(uint address, uint8 data) -> void;

    uint8* data = nullptr;
    uint size = 0;
  } rom, ram, rtc;

  bool bootromEnable = true;

private:
  struct Mapper {
    virtual auto load(Markup::Node document) -> void {}
    virtual auto save(Markup::Node document) -> void {}
    virtual auto main() -> void;
    virtual auto second() -> void {}
    virtual auto read(uint16 address) -> uint8 = 0;
    virtual auto write(uint16 address, uint8 data) -> void = 0;
    virtual auto power() -> void = 0;
    virtual auto serialize(serializer&) -> void = 0;
  };
  Mapper* mapper = nullptr;
  bool accelerometer = false;
  bool rumble = false;

  #include "mbc0/mbc0.hpp"
  #include "mbc1/mbc1.hpp"
  #include "mbc1m/mbc1m.hpp"
  #include "mbc2/mbc2.hpp"
  #include "mbc3/mbc3.hpp"
  #include "mbc5/mbc5.hpp"
  #include "mbc6/mbc6.hpp"
  #include "mbc7/mbc7.hpp"
  #include "mmm01/mmm01.hpp"
  #include "huc1/huc1.hpp"
  #include "huc3/huc3.hpp"
  #include "tama/tama.hpp"
};

extern Cartridge cartridge;
