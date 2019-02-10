auto Justifiers::create() -> Node::Peripheral {
  auto node = Node::Peripheral::create("Justifiers");
  node->append<Node::Axis  >("Player 1: X");
  node->append<Node::Axis  >("Player 1: Y");
  node->append<Node::Button>("Player 1: Trigger");
  node->append<Node::Button>("Player 1: Start");
  node->append<Node::Axis  >("Player 2: X");
  node->append<Node::Axis  >("Player 2: Y");
  node->append<Node::Button>("Player 2: Trigger");
  node->append<Node::Button>("Player 2: Start");
  return node;
}

Justifiers::Justifiers(Node::Peripheral peripheral) {
  node     = peripheral;
  x1       = node->find<Node::Axis  >("Player 1: X");
  y1       = node->find<Node::Axis  >("Player 1: Y");
  trigger1 = node->find<Node::Button>("Player 1: Trigger");
  start1   = node->find<Node::Button>("Player 1: Start");
  x2       = node->find<Node::Axis  >("Player 2: X");
  y2       = node->find<Node::Axis  >("Player 2: Y");
  trigger2 = node->find<Node::Button>("Player 2: Trigger");
  start2   = node->find<Node::Button>("Player 2: Start");

  sprite1 = video.createSprite(32, 32);
  sprite1->setPixels(Resource::Sprite::CrosshairGreen);

  sprite2 = video.createSprite(32, 32);
  sprite2->setPixels(Resource::Sprite::CrosshairRed);

  Thread::create(system.cpuFrequency(), [&] {
    while(true) scheduler.synchronize(), main();
  });
}

Justifiers::~Justifiers() {
  video.removeSprite(sprite1);
  video.removeSprite(sprite2);
}

auto Justifiers::main() -> void {
  uint next = cpu.vcounter() * 1364 + cpu.hcounter();

  int px = active == 0 ? x1->value : x2->value;
  int py = active == 0 ? y1->value : y2->value;
  bool offscreen = px < 0 || py < 0 || px >= 256 || py >= ppu.vdisp();

  if(!offscreen) {
    uint target = py * 1364 + (px + 24) * 4;
    if(next >= target && previous < target) {
      //CRT raster detected, strobe iobit to latch counters
      iobit(0);
      iobit(1);
    }
  }

  if(next < previous) {
    platform->inputPoll(x1);
    platform->inputPoll(y1);
    int nx1 = x1->value + cx1;
    int ny1 = y1->value + cy1;
    cx1 = max(-16, min(256 + 16, nx1));
    cy1 = max(-16, min(240 + 16, ny1));
    sprite1->setPosition(cx1 * 2 - 16, cy1 * 2 - 16);
    sprite1->setVisible(true);

    platform->inputPoll(x2);
    platform->inputPoll(y2);
    int nx2 = x2->value + cx2;
    int ny2 = y2->value + cy2;
    cx2 = max(-16, min(256 + 16, nx2));
    cy2 = max(-16, min(240 + 16, ny2));
    sprite2->setPosition(cx2 * 2 - 16, cy2 * 2 - 16);
    sprite2->setVisible(true);
  }

  previous = next;
  step(2);
  synchronize(cpu);
}

auto Justifiers::data() -> uint2 {
  if(counter == 0) {
    platform->inputPoll(trigger1);
    platform->inputPoll(start1);

    platform->inputPoll(trigger2);
    platform->inputPoll(start2);
  }

  switch(counter++) {
  case  0: return 0;
  case  1: return 0;
  case  2: return 0;
  case  3: return 0;
  case  4: return 0;
  case  5: return 0;
  case  6: return 0;
  case  7: return 0;
  case  8: return 0;
  case  9: return 0;
  case 10: return 0;
  case 11: return 0;

  case 12: return 1;  //4-bit device signature
  case 13: return 1;
  case 14: return 1;
  case 15: return 0;

  case 16: return 0;
  case 17: return 1;
  case 18: return 0;
  case 19: return 1;
  case 20: return 0;
  case 21: return 1;
  case 22: return 0;
  case 23: return 1;

  case 24: return trigger1->value;
  case 25: return trigger2->value;
  case 26: return start1->value;
  case 27: return start2->value;
  case 28: return active;
  case 29: return 0;
  case 30: return 0;
  case 31: return 0;
  }

  if(counter > 32) counter = 32;
  return 1;
}

auto Justifiers::latch(bool data) -> void {
  if(latched != data) {
    latched = data;
    counter = 0;
    if(!latched) active = !active;
  }
}
