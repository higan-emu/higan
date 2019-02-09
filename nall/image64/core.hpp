#pragma once

namespace nall {

image::image(const uint64_t* from, uint64_t width, uint64_t height) {
  auto data = new uint64_t[width * height];
  memory::copy<uint64_t>(data, from, width * height);
  _data = data;
  _width = width;
  _height = height;
}

image::image(uint64_t*&& data, uint64_t width, uint64_t height) {
  _data = data;
  _width = width;
  _height = height;
}

auto image::data() -> shared_pointer<uint64_t> {
  return _data;
}

auto image::allocate(uint64_t width, uint64_t height, uint64_t color) -> type& {
  _data.reset();
  _width = width;
  _height = height;
  auto data = new uint64_t[width * height];
  for(uint64_t index : range(size())) data[index] = color;
  _data = data;
  return *this;
}

}
