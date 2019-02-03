namespace higan::Core::Port {

struct Port : Node {
  using Node::Node;

  template<typename T = Node> auto connected() -> shared_pointer<T> {
    if(nodes) return nodes.first();
    return {};
  }
};

struct Video : Port {
  using Port::Port;

  string type;  //"CRT", "LCD"
  uint width = 0;
  uint height = 0;
  double aspect = 1.0;
};

struct Audio : Port {
  using Port::Port;

  uint channels = 2;
};

struct Cartridge : Port {
  using Port::Port;

  auto connect(function<void (higan::Node::Cartridge)> callback) -> void {
    auto node = higan::Node::Cartridge::create();
    if(callback) callback(node);
    assign(node);
  }
};

struct Peripheral : Port {
  using Port::Port;

  auto connect(higan::Node::Peripheral node) -> void {
    assign(node);
  }
};

}

namespace higan::Node::Port {
  using Port       = shared_pointer<Core::Port::Port>;
  using Video      = shared_pointer<Core::Port::Video>;
  using Audio      = shared_pointer<Core::Port::Audio>;
  using Cartridge  = shared_pointer<Core::Port::Cartridge>;
  using Peripheral = shared_pointer<Core::Port::Peripheral>;
}
