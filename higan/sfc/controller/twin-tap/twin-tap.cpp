auto TwinTap::create() -> Node::Peripheral {
  auto node = Node::Peripheral::create("Twin Tap");
  node->append<Node::Button>("1");
  node->append<Node::Button>("2");
  return node;
}

TwinTap::TwinTap(Node::Peripheral peripheral) {
  node = peripheral;
  one  = node->find<Node::Button>("1");
  two  = node->find<Node::Button>("2");
}

auto TwinTap::data() -> uint2 {
  if(latched == 1) return platform->inputPoll(two), two->value;  //unconfirmed

  switch(counter++) {
  case 0: return two->value;
  case 1: return one->value;
  }
  if(counter < 24) return 0;  //unconfirmed
  counter = 24;  //unconfirmed
  return 1;  //unconfirmed
}

//unconfirmed
auto TwinTap::latch(bool data) -> void {
  if(latched == data) return;
  latched = data;
  counter = 0;

  if(latched == 0) {
    platform->inputPoll(one);
    platform->inputPoll(two);
  }
}
