#include <pce/pce.hpp>

namespace higan::PCEngine {

PSG psg;
#include "io.cpp"
#include "channel.cpp"
#include "serialization.cpp"

auto PSG::load(Node::Object parent) -> void {
  node = parent->append<Node::Component>("PSG");

  stream = parent->append<Node::Stream>("PSG");
  stream->setChannels(2);
  stream->setFrequency(system.colorburst());
  stream->addHighPassFilter(20.0, 1);
}

auto PSG::unload() -> void {
  node = {};
  stream = {};
}

auto PSG::main() -> void {
  int16 outputLeft;
  int16 outputRight;

  frame(outputLeft, outputRight);
  stream->sample(sclamp<16>(outputLeft) / 32768.0, sclamp<16>(outputRight) / 32768.0);
  step(1);
}

auto PSG::frame(int16& outputLeft, int16& outputRight) -> void {
  static const uint5 volumeScale[16] = {
    0x00, 0x03, 0x05, 0x07, 0x09, 0x0b, 0x0d, 0x0f,
    0x10, 0x13, 0x15, 0x17, 0x19, 0x1b, 0x1d, 0x1f,
  };

  outputLeft  = 0;
  outputRight = 0;

  uint5 lmal = volumeScale[io.volumeLeft];
  uint5 rmal = volumeScale[io.volumeRight];

  for(auto C : range(6)) {
    uint5  al = channel[C].io.volume;
    uint5 lal = volumeScale[channel[C].io.volumeLeft];
    uint5 ral = volumeScale[channel[C].io.volumeRight];

    uint5 volumeLeft  = min(0x1f, (0x1f - lmal) + (0x1f - lal) + (0x1f - al));
    uint5 volumeRight = min(0x1f, (0x1f - rmal) + (0x1f - ral) + (0x1f - al));

    channel[C].run();
    if(C == 1 && io.lfoEnable) {
      //todo: frequency modulation of channel 0 using channel 1's output
    } else {
      outputLeft  += channel[C].io.output * volumeScalar[volumeLeft];
      outputRight += channel[C].io.output * volumeScalar[volumeRight];
    }
  }
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto PSG::power() -> void {
  Thread::create(system.colorburst(), {&PSG::main, this});

  io = {};
  for(auto C : range(6)) channel[C].power(C);

  double level = 32768.0 / 6.0 / 32.0;  //max volume / channels / steps
  double step = 48.0 / 32.0;            //48dB volume range spread over 32 steps
  for(uint n : range(31)) {
    volumeScalar[n] = level;
    level /= pow(10.0, step / 20.0);
  }
  volumeScalar[31] = 0.0;
}

}
