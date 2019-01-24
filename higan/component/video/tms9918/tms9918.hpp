#pragma once

namespace higan {

//Texas Instruments TMS9918 family

struct TMS9918 {
  //tms9918.cpp
  auto power() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;
};

}
