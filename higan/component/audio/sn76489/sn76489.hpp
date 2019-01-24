#pragma once

namespace higan {

//Texas Instruments SN76489
//Game Gear: stereo audio extension

struct SN76489 {
  //sn76489.cpp
  auto clock() -> array<double[2]>;
  auto power(uint scale) -> void;

  //io.cpp
  auto write(uint8 data) -> void;
  auto balance(uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

protected:
  struct Tone {
    //tone.cpp
    auto run() -> void;
    auto power() -> void;

    //serialization.cpp
    auto serialize(serializer&) -> void;

    uint4  volume;
    uint10 counter;
    uint10 pitch;
    uint1  output;
  };

  struct Noise {
    //noise.cpp
    auto run() -> void;
    auto power() -> void;

    //serialization.cpp
    auto serialize(serializer&) -> void;

    uint4  volume;
    uint10 counter;
    uint10 pitch;
    uint1  enable;
    uint2  rate;
    uint16 lfsr;
    uint1  clock;
    uint1  output;
  };

  Tone tone0;
  Tone tone1;
  Tone tone2;
  Noise noise;

  uint3 select;
  uint8 enable;  //Game Gear
  int16 amplitude[16];
};

}
