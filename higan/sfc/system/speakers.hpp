struct Speakers {
  Node::Audio node;

  //speakers.cpp
  auto load(Node::Object, Node::Object) -> void;
};

extern Speakers speakers;
