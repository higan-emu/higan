CartridgePort cartridgePort{"Cartridge Port"};

CartridgePort::CartridgePort(string_view name) : name(name) {
}

auto CartridgePort::load(Node::Object parent, Node::Object from) -> void {
  port = Node::Port::create(name, "Cartridge");
  port->allocate = [&] { return Node::Peripheral::create(interface->name()); };
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
  if(from = Node::load(port, from)) {
    if(auto node = from->find<Node::Peripheral>(0)) port->connect(node);
  }
  parent->append(port);
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
