#include <emulator/emulator.hpp>
#include "sn76489.hpp"

namespace higan {

#include "io.cpp"
#include "tone.cpp"
#include "noise.cpp"
#include "serialization.cpp"

auto SN76489::clock() -> array<double[2]> {
  tone0.run();
  tone1.run();
  tone2.run();
  noise.run();

  int left = 0;
  left  += amplitude[tone0.volume] * tone0.output * enable.bit(4);
  left  += amplitude[tone1.volume] * tone1.output * enable.bit(5);
  left  += amplitude[tone2.volume] * tone2.output * enable.bit(6);
  left  += amplitude[noise.volume] * noise.output * enable.bit(7);

  int right = 0;
  right += amplitude[tone0.volume] * tone0.output * enable.bit(0);
  right += amplitude[tone1.volume] * tone1.output * enable.bit(1);
  right += amplitude[tone2.volume] * tone2.output * enable.bit(2);
  right += amplitude[noise.volume] * noise.output * enable.bit(3);

  return {sclamp<16>(left) / 32767.0, sclamp<16>(right) / 32767.0};
}

auto SN76489::power(uint scale) -> void {
  select = 0;
  enable = ~0;
  for(uint n : range(15)) {
    amplitude[n] = scale * pow(2, n * -2.0 / 6.0) + 0.5;
  }
  amplitude[15] = 0;

  tone0.power();
  tone1.power();
  tone2.power();
  noise.power();
}

}
