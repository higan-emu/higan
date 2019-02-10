//The Super Scope is a light-gun: it detects the CRT beam cannon position,
//and latches the counters by toggling iobit. This only works on controller
//port 2, as iobit there is connected to the PPU H/V counter latch.
//(PIO $4201.d7)

//It is obviously not possible to perfectly simulate an IR light detecting
//a CRT beam cannon, hence this class will read the PPU raster counters.

//A Super Scope can still technically be used in port 1, however it would
//require manual polling of PIO ($4201.d6) to determine when iobit was written.
//Note that no commercial game ever utilizes a Super Scope in port 1.

auto SuperScope::create() -> Node::Peripheral {
  auto node = Node::Peripheral::create("Super Scope");
  node->append<Node::Axis  >("X");
  node->append<Node::Axis  >("Y");
  node->append<Node::Button>("Trigger");
  node->append<Node::Button>("Cursor");
  node->append<Node::Button>("Turbo");
  node->append<Node::Button>("Pause");
  return node;
}

SuperScope::SuperScope(Node::Peripheral peripheral) {
  node    = peripheral;
  x       = node->find<Node::Axis  >("X");
  y       = node->find<Node::Axis  >("Y");
  trigger = node->find<Node::Button>("Trigger");
  cursor  = node->find<Node::Button>("Cursor");
  turbo   = node->find<Node::Button>("Turbo");
  pause   = node->find<Node::Button>("Pause");

  sprite = video.createSprite(32, 32);
  sprite->setPixels(Resource::Sprite::CrosshairGreen);

  Thread::create(system.cpuFrequency(), [&] {
    while(true) scheduler.synchronize(), main();
  });
}

SuperScope::~SuperScope() {
  video.removeSprite(sprite);
}

auto SuperScope::main() -> void {
  uint next = cpu.vcounter() * 1364 + cpu.hcounter();

  if(!offscreen) {
    uint target = cy * 1364 + (cx + 24) * 4;
    if(next >= target && previous < target) {
      //CRT raster detected, strobe iobit to latch counters
      iobit(0);
      iobit(1);
    }
  }

  if(next < previous) {
    //Vcounter wrapped back to zero; update cursor coordinates for start of new frame
    platform->inputPoll(x);
    platform->inputPoll(y);
    int nx = x->value + cx;
    int ny = y->value + cy;
    cx = max(-16, min(256 + 16, nx));
    cy = max(-16, min(240 + 16, ny));
    offscreen = (cx < 0 || cy < 0 || cx >= 256 || cy >= ppu.vdisp());
    sprite->setPosition(cx * 2 - 16, cy * 2 - 16);
    sprite->setVisible(true);
  }

  previous = next;
  step(2);
  synchronize(cpu);
}

auto SuperScope::data() -> uint2 {
  if(counter == 0) {
    //turbo is a switch; toggle is edge sensitive
    platform->inputPoll(turbo);
    bool turboNew = turbo->value;
    if(turboNew && !turboOld) {
      turboEdge = !turboEdge;  //toggle state
      sprite->setPixels(turboEdge ? (image)Resource::Sprite::CrosshairRed : (image)Resource::Sprite::CrosshairGreen);
    }
    turboOld = turboNew;

    //trigger is a button
    //if turbo is active, trigger is level sensitive; otherwise, it is edge sensitive
    triggerValue = false;
    platform->inputPoll(trigger);
    bool triggerNew = trigger->value;
    if(triggerNew && (turboEdge || !triggerLock)) {
      triggerValue = true;
      triggerLock = true;
    } else if(!triggerNew) {
      triggerLock = false;
    }

    //cursor is a button; it is always level sensitive
    platform->inputPoll(cursor);

    //pause is a button; it is always edge sensitive
    pauseEdge = false;
    platform->inputPoll(pause);
    bool pauseNew = pause->value;
    if(pauseNew && !pauseLock) {
      pauseEdge = true;
      pauseLock = true;
    } else if(!pauseNew) {
      pauseLock = false;
    }

    offscreen = (cx < 0 || cy < 0 || cx >= 256 || cy >= ppu.vdisp());
  }

  switch(counter++) {
  case 0: return triggerValue & !offscreen;
  case 1: return cursor->value;
  case 2: return turboEdge;
  case 3: return pauseEdge;
  case 4: return 0;
  case 5: return 0;
  case 6: return offscreen;
  case 7: return 0;  //noise (1 = yes)
  }

  if(counter > 8) counter = 8;
  return 1;
}

auto SuperScope::latch(bool data) -> void {
  if(latched != data) {
    latched = data;
    counter = 0;
  }
}
