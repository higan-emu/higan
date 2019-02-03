namespace higan::Core::Port {

struct Port : Node {
  DeclareClass(Port, "Port")

  using Node::Node;

  auto connected() -> shared_pointer<Node> {
    return Node::first();
  }

  auto connect(function<void (shared_pointer<Node>)> callback) -> shared_pointer<Node> {
    auto node = createConnected();
    if(callback) callback(node);
    reset();
    append(node);
    return node;
  }

  virtual auto createConnected() const -> shared_pointer<Node> { return new Node; }
};

struct Video : Port {
  DeclareClass(Video, "Port::Video")

  using Port::Port;

  string type;  //"CRT", "LCD"
  uint width = 0;
  uint height = 0;
  double aspect = 1.0;
};

struct Audio : Port {
  DeclareClass(Audio, "Port::Audio")

  using Port::Port;

  uint channels = 2;
};

struct Cartridge : Port {
  DeclareClass(Cartridge, "Port::Cartridge")

  auto createConnected() const -> shared_pointer<Node> override { return new Core::Cartridge; }

  using Port::Port;
};

struct Peripheral : Port {
  DeclareClass(Peripheral, "Port::Peripheral")

  auto createConnected() const -> shared_pointer<Node> override { return new Core::Peripheral; }

  using Port::Port;
};

}
