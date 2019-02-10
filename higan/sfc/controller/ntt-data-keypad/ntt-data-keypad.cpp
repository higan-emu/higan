auto NTTDataKeypad::create() -> Node::Peripheral {
  auto node = Node::Peripheral::create("NTT Data Keypad");
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
  node->append<Node::Button>("Back");
  node->append<Node::Button>("Next");
  node->append<Node::Button>("1");
  node->append<Node::Button>("2");
  node->append<Node::Button>("3");
  node->append<Node::Button>("4");
  node->append<Node::Button>("5");
  node->append<Node::Button>("6");
  node->append<Node::Button>("7");
  node->append<Node::Button>("8");
  node->append<Node::Button>("9");
  node->append<Node::Button>("0");
  node->append<Node::Button>("*");
  node->append<Node::Button>("C");
  node->append<Node::Button>("#");
  node->append<Node::Button>(".");
  node->append<Node::Button>("End");
  return node;
}

NTTDataKeypad::NTTDataKeypad(Node::Peripheral peripheral) {
  node  = peripheral;
  up    = node->find<Node::Button>("Up");
  down  = node->find<Node::Button>("Down");
  left  = node->find<Node::Button>("Left");
  right = node->find<Node::Button>("Right");
  b     = node->find<Node::Button>("B");
  a     = node->find<Node::Button>("A");
  y     = node->find<Node::Button>("Y");
  x     = node->find<Node::Button>("X");
  l     = node->find<Node::Button>("L");
  r     = node->find<Node::Button>("R");
  back  = node->find<Node::Button>("Back");
  next  = node->find<Node::Button>("Next");
  one   = node->find<Node::Button>("1");
  two   = node->find<Node::Button>("2");
  three = node->find<Node::Button>("3");
  four  = node->find<Node::Button>("4");
  five  = node->find<Node::Button>("5");
  six   = node->find<Node::Button>("6");
  seven = node->find<Node::Button>("7");
  eight = node->find<Node::Button>("8");
  nine  = node->find<Node::Button>("9");
  zero  = node->find<Node::Button>("0");
  star  = node->find<Node::Button>("*");
  clear = node->find<Node::Button>("C");
  pound = node->find<Node::Button>("#");
  point = node->find<Node::Button>(".");
  end   = node->find<Node::Button>("End");
}

auto NTTDataKeypad::data() -> uint2 {
  if(latched == 1) return platform->inputPoll(b), b->value;

  switch(counter++) {
  case  0: return b->value;
  case  1: return y->value;
  case  2: return back->value;
  case  3: return next->value;
  case  4: return upLatch;
  case  5: return downLatch;
  case  6: return leftLatch;
  case  7: return rightLatch;
  case  8: return a->value;
  case  9: return x->value;
  case 10: return l->value;
  case 11: return r->value;

  case 12: return 1;  //4-bit device signature
  case 13: return 0;
  case 14: return 1;
  case 15: return 1;

  case 16: return zero->value;
  case 17: return one->value;
  case 18: return two->value;
  case 19: return three->value;
  case 20: return four->value;
  case 21: return five->value;
  case 22: return six->value;
  case 23: return seven->value;
  case 24: return eight->value;
  case 25: return nine->value;
  case 26: return star->value;
  case 27: return pound->value;
  case 28: return point->value;
  case 29: return clear->value;
  case 30: return 0;  //unverified (likely correct)
  case 31: return end->value;
  }

  counter = 32;
  return 1;  //unverified (likely correct)
}

auto NTTDataKeypad::latch(bool data) -> void {
  if(latched == data) return;
  latched = data;
  counter = 0;

  if(latched == 0) {
    platform->inputPoll(b);
    platform->inputPoll(y);
    platform->inputPoll(back);
    platform->inputPoll(next);
    platform->inputPoll(up);
    platform->inputPoll(down);
    platform->inputPoll(left);
    platform->inputPoll(right);
    platform->inputPoll(a);
    platform->inputPoll(x);
    platform->inputPoll(l);
    platform->inputPoll(r);
    platform->inputPoll(zero);
    platform->inputPoll(one);
    platform->inputPoll(two);
    platform->inputPoll(three);
    platform->inputPoll(four);
    platform->inputPoll(five);
    platform->inputPoll(six);
    platform->inputPoll(seven);
    platform->inputPoll(eight);
    platform->inputPoll(nine);
    platform->inputPoll(star);
    platform->inputPoll(pound);
    platform->inputPoll(point);
    platform->inputPoll(clear);
    platform->inputPoll(end);

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
