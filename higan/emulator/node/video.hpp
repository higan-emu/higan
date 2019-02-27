namespace higan::Core {

struct Video : Object {
  DeclareClass(Video, "Video")
  using Object::Object;

  auto serialize(string& output, string depth) -> void override {
    Object::serialize(output, depth);
    output.append(depth, "  type: ", type, "\n");
    output.append(depth, "  width: ", width, "\n");
    output.append(depth, "  height: ", height, "\n");
    output.append(depth, "  scaleX: ", scaleX, "\n");
    output.append(depth, "  scaleY: ", scaleY, "\n");
    output.append(depth, "  aspectX: ", aspectX, "\n");
    output.append(depth, "  aspectY: ", aspectY, "\n");
    output.append(depth, "  colors: ", colors, "\n");
  }

  auto unserialize(Markup::Node node) -> void override {
    Object::unserialize(node);
    type = node["type"].text();
    width = node["width"].natural();
    height = node["height"].natural();
    scaleX = node["scaleX"].real();
    scaleY = node["scaleY"].real();
    aspectX = node["aspectX"].real();
    aspectY = node["aspectY"].real();
    colors = node["colors"].natural();
  }

  string type;  //"CRT", "LCD"
  uint width = 0;
  uint height = 0;
  double scaleX = 1.0;
  double scaleY = 1.0;
  double aspectX = 1.0;
  double aspectY = 1.0;
  uint colors = 0;

  function<uint64 (uint32)> color;
};

}
