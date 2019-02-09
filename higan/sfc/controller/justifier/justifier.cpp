auto Justifier::create() -> Node::Peripheral {
  auto node = Node::Peripheral::create("Justifier");
  node->append<Node::Axis>("X-Axis");
  node->append<Node::Axis>("Y-Axis");
  node->append<Node::Button>("Trigger");
  node->append<Node::Button>("Start");
  node->append(ControllerPort::create("Link Port"));
  return node;
}

Justifier::Justifier(Node::Peripheral peripheral) : linkPort{"Link Port"} {
  node    = peripheral;
  x       = node->find<Node::Axis>("X-Axis");
  y       = node->find<Node::Axis>("Y-Axis");
  trigger = node->find<Node::Button>("Trigger");
  start   = node->find<Node::Button>("Start");
  linkPort.bind(node->find<Node::Port>(linkPort.name));

  latched  = 0;
  counter  = 0;
  active   = 0;
  previous = 0;

  cx = 256 / 2;
  cy = 240 / 2;
  trigger->value = false;
  start->value   = false;

  sprite = video.createSprite(32, 32);
  if(node->parent.acquire() == controllerPort2.port) {
    Thread::create(system.cpuFrequency(), [&] {
      while(true) scheduler.synchronize(), main();
    });

    sprite->setPixels(Resource::Sprite::CrosshairGreen);
    cx -= 16;
  } else {
    sprite->setPixels(Resource::Sprite::CrosshairRed);
    cx += 16;
  }
}

Justifier::~Justifier() {
  video.removeSprite(sprite);
}

auto Justifier::main() -> void {
  uint next = cpu.vcounter() * 1364 + cpu.hcounter();
  auto linked = this->linked();

  int px = !active ? (int)x->value : -1;
  int py = !active ? (int)y->value : -1;
  if(active && linked) {
    px = linked->x->value;
    py = linked->y->value;
  }

  bool offscreen = (px < 0 || py < 0 || px >= 256 || py >= ppu.vdisp());

  if(!offscreen) {
    uint target = py * 1364 + (px + 24) * 4;
    if(next >= target && previous < target) {
      //CRT raster detected, toggle iobit to latch counters
      iobit(0);
      iobit(1);
    }
  }

  if(next < previous) {
    platform->inputPoll(x);
    platform->inputPoll(y);
    int nx = x->value + cx;
    int ny = y->value + cy;
    cx = max(-16, min(256 + 16, nx));
    cy = max(-16, min(240 + 16, ny));
    sprite->setPosition(cx * 2 - 16, cy * 2 - 16);
    sprite->setVisible(true);
  }

  if(next < previous && linked) {
    platform->inputPoll(linked->x);
    platform->inputPoll(linked->x);
    int nx = linked->x->value + linked->cx;
    int ny = linked->y->value + linked->cy;
    linked->cx = max(-16, min(256 + 16, nx));
    linked->cy = max(-16, min(240 + 16, ny));
    linked->sprite->setPosition(linked->cx * 2 - 16, linked->cy * 2 - 16);
    linked->sprite->setVisible(true);
  }

  previous = next;
  step(2);
  synchronize(cpu);
}

auto Justifier::data() -> uint2 {
  if(counter >= 32) return 1;
  auto linked = this->linked();

  if(counter == 0) {
    platform->inputPoll(trigger);
    platform->inputPoll(start);

    if(linked) {
      platform->inputPoll(linked->trigger);
      platform->inputPoll(linked->start);
    }
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

  case 24: return trigger->value;
  case 25: return linked ? (bool)linked->trigger->value : 0;
  case 26: return start->value;
  case 27: return linked ? (bool)linked->start->value : 0;
  case 28: return active;

  case 29: return 0;
  case 30: return 0;
  case 31: return 0;
  }

  unreachable;
}

auto Justifier::latch(bool data) -> void {
  if(latched == data) return;
  latched = data;
  counter = 0;
  if(latched == 0) active = !active;  //toggle between both controllers, even when unchained
}

auto Justifier::linked() -> maybe<Justifier&> {
  if(auto device = controllerPort2.device) {
    if(auto justifier = dynamic_cast<Justifier*>(device)) {
      if(auto linkedDevice = justifier->linkPort.device) {
        if(auto linkedJustifier = dynamic_cast<Justifier*>(linkedDevice)) {
          return *linkedJustifier;
        }
      }
    }
  }
  return {};
}
