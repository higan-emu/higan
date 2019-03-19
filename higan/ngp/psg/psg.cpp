#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

PSG psg;
#include "io.cpp"
#include "tone.cpp"
#include "noise.cpp"
#include "serialization.cpp"

auto PSG::main() -> void {
  uint15 left  = dac.left  << 6;
  uint15 right = dac.right << 6;

  if(psg.enable) {
    tone0.run();
    tone1.run();
    tone2.run();
    noise.run();

    left  += amplitude[tone0.volume.left ] * tone0.output;
    left  += amplitude[tone1.volume.left ] * tone1.output;
    left  += amplitude[tone2.volume.left ] * tone2.output;
    left  += amplitude[noise.volume.left ] * noise.output;

    right += amplitude[tone0.volume.right] * tone0.output;
    right += amplitude[tone1.volume.right] * tone1.output;
    right += amplitude[tone2.volume.right] * tone2.output;
    right += amplitude[noise.volume.right] * noise.output;
  }

  stream->sample(left / 32767.0, right / 32767.0);
  step(1);
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
  synchronize(apu);
}

auto PSG::power() -> void {
  Thread::create(system.frequency() / 32.0, [&] {
    while(true) scheduler.synchronize(), main();
  });
  stream = audio.createStream(2, frequency());
  stream->addHighPassFilter(20.0, Filter::Order::First);
  stream->addDCRemovalFilter();
  psg = {};
  tone0 = {};
  tone1 = {};
  tone2 = {};
  noise = {};
  dac = {};

  select = 0;
  for(uint n : range(15)) {
    amplitude[n] = 0xfff * pow(2, n * -2.0 / 6.0) + 0.5;
  }
  amplitude[15] = 0;
}

auto PSG::enable() -> void {
  psg.enable = 1;
}

auto PSG::disable() -> void {
  psg.enable = 0;
}

}
