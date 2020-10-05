#include <spec/spec.hpp>

namespace higan::Spectrum {

PSG psg;
#include "serialization.cpp"

auto PSG::load(Node::Object parent) -> void {
  node = parent->append<Node::Component>("PSG");

  stream = node->append<Node::Stream>("PSG");
  stream->setChannels(1);
  stream->setFrequency(system.frequency() / 16);
}

auto PSG::unload() -> void {
  node = {};
  stream = {};
}

auto PSG::main() -> void {
  auto channels = AY38910::clock();
  double output = 0.0;
  output += volume[channels[0]];
  output += volume[channels[1]];
  output += volume[channels[2]];
  stream->sample(output / 3.0);
  step(1);
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  Thread::synchronize();
}

auto PSG::power() -> void {
  AY38910::power();
  Thread::create(system.frequency() / 16, {&PSG::main, this});

  for(uint level : range(16)) {
    volume[level] = 1.0 / pow(2, 1.0 / 2 * (15 - level));
  }
}

auto PSG::readIO(uint1 port) -> uint8 {
  return 0xff;
}

auto PSG::writeIO(uint1 port, uint8 data) -> void {

}

}
