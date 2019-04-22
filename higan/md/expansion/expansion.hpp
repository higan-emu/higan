//Mega CD interface:
//the only actual peripheral released for the expansion port was the Mega CD.
//this class is used to connect a peripheral folder and the BIOS to the emulator.

struct Expansion {
  Node::Port port;
  Node::Peripheral node;

  auto load(Node::Object, Node::Object) -> void;
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;

  auto save() -> void;
  auto power() -> void;

  auto read(uint22 address) -> uint8;
  auto write(uint22 address, uint8 data) -> void;

  auto readIO(uint24 address) -> uint8;
  auto writeIO(uint24 address, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  struct Information {
    string metadata;
    string region;
  };

  Information information;
};

extern Expansion expansion;
