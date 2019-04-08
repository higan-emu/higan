Controls controls;

auto Controls::load(Node::Object parent, Node::Object from) -> void {
  reset      = Node::append<Node::Button>(parent, from, "Reset");
  microphone = Node::append<Node::Button>(parent, from, "Microphone");
}

auto Controls::poll() -> void {
  platform->input(reset);
  platform->input(microphone);
}
