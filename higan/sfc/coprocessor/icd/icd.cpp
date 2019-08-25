ICD icd;

#if defined(CORE_GB)

#include "interface.cpp"
#include "io.cpp"
#include "serialization.cpp"

auto ICD::load(Node::Peripheral parent, Node::Peripheral from) -> void {
  auto frequency = (Frequency ? Frequency : system.cpuFrequency()) / 5.0;

  higan::audio.attach(stream);
  stream->setChannels(2);
  stream->setFrequency(frequency / 2.0);
  stream->addHighPassFilter(20.0, Filter::Order::First);

  port = Node::append<Node::Port>(parent, from, "Game Boy Cartridge Slot", "Game Boy Cartridge");
  port->allocate = [&] { return Node::Peripheral::create("Game Boy"); };
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
  GameBoy::superGameBoy = this;
  GameBoy::system.node = parent;
  GameBoy::system.information.model = GameBoy::System::Model::SuperGameBoy;
  GameBoy::cpu.version->setAllowedValues({"SGB-CPU-01", "CPU SGB2"});
  GameBoy::cpu.version->setValue(!Frequency ? "SGB-CPU-01" : "CPU SGB2");
  GameBoy::cpu.version->setLatch();
  GameBoy::cartridge.port = port;
  port->scan(from);

  GameBoy::cpu.load(parent, from);
}

auto ICD::unload() -> void {
  higan::audio.detach(stream);

  disconnect();
  port = {};

  GameBoy::cpu.unload();
}

auto ICD::connect(Node::Peripheral with) -> void {
  node = Node::append<Node::Peripheral>(port, with, "Game Boy");

  GameBoy::cartridge.node = node;
  GameBoy::cartridge.connect(with);
  power();
}

auto ICD::disconnect() -> void {
  if(!node) return;
  GameBoy::cartridge.disconnect();
  cpu.coprocessors.removeByValue(this);
  Thread::destroy();
  node = {};
}

auto ICD::name() const -> string {
  return GameBoy::interface->game();
}

auto ICD::main() -> void {
  if(r6003.bit(7)) {
    GameBoy::system.run();
    Thread::step(GameBoy::system.clocksExecuted());
  } else {  //DMG halted
    stream->sample(0.0, 0.0);
    Thread::step(2);  //two clocks per audio sample
  }
  Thread::synchronize(cpu);
}

auto ICD::power(bool reset) -> void {
  //SGB1 uses CPU oscillator; SGB2 uses dedicated oscillator
  Thread::create((Frequency ? Frequency : system.cpuFrequency()) / 5.0, [&] {
    while(true) {
      if(scheduler.serializing()) GameBoy::system.runToSave();
      scheduler.serialize();
      main();
    }
  });
  cpu.coprocessors.append(this);

  for(auto& packet : this->packet) packet = {};
  packetSize = 0;

  joypID = 3;
  joyp14Lock = 0;
  joyp15Lock = 0;
  pulseLock = 1;
  strobeLock = 0;
  packetLock = 0;
  joypPacket = {};
  packetOffset = 0;
  bitData = 0;
  bitOffset = 0;

  for(auto& n : output) n = 0xff;
  readBank = 0;
  readAddress = 0;
  writeBank = 0;

  r6003 = 0x00;
  r6004 = 0xff;
  r6005 = 0xff;
  r6006 = 0xff;
  r6007 = 0xff;
  for(auto& r : r7000) r = 0x00;
  mltReq = 0;

  hcounter = 0;
  vcounter = 0;

  GameBoy::system.power();
}

#endif
