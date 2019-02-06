namespace higan::Core {

struct Port : Object {
  DeclareClass(Port, "Port")

  Port(string name = {}, string type = {}) : Object(name), type(type) {
    allocate = [](auto) { return Node::Peripheral::create("Peripheral"); };
  }

  auto connected() -> Node::Peripheral {
    return find<Node::Peripheral>(0);
  }

  auto connect(string name, function<void (Node::Peripheral)> bind) -> void {
    disconnect();
    auto peripheral = allocate(name);
    if(bind) bind(peripheral);
    prepend(peripheral);
    if(attach) attach(peripheral);
  }

  auto disconnect() -> void {
    if(auto peripheral = connected()) {
      if(detach) detach(peripheral);
      remove(peripheral);
    }
  }

  auto serialize(string& output, string depth) -> void override {
    Object::serialize(output, depth);
    if(type) output.append(depth, "  type: ", type, "\n");
    output.append(depth, "  hotSwappable: ", hotSwappable, "\n");
  }

  auto unserialize(Markup::Node node) -> void override {
    Object::unserialize(node);
    type = node["type"].text();
    hotSwappable = node["hotSwappable"].boolean();
  }

  auto copy(Node::Object node) -> void override {
    if(auto source = node->cast<Node::Port>()) {
      type = source->type;
      hotSwappable = source->hotSwappable;
      if(auto peripheral = source->connected()) {
        connect(peripheral->name, [&](auto node) {
          node->copy(peripheral);
        });
      }
    }
    Object::copy(node);
  }

  string type;
  bool hotSwappable = false;

  function<Node::Peripheral (string)> allocate;
  function<void (Node::Peripheral)> attach;
  function<void (Node::Peripheral)> detach;
};

}
