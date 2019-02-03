Gamepad::Gamepad(uint port, Node node) : Controller(port) {
  up     = node->append(Node::Input::Button::create("Up"));
  down   = node->append(Node::Input::Button::create("Down"));
  left   = node->append(Node::Input::Button::create("Left"));
  right  = node->append(Node::Input::Button::create("Right"));
  b      = node->append(Node::Input::Button::create("B"));
  a      = node->append(Node::Input::Button::create("A"));
  y      = node->append(Node::Input::Button::create("Y"));
  x      = node->append(Node::Input::Button::create("X"));
  l      = node->append(Node::Input::Button::create("L"));
  r      = node->append(Node::Input::Button::create("R"));
  select = node->append(Node::Input::Button::create("Select"));
  start  = node->append(Node::Input::Button::create("Start"));

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
