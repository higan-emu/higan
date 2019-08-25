#include <ms/ms.hpp>

namespace higan::MasterSystem {

OPLL opll;
#include "serialization.cpp"

auto OPLL::load(Node::Object parent, Node::Object with) -> void {
  audio.attach(stream);
  stream->setChannels(1);
  stream->setFrequency(system.colorburst() / 72.0);
  stream->addHighPassFilter(20.0, Filter::Order::First);
}

auto OPLL::unload() -> void {
  audio.detach(stream);
}

auto OPLL::main() -> void {
  auto output = YM2413::clock();
  stream->sample(output);
  step(1);
}

auto OPLL::step(uint clocks) -> void {
  Thread::step(clocks);
  Thread::synchronize(cpu);
}

auto OPLL::power() -> void {
  YM2413::power();
  Thread::create(system.colorburst() / 72.0, {&OPLL::main, this});
}

}
