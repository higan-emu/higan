ICD icd;

#if defined(CORE_GB)

#include "platform.cpp"
#include "interface.cpp"
#include "io.cpp"
#include "serialization.cpp"

auto ICD::main() -> void {
  if(r6003 & 0x80) {
    GameBoy::system.run();
    step(GameBoy::system.information.clocksExecuted);
    GameBoy::system.information.clocksExecuted = 0;
  } else {  //DMG halted
    stream->sample(0.0, 0.0);
    step(2);  //two clocks per audio sample
  }
  synchronize(cpu);
}

auto ICD::load(Node::Peripheral parent, Node::Peripheral from) -> void {
  port = Node::Port::create("Game Boy Cartridge Slot", "Game Boy Cartridge");
  port->allocate = [&] { return Node::Peripheral::create("Game Boy"); };
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
  GameBoy::superGameBoy = this;
  GameBoy::system.node = parent;
  GameBoy::system.information.model = GameBoy::System::Model::SuperGameBoy;
  GameBoy::cartridge.port = port;
  if(from = Node::load(port, from)) {
    if(auto node = from->find<Node::Peripheral>(0)) port->connect(node);
  }
  parent->append(port);
}

auto ICD::connect(Node::Peripheral with) -> void {
  node = Node::Peripheral::create("Game Boy");
  node->load(with);

  GameBoy::cartridge.node = node;
  GameBoy::cartridge.connect(with);
  power();
}

auto ICD::disconnect() -> void {
  GameBoy::cartridge.disconnect();
  Thread::destroy();
  node = {};
  port = {};
}

auto ICD::power() -> void {
  //SGB1 uses CPU oscillator; SGB2 uses dedicated oscillator
  Thread::create((Frequency ? Frequency : system.cpuFrequency()) / 5.0, [&] {
    while(true) {
      if(scheduler.synchronizing()) GameBoy::system.runToSave();
      scheduler.synchronize();
      icd.main();
    }
  });
  cpu.coprocessors.append(this);
  stream = higan::audio.createStream(2, frequency() / 2.0);
  stream->addHighPassFilter(20.0, Filter::Order::First);
  stream->addDCRemovalFilter();

  r6003 = 0x00;
  r6004 = 0xff;
  r6005 = 0xff;
  r6006 = 0xff;
  r6007 = 0xff;
  for(auto& r : r7000) r = 0x00;
  mltReq = 0;

  for(auto& n : output) n = 0xff;
  readBank = 0;
  readAddress = 0;
  writeBank = 0;
  writeAddress = 0;

  packetSize = 0;
  joypID = 3;
  joyp15Lock = 0;
  joyp14Lock = 0;
  pulseLock = true;

  GameBoy::system.power();
}

auto ICD::reset() -> void {
  Thread::create(Frequency ? Frequency : system.cpuFrequency() / 5.0, [&] {
    while(true) {
      if(scheduler.synchronizing()) GameBoy::system.runToSave();
      scheduler.synchronize();
      main();
    }
  });
  cpu.coprocessors.append(this);

  r6003 = 0x00;
  r6004 = 0xff;
  r6005 = 0xff;
  r6006 = 0xff;
  r6007 = 0xff;
  for(auto& r : r7000) r = 0x00;
  mltReq = 0;

  for(auto& n : output) n = 0xff;
  readBank = 0;
  readAddress = 0;
  writeBank = 0;
  writeAddress = 0;

  packetSize = 0;
  joypID = 3;
  joyp15Lock = 0;
  joyp14Lock = 0;
  pulseLock = true;

  GameBoy::system.power();
}

#endif
