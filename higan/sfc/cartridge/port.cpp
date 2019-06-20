CartridgePort cartridgePort{"Cartridge Port"};

CartridgePort::CartridgePort(string_view name) : name(name) {
}

auto CartridgePort::load(Node::Object parent, Node::Object from) -> void {
  port = Node::append<Node::Port>(parent, from, name, "Cartridge");
  port->allocate = [&] { return Node::Peripheral::create(interface->name()); };
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
  port->scan(from);
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
