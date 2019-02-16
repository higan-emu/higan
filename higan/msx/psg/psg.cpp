#include <msx/msx.hpp>

namespace higan::MSX {

PSG psg;
#include "serialization.cpp"

auto PSG::main() -> void {
  stream->sample(0.0);
  step(1);
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto PSG::power() -> void {
  Thread::create(system.colorburst() / 2.0, [&] {
    while(true) scheduler.synchronize(), main();
  });
  stream = audio.createStream(1, frequency());
  stream->addHighPassFilter(20.0, Filter::Order::First);
  stream->addDCRemovalFilter();
}

}
