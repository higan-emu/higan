#include <msx/msx.hpp>

namespace higan::MSX {

PSG psg;
#include "io.cpp"
#include "tone.cpp"
#include "noise.cpp"
#include "envelope.cpp"
#include "serialization.cpp"

auto PSG::main() -> void {
  toneA.clock();
  toneB.clock();
  toneC.clock();
  noise.clock();
  envelope.clock();
  stream->sample(mix());
  step(1);
}

auto PSG::mix() -> double {
  double output = 0.0;
  if((toneA.phase && !channelA.tone) || (noise.lfsr.bit(0) && !channelA.noise)) {
    output += amplitudes[channelA.envelope ? envelope.phase : channelA.amplitude];
  }
  if((toneB.phase && !channelB.tone) || (noise.lfsr.bit(0) && !channelB.noise)) {
    output += amplitudes[channelB.envelope ? envelope.phase : channelB.amplitude];
  }
  if((toneC.phase && !channelC.tone) || (noise.lfsr.bit(0) && !channelC.noise)) {
    output += amplitudes[channelC.envelope ? envelope.phase : channelC.amplitude];
  }
  return output / 3.0;
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto PSG::power() -> void {
  Thread::create(system.colorburst() / 16.0, [&] {
    while(true) scheduler.synchronize(), main();
  });
  stream = audio.createStream(1, frequency());
  stream->addHighPassFilter(20.0, Filter::Order::First);

  toneA = {};
  toneB = {};
  toneC = {};
  noise = {};
  channelA = {};
  channelB = {};
  channelC = {};
  envelope = {};
  io = {};

  //logarithmic volume scale
  amplitudes[ 0] = 0.0000;
  amplitudes[ 1] = 0.0106;
  amplitudes[ 2] = 0.0150;
  amplitudes[ 3] = 0.0222;
  amplitudes[ 4] = 0.0320;
  amplitudes[ 5] = 0.0466;
  amplitudes[ 6] = 0.0665;
  amplitudes[ 7] = 0.1039;
  amplitudes[ 8] = 0.1237;
  amplitudes[ 9] = 0.1986;
  amplitudes[10] = 0.2803;
  amplitudes[11] = 0.3548;
  amplitudes[12] = 0.4702;
  amplitudes[13] = 0.6030;
  amplitudes[14] = 0.7530;
  amplitudes[15] = 0.9250;
//for(uint n : range(16)) print(exp(n / 2.0 - 7.5), "\n");
}

}
