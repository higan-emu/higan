Gamepad::Gamepad(Node::Port parent, Node::Peripheral with) {
  node = Node::Peripheral::create("Gamepad");
  node->load(with);
  up = Node::append<Node::Button>(node, with, "Up");
  down = Node::append<Node::Button>(node, with, "Down");
  left = Node::append<Node::Button>(node, with, "Left");
  right = Node::append<Node::Button>(node, with, "Right");
  b = Node::append<Node::Button>(node, with, "B");
  a = Node::append<Node::Button>(node, with, "A");
  parent->prepend(node);
}

auto Gamepad::read() -> uint6 {
  platform->input(up);
  platform->input(down);
  platform->input(left);
  platform->input(right);
  platform->input(b);
  platform->input(a);

  if(!(up->value & down->value)) {
    yHold = 0, upLatch = up->value, downLatch = down->value;
  } else if(!yHold) {
    yHold = 1, swap(upLatch, downLatch);
  }

  if(!(left->value & right->value)) {
    xHold = 0, leftLatch = left->value, rightLatch = right->value;
  } else if(!xHold) {
    xHold = 1, swap(leftLatch, rightLatch);
  }

  uint6 data;
  data.bit(0) = !upLatch;
  data.bit(1) = !downLatch;
  data.bit(2) = !leftLatch;
  data.bit(3) = !rightLatch;
  data.bit(4) = !b->value;
  data.bit(5) = !a->value;
  return data;
}
