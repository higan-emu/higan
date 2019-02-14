namespace higan::Core {

struct Video : Object {
  DeclareClass(Video, "Video")
  using Object::Object;

  auto serialize(string& output, string depth) -> void override {
    Object::serialize(output, depth);
    output.append(depth, "  type: ", type, "\n");
    output.append(depth, "  width: ", width, "\n");
    output.append(depth, "  height: ", height, "\n");
    output.append(depth, "  aspect: ", aspect, "\n");
    output.append(depth, "  colors: ", colors, "\n");
  }

  auto unserialize(Markup::Node node) -> void override {
    Object::unserialize(node);
    type = node["type"].text();
    width = node["width"].natural();
    height = node["height"].natural();
    aspect = node["aspect"].real();
    colors = node["colors"].natural();
  }

  string type;  //"CRT", "LCD"
  uint width = 0;
  uint height = 0;
  double aspect = 1.0;
  uint colors = 0;

  function<uint64 (uint32)> color;
};

}
