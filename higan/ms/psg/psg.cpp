#include <ms/ms.hpp>

namespace higan::MasterSystem {

PSG psg;
#include "serialization.cpp"

auto PSG::main() -> void {
  auto samples = SN76489::clock();
  stream->sample(samples[0], samples[1]);
  step(1);
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto PSG::balance(uint8 data) -> void {
  SN76489::enable = data;
}

auto PSG::power() -> void {
  SN76489::power(0x2000);
  create(system.colorburst() / 16.0, [&] {
    while(true) scheduler.synchronize(), psg.main();
  });
  stream = audio.createStream(2, frequency());
  stream->addHighPassFilter(20.0, Filter::Order::First);
  stream->addDCRemovalFilter();
}

}
