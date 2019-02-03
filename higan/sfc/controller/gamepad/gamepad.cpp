auto Gamepad::create() -> Node::Peripheral {
  auto node = Node::Peripheral::create();
  node->type = "Controller";
  node->name = "Gamepad";
  node->append(Node::Input::Button::create("Up"));
  node->append(Node::Input::Button::create("Down"));
  node->append(Node::Input::Button::create("Left"));
  node->append(Node::Input::Button::create("Right"));
  node->append(Node::Input::Button::create("B"));
  node->append(Node::Input::Button::create("A"));
  node->append(Node::Input::Button::create("Y"));
  node->append(Node::Input::Button::create("X"));
  node->append(Node::Input::Button::create("L"));
  node->append(Node::Input::Button::create("R"));
  node->append(Node::Input::Button::create("Select"));
  node->append(Node::Input::Button::create("Start"));
  return node;
}

Gamepad::Gamepad(uint port, Node::Node parent) : Controller(port) {
  latched = 0;
  counter = 0;

  auto buttons = parent->find<Node::Input::Button>();
  assert(buttons == 12);
  up     = buttons[ 0];
  down   = buttons[ 1];
  left   = buttons[ 2];
  right  = buttons[ 3];
  b      = buttons[ 4];
  a      = buttons[ 5];
  y      = buttons[ 6];
  x      = buttons[ 7];
  l      = buttons[ 8];
  r      = buttons[ 9];
  select = buttons[10];
  start  = buttons[11];
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
