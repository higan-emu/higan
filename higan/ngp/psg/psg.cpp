#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

PSG psg;
#include "io.cpp"
#include "tone.cpp"
#include "noise.cpp"
#include "serialization.cpp"

auto PSG::main() -> void {
  uint15 left;
  uint15 right;

  if(psg.enable) {
    if(tone0.run()) {
      left  += amplitude[tone0.volume.left ];
      right += amplitude[tone0.volume.right];
    }
    if(tone1.run()) {
      left  += amplitude[tone1.volume.left ];
      right += amplitude[tone1.volume.right];
    }
    if(tone2.run()) {
      left  += amplitude[tone2.volume.left ];
      right += amplitude[tone2.volume.right];
    }
    if(noise.run()) {
      left  += amplitude[noise.volume.left ];
      right += amplitude[noise.volume.right];
    }
  } else {
    left  += dac.left  << 7;
    right += dac.right << 7;
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

  tone0 = {};
  tone1 = {};
  tone2 = {};
  noise = {};
  psg = {};
  dac = {};
  select = 0;
  for(uint n : range(15)) {
    amplitude[n] = 0x1fff * pow(2, n * -2.0 / 6.0) + 0.5;
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
