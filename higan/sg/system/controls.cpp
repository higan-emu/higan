Controls controls;

auto Controls::load(Node::Object parent, Node::Object from) -> void {
  pause = Node::append<Node::Button>(parent, from, "Pause");
}

auto Controls::poll() -> void {
  auto value = pause->value;
  platform->input(pause);
  if(!value && pause->value) cpu.setNMI(1);
}
