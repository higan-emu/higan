namespace higan::Core {

struct Peripheral : Object {
  DeclareClass(Peripheral, "Peripheral")

  Peripheral(string name = {}, string type = {}) : Object(name), type(type) {
  }

  auto serialize(string& output, string depth) -> void override {
    Object::serialize(output, depth);
    if(type) output.append(depth, "  type: ", type, "\n");
  }

  auto unserialize(Markup::Node node) -> void override {
    Object::unserialize(node);
    type = node["type"].text();
  }

  string type;
};

}
