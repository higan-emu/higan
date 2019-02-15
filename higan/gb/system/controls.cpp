Controls controls;

auto Controls::load(Node::Object parent, Node::Object from) -> void {
  if(Model::SuperGameBoy()) return;  //inputs provided by SNES gamepad(s)

  up     = Node::append<Node::Button>(parent, from, "Up");
  down   = Node::append<Node::Button>(parent, from, "Down");
  left   = Node::append<Node::Button>(parent, from, "Left");
  right  = Node::append<Node::Button>(parent, from, "Right");
  b      = Node::append<Node::Button>(parent, from, "B");
  a      = Node::append<Node::Button>(parent, from, "A");
  select = Node::append<Node::Button>(parent, from, "Select");
  start  = Node::append<Node::Button>(parent, from, "Start");
}

auto Controls::poll() -> void {
  if(Model::SuperGameBoy()) {
    auto data = superGameBoy->input();
    rightLatch    = data.bit(0);
    leftLatch     = data.bit(1);
    upLatch       = data.bit(2);
    downLatch     = data.bit(3);
    a->value      = data.bit(4);
    b->value      = data.bit(5);
    select->value = data.bit(6);
    start->value  = data.bit(7);
    return;
  }

  platform->input(up);
  platform->input(down);
  platform->input(left);
  platform->input(right);
  platform->input(b);
  platform->input(a);
  platform->input(select);
  platform->input(start);

  if(!(up->value & down->value)) {
    yHold = 0, upLatch = up->value, downLatch = down->value;
  } else if(!yHold) {
    yHold = 1, swap(upLatch, downLatch);
  }

  if(!(left->value & right->value)) {
    xHold = 0, leftLatch = left->value, rightLatch = right->value;
  } else if(!xHold) {
    xHold = 1, swap(leftLatch, downLatch);
  }
}
