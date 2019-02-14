Speakers speakers;

auto Speakers::load(Node::Object parent, Node::Object from) -> void {
  node = Node::Audio::create("Speakers");
  Node::load(node, from);
  parent->append(node);
}
