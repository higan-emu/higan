ControllerPort controllerPort1{"Controller Port 1"};
ControllerPort controllerPort2{"Controller Port 2"};

ControllerPort::ControllerPort(string_view name) : name(name) {
}

auto ControllerPort::load(Node::Object parent, Node::Object from) -> void {
  port = Node::Port::create(name, "Controller");
  port->hotSwappable = true;
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
  if(from = Node::load(port, from)) {
    if(auto node = from->find<Node::Peripheral>(0)) port->connect(node);
  }
  parent->append(port);
}

auto ControllerPort::connect(Node::Peripheral node) -> void {
  disconnect();
  if(node) {
    if(node->name == "Gamepad") device = new Gamepad(port, node);
    if(node->name == "Justifier") device = new Justifier(port, node);
    if(node->name == "Justifiers") device = new Justifiers(port, node);
    if(node->name == "Mouse") device = new Mouse(port, node);
    if(node->name == "NTT Data Keypad") device = new NTTDataKeypad(port, node);
    if(node->name == "Super Multitap") device = new SuperMultitap(port, node);
    if(node->name == "Super Scope") device = new SuperScope(port, node);
    if(node->name == "Twin Tap") device = new TwinTap(port, node);
  }
}

auto ControllerPort::disconnect() -> void {
  device = {};
}

auto ControllerPort::serialize(serializer& s) -> void {
}
