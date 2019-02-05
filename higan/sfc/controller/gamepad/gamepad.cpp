auto Gamepad::create() -> Node::Peripheral {
  auto node = Node::Peripheral::create("Gamepad");
  node->append<Node::Button>("Up");
  node->append<Node::Button>("Down");
  node->append<Node::Button>("Left");
  node->append<Node::Button>("Right");
  node->append<Node::Button>("B");
  node->append<Node::Button>("A");
  node->append<Node::Button>("Y");
  node->append<Node::Button>("X");
  node->append<Node::Button>("L");
  node->append<Node::Button>("R");
  node->append<Node::Button>("Select");
  node->append<Node::Button>("Start");
  return node;
}

Gamepad::Gamepad(Node::Peripheral peripheral, uint port) : Controller(port) {
  node   = peripheral;
  up     = node->find<Node::Button>("Up");
  down   = node->find<Node::Button>("Down");
  left   = node->find<Node::Button>("Left");
  right  = node->find<Node::Button>("Right");
  b      = node->find<Node::Button>("B");
  a      = node->find<Node::Button>("A");
  y      = node->find<Node::Button>("Y");
  x      = node->find<Node::Button>("X");
  l      = node->find<Node::Button>("L");
  r      = node->find<Node::Button>("R");
  select = node->find<Node::Button>("Select");
  start  = node->find<Node::Button>("Start");

  latched = 0;
  counter = 0;
}

auto Gamepad::data() -> uint2 {
  if(counter >= 16) return 1;
  if(latched == 1) return platform->inputPoll(b), b->value;

  //note: D-pad physically prevents up+down and left+right from being pressed at the same time
  switch(counter++) {
  case  0: return b->value;
  case  1: return y->value;
  case  2: return select->value;
  case  3: return start->value;
  case  4: return up->value & !down->value;
  case  5: return down->value & !up->value;
  case  6: return left->value & !right->value;
  case  7: return right->value & !left->value;
  case  8: return a->value;
  case  9: return x->value;
  case 10: return l->value;
  case 11: return r->value;
  }

  return 0;  //12-15: signature
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
  }
}
