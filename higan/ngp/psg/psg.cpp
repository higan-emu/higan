#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

PSG psg;
#include "serialization.cpp"

auto PSG::main() -> void {
  stream->sample(SN76489::clock()[0]);
  step(1);
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto PSG::power() -> void {
  SN76489::power(0x2000);
  Thread::create(system.frequency() / 2.0, [&] {
    while(true) scheduler.synchronize(), main();
  });
  stream = audio.createStream(1, frequency());
  stream->addHighPassFilter(20.0, Filter::Order::First);
  stream->addDCRemovalFilter();
}

}
