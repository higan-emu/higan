struct Cartridge;

struct CartridgePort {
  CartridgePort(string_view name);
  auto load(Node::Object, Node::Object) -> void;
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;

  const string name;
  Node::Port port;
  maybe<Cartridge&> peripheral;
};

extern CartridgePort cartridgePort;
