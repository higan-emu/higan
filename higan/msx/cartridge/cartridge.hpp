struct Cartridge {
  Node::Port port;
  Node::Peripheral node;

  inline auto region() const -> string { return information.region; }

  //cartridge.cpp
  Cartridge(string_view name);
  auto load(Node::Object, Node::Object) -> void;
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;

  auto save() -> void;
  auto power() -> void;

  auto read(uint16 address) -> uint8;
  auto write(uint16 address, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  struct Information {
    string region;
    string manifest;
  } information;

  Memory::Readable<uint8> rom;
  Memory::Writable<uint8> ram;

  const string name;
};

extern Cartridge cartridge;
extern Cartridge expansion;
