Controls controls;

auto Controls::load(Node::Object parent, Node::Object from) -> void {
  reset = Node::append<Node::Button>(parent, from, "Reset");
}

auto Controls::poll() -> void {
  auto value = reset->value;
  platform->input(reset);
  if(!value && reset->value) system.power();  //todo: soft reset
}
