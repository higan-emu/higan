#pragma once

namespace nall {

auto image::fromARGB32(const uint32_t* from, uint64_t width, uint64_t height) -> image {
  auto data = new uint64_t[width * height];
  auto size = width * height;
  for(uint64_t index : range(size)) {
    uint64_t pixel = from[index];
    data[index] = pixel << 32 & 0xff00'0000'0000'0000
                | pixel << 24 & 0x00ff'ff00'0000'0000
                | pixel << 16 & 0x0000'00ff'ff00'0000
                | pixel <<  8 & 0x0000'0000'00ff'ff00
                | pixel <<  0 & 0x0000'0000'0000'00ff;
  }
  return {move(data), width, height};
}

auto image::toARGB32() const -> shared_pointer<uint32_t> {
  auto data = new uint32_t[width() * height()];
  auto from = _data.data();
  for(uint64_t index : range(size())) {
    uint64_t pixel = from[index];
    data[index] = pixel >> 56 & 0xff000000 | pixel >> 40 & 0xff0000 | pixel >> 24 & 0xff00 | pixel >> 8 & 0xff;
  }
  return data;
}

}
