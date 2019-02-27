#include <cv/cv.hpp>

namespace higan::ColecoVision {

PSG psg;
#include "serialization.cpp"

auto PSG::main() -> void {
  auto samples = SN76489::clock();
  stream->sample(samples[0]);
  step(1);
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto PSG::power() -> void {
  SN76489::power(0x2000);
  Thread::create(system.colorburst() / 16.0, [&] {
    while(true) scheduler.synchronize(), psg.main();
  });
  stream = audio.createStream(1, frequency());
  stream->addHighPassFilter(20.0, Filter::Order::First);
  stream->addDCRemovalFilter();
}

}
