auto SuperMultitap::create() -> Node::Peripheral {
  auto node = Node::Peripheral::create("Super Multitap");
  node->append(ControllerPort::create("Controller Port 1"));
  node->append(ControllerPort::create("Controller Port 2"));
  node->append(ControllerPort::create("Controller Port 3"));
  node->append(ControllerPort::create("Controller Port 4"));
  return node;
}

SuperMultitap::SuperMultitap(Node::Peripheral peripheral):
port1{"Controller Port 1"},
port2{"Controller Port 2"},
port3{"Controller Port 3"},
port4{"Controller Port 4"}
{
  node = peripheral;
  port1.bind(node->find<Node::Port>(port1.name));
  port2.bind(node->find<Node::Port>(port2.name));
  port3.bind(node->find<Node::Port>(port3.name));
  port4.bind(node->find<Node::Port>(port4.name));
}

auto SuperMultitap::data() -> uint2 {
  if(latched) return 2;  //Super Multitap device detection

  if(iobit()) {
    return port1.data().bit(0) << 0 | port2.data().bit(0) << 1;
  } else {
    return port3.data().bit(0) << 0 | port4.data().bit(0) << 1;
  }
}

auto SuperMultitap::latch(bool data) -> void {
  latched = data;
  port1.latch(data);
  port2.latch(data);
  port3.latch(data);
  port4.latch(data);
}
