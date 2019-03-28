#include <msx/msx.hpp>

namespace higan::MSX {

PSG psg;
#include "serialization.cpp"

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
  synchronize(cpu);
}

auto PSG::power() -> void {
  AY38910::power();
  Thread::create(system.colorburst() / 16.0, [&] {
    while(true) scheduler.synchronize(), main();
  });
  stream = audio.createStream(1, frequency());
  stream->addHighPassFilter(20.0, Filter::Order::First);

  for(uint level : range(16)) {
    volume[level] = 1.0 / pow(2, 1.0 / 2 * (15 - level));
  }
}

}
