namespace higan::Core {

struct Port : Object {
  DeclareClass(Port, "Port")

  Port(string name = {}, string type = {}) : Object(name), type(type) {
    allocate = [](auto) { return Node::Peripheral::create("Peripheral"); };
  }

  auto connected() -> Node::Peripheral {
    return find<Node::Peripheral>(0);
  }

  auto connect(Node::Peripheral peripheral) -> void {
    disconnect();
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
  }

  auto unserialize(Markup::Node node) -> void override {
    Object::unserialize(node);
    type = node["type"].text();
  }

  auto copy(Node::Object node) -> void override {
    if(auto source = node->cast<Node::Port>()) {
      type = source->type;
      if(auto peripheral = source->connected()) {
        auto node = allocate(peripheral->name);
        node->copy(peripheral);
        connect(node);
      }
    }
    Object::copy(node);
  }

  string type;

  function<Node::Peripheral (string)> allocate;
  function<void (Node::Peripheral)> attach;
  function<void (Node::Peripheral)> detach;
};

}
