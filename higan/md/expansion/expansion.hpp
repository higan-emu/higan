struct Expansion {
  Node::Port port;
  Node::Peripheral node;

  Memory::Readable<uint16> rom;

  auto load(Node::Object, Node::Object) -> void;
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;

  auto save() -> void;
  auto power() -> void;

  auto read(uint22 address) -> uint16;
  auto write(uint22 address, uint16 data) -> void;

  auto readIO(uint24 address) -> uint16;
  auto writeIO(uint24 address, uint16 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  struct Information {
    string metadata;
    string region;
  };

  Information information;
};

extern Expansion expansion;
