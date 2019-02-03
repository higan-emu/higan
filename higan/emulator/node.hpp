namespace higan {

struct UniqueID {
  auto initialize() -> void { counter = 0; }
  auto operator()() -> uint { return counter++; }
  uint counter = 1;  //0 = no ID (virtual)
};

struct Object;

struct Node : shared_pointer<Object> {
  using shared_pointer<Object>::shared_pointer;
  static auto create() -> Node;
};

struct Property {
  Property(string name, string value = {}) : name(name), value(value) {}
  auto operator==(const Property& source) const -> bool { return name == source.name; }
  auto operator< (const Property& source) const -> bool { return name <  source.name; }

  string name;
  string value;
};

struct Object {
  auto reset() -> void {
    if(nodes && detach) for(auto& node : nodes) detach(node);
    nodes.reset();
  }

  auto assign(Node node) -> void {
    if(nodes && detach) for(auto& node : nodes) detach(node);
    nodes.reset();
    nodes.append(node);
    if(attach) attach(node);
  }

  auto append(Node node) -> void {
    nodes.append(node);
    if(attach) attach(node);
  }

  auto remove(Node node) -> void {
    if(auto index = nodes.find(node)) {
      if(detach) detach(nodes[*index]);
      nodes.remove(*index);
    }
  }

  auto first() -> Node {
    if(nodes) return nodes.first();
    return {};
  }

  auto property(string name) const -> string {
    if(auto property = properties.find(name)) return property->value;
    return {};
  }

  auto setProperty(string name, string value) -> void {
    if(auto property = properties.find(name)) {
      if(value) property->value = value;
      else properties.remove(*property);
    } else {
      if(value) properties.insert({name, value});
    }
  }

  auto serialize(string depth = "") -> string {
    string output;
    output.append(depth, "node\n");
    output.append(depth, "  id: ", id, "\n");
    output.append(depth, "  type: ", type, "\n");
    output.append(depth, "  name: ", name, "\n");
    for(auto& property : properties) {
      output.append(depth, "  property\n");
      output.append(depth, "    name: ", property.name, "\n");
      output.append(depth, "    value: ", property.value, "\n");
    }
    depth.append("  ");
    for(auto node : nodes) output.append(node->serialize(depth));
    return output;
  }

  uint id = 0;
  bool edge = false;
  string type;
  string name;
  set<Property> properties;
  vector<Node> nodes;
  vector<Node> list;

  function<void (Node)> attach;
  function<void (Node)> detach;
};

inline auto Node::create() -> Node {
  return new Object;
}

}
