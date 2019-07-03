struct Cartridge {
  Node::Port port;
  Node::Peripheral node;

  inline auto manifest() const -> string { return information.manifest; }
  inline auto name() const -> string { return information.name; }

  //cartridge.cpp
  auto load(Node::Object, Node::Object) -> void;
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;

  auto save() -> void;
  auto power() -> void;

  auto read(uint20 addr) -> uint8;
  auto write(uint20 addr, uint8 data) -> void;

private:
  auto mirror(uint addr, uint size) -> uint;

  struct Information {
    string manifest;
    string name;
  } information;

  struct Memory {
    uint8* data = nullptr;
    uint size = 0;
  };

  Memory rom;
};

extern Cartridge cartridge;
