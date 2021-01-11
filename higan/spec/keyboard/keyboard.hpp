struct Keyboard {
  Node::Port port;
  Node::Peripheral layout;

  //keyboard.cpp
  auto load(Node::Object) -> void;
  auto unload() -> void;

  auto allocate(Node::Port, string) -> Node::Peripheral;
  auto connect() -> void;
  auto disconnect() -> void;

  auto power() -> void;
  auto read(uint8 row) -> uint5 ;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  Node::Button keys[40];
};

extern Keyboard keyboard;
