Gamepad::Gamepad(Node::Port parent, Node::Peripheral with) {
  parent->prepend(node = Node::Peripheral::create("Gamepad", parent->type));
  node->load(with);

  up     = Node::append<Node::Button>(node, with, "Up");
  down   = Node::append<Node::Button>(node, with, "Down");
  left   = Node::append<Node::Button>(node, with, "Left");
  right  = Node::append<Node::Button>(node, with, "Right");
  b      = Node::append<Node::Button>(node, with, "B");
  a      = Node::append<Node::Button>(node, with, "A");
  y      = Node::append<Node::Button>(node, with, "Y");
  x      = Node::append<Node::Button>(node, with, "X");
  l      = Node::append<Node::Button>(node, with, "L");
  r      = Node::append<Node::Button>(node, with, "R");
  select = Node::append<Node::Button>(node, with, "Select");
  start  = Node::append<Node::Button>(node, with, "Start");
}

auto Gamepad::data() -> uint2 {
  if(latched == 1) return platform->inputPoll(b), b->value;

  //note: D-pad physically prevents up+down and left+right from being pressed at the same time
  switch(counter++) {
  case  0: return b->value;
  case  1: return y->value;
  case  2: return select->value;
  case  3: return start->value;
  case  4: return upLatch;
  case  5: return downLatch;
  case  6: return leftLatch;
  case  7: return rightLatch;
  case  8: return a->value;
  case  9: return x->value;
  case 10: return l->value;
  case 11: return r->value;

  case 12: return 0;  //4-bit device signature
  case 13: return 0;
  case 14: return 0;
  case 15: return 0;
  }

  counter = 16;
  return 1;
}

auto Gamepad::latch(bool data) -> void {
  if(latched == data) return;
  latched = data;
  counter = 0;

  if(latched == 0) {
    platform->inputPoll(b);
    platform->inputPoll(y);
    platform->inputPoll(select);
    platform->inputPoll(start);
    platform->inputPoll(up);
    platform->inputPoll(down);
    platform->inputPoll(left);
    platform->inputPoll(right);
    platform->inputPoll(a);
    platform->inputPoll(x);
    platform->inputPoll(l);
    platform->inputPoll(r);

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
  }
}
