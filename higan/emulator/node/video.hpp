namespace higan::Object {

struct Video : Node {
  DeclareClass(Video, "Video")

  using Node::Node;

  auto serialize(string& output, string depth) -> void override {
    Node::serialize(output, depth);
    output.append(depth, "  kind: ", kind, "\n");
    output.append(depth, "  width: ", width, "\n");
    output.append(depth, "  height: ", height, "\n");
    output.append(depth, "  aspect: ", aspect, "\n");
  }

  auto unserialize(Markup::Node node) -> void override {
    Node::unserialize(node);
    kind = node["kind"].text();
    width = node["width"].natural();
    height = node["height"].natural();
    aspect = node["aspect"].real();
  }

  auto copy(shared_pointer<Node> node) -> void override {
    if(auto source = node->cast<shared_pointer<Video>>()) {
      kind = source->kind;
      width = source->width;
      height = source->height;
      aspect = source->aspect;
    }
  }

  string kind;  //"CRT", "LCD"
  uint width = 0;
  uint height = 0;
  double aspect = 1.0;
};

}
