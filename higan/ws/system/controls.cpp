Controls controls;

auto Controls::load(Node::Object parent, Node::Object from) -> void {
  y1    = Node::append<Node::Button>(parent, from, "Y1");
  y2    = Node::append<Node::Button>(parent, from, "Y2");
  y3    = Node::append<Node::Button>(parent, from, "Y3");
  y4    = Node::append<Node::Button>(parent, from, "Y4");
  x1    = Node::append<Node::Button>(parent, from, "X1");
  x2    = Node::append<Node::Button>(parent, from, "X2");
  x3    = Node::append<Node::Button>(parent, from, "X3");
  x4    = Node::append<Node::Button>(parent, from, "X4");
  b     = Node::append<Node::Button>(parent, from, "B");
  a     = Node::append<Node::Button>(parent, from, "A");
  start = Node::append<Node::Button>(parent, from, "Start");
}

auto Controls::poll() -> void {
  platform->input(y1);
  platform->input(y2);
  platform->input(y3);
  platform->input(y4);
  platform->input(x1);
  platform->input(x2);
  platform->input(x3);
  platform->input(x4);
  platform->input(b);
  platform->input(a);
  platform->input(start);

  if(y1->value || y2->value || y3->value || y4->value
  || x1->value || x2->value || x3->value || x4->value
  || b->value || a->value || start->value
  ) {
    cpu.raise(CPU::Interrupt::Input);
  }
}
