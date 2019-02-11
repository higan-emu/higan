ExpansionPort expansionPort{"Expansion Port"};

ExpansionPort::ExpansionPort(string_view name) : name(name) {
}

auto ExpansionPort::load(Node::Object parent, Node::Object from) -> void {
  port = Node::Port::create(name, "Expansion");
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
  if(from = Node::load(port, from)) {
    if(auto node = from->find<Node::Peripheral>(0)) port->connect(node);
  }
  parent->append(port);
}

auto ExpansionPort::connect(Node::Peripheral node) -> void {
  disconnect();
  if(node) {
    if(node->name == "Satellaview") device = new Satellaview(port, node);
    if(node->name == "21fx") device = new S21FX(port, node);
  }
}

auto ExpansionPort::disconnect() -> void {
  device = {};
}

auto ExpansionPort::serialize(serializer& s) -> void {
}
