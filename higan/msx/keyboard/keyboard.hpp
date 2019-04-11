struct Keyboard {
  //keyboard.cpp
  auto load(Node::Object, Node::Object) -> void;

  auto power() -> void;
  auto read() -> uint8;
  auto write(uint4 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  struct IO {
    uint4 select;
  } io;
};

extern Keyboard keyboard;
