#include <md/md.hpp>

namespace higan::MegaDrive {

PSG psg;
#include "serialization.cpp"

auto PSG::Enter() -> void {
  while(true) scheduler.synchronize(), psg.main();
}

auto PSG::main() -> void {
  stream->sample(SN76489::clock()[0]);
  step(16);
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
  synchronize(apu);
}

auto PSG::power(bool reset) -> void {
  SN76489::power(0x1400);
  create(PSG::Enter, system.frequency() / 15.0);
  stream = audio.createStream(1, frequency() / 16.0);
  stream->addHighPassFilter(  20.0, Filter::Order::First);
  stream->addLowPassFilter (2840.0, Filter::Order::First);
  stream->addDCRemovalFilter();
}

}
