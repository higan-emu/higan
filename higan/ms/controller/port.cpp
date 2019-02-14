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
    if(node->name == "Number Pad") device = new NumberPad(port, node);
  }
}

auto ControllerPort::disconnect() -> void {
  device = {};
}

auto ControllerPort::serialize(serializer& s) -> void {
}
