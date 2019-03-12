#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

PSG psg;
#include "serialization.cpp"

auto PSG::main() -> void {
  double outLeft  = dacLeft  / 256.0;
  double outRight = dacRight / 256.0;

  if(psgEnable) {
    outLeft  += psgLeft .clock()[0];
    outRight += psgRight.clock()[0];
  }

  if(outLeft  > 1.0) outLeft  = 1.0;
  if(outRight > 1.0) outRight = 1.0;

  stream->sample(outLeft, outRight);
  step(1);
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
  synchronize(apu);
}

auto PSG::power() -> void {
  Thread::create(system.frequency() / 2.0, [&] {
    while(true) scheduler.synchronize(), main();
  });
  stream = audio.createStream(2, frequency());
  stream->addHighPassFilter(20.0, Filter::Order::First);
  stream->addDCRemovalFilter();
  psgLeft .power(0x2000);
  psgRight.power(0x2000);
}

}
