#include <sg/sg.hpp>

namespace higan::SG1000 {

PSG psg;
#include "serialization.cpp"

auto PSG::load(Node::Object parent, Node::Object with) -> void {
  audio.attach(stream);
  stream->setChannels(1);
  stream->setFrequency(system.colorburst() / 16.0);
  stream->addHighPassFilter(20.0, Filter::Order::First);
}

auto PSG::unload() -> void {
  audio.detach(stream);
}

auto PSG::main() -> void {
  auto channels = SN76489::clock();
  double output = 0.0;
  output += volume[channels[0]];
  output += volume[channels[1]];
  output += volume[channels[2]];
  output += volume[channels[3]];
  stream->sample(output / 4.0);
  step(1);
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  Thread::synchronize(cpu);
}

auto PSG::power() -> void {
  SN76489::power();
  Thread::create(system.colorburst() / 16.0, {&PSG::main, this});

  for(uint level : range(15)) {
    volume[level] = pow(2, level * -2.0 / 6.0);
  }
  volume[15] = 0;
}

}
