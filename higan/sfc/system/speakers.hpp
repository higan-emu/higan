struct Speakers {
  Node::Audio node;

  auto load(Node::Object, Node::Object) -> void;
};

extern Speakers speakers;
