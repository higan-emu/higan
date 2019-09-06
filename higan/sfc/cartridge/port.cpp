CartridgePort cartridgePort{"Cartridge Port"};

CartridgePort::CartridgePort(string_view name) : name(name) {
}

auto CartridgePort::load(Node::Object parent, Node::Object from) -> void {
  port = Node::append<Node::Port>(parent, from, name);
  port->setFamily("Super Famicom");
  port->setType("Cartridge");
  port->setAllocate([&] { return Node::Peripheral::create(interface->name()); });
  port->setAttach([&](auto node) { connect(node); });
  port->setDetach([&](auto node) { disconnect(); });
  port->scan(from);
}

auto CartridgePort::unload() -> void {
  disconnect();
  port = {};
}

auto CartridgePort::connect(Node::Peripheral node) -> void {
  disconnect();
  if(node) {
    peripheral = cartridge;
    peripheral->connect(port, node);
  }
}

auto CartridgePort::disconnect() -> void {
  if(!peripheral) return;
  peripheral->disconnect();
  peripheral.reset();
}
