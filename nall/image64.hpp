#pragma once

//incomplete: the start of a replacement for nall/image:
//use ARGB16 format internally, with functions to convert to and from other formats
//rationale: much faster, easier operations without the need for all the conversions

namespace nall {

struct image64 {
  using type = image64;

  inline auto data() const -> const uint64_t* { return _data ? _data.data() : nullptr; }
  inline auto size() const -> uint64_t { return _width * _height; }
  inline auto width() const -> uint64_t { return _width; }
  inline auto height() const -> uint64_t { return _height; }

  inline image64() = default;
  inline image64(uint width, uint height) { allocate(width, height); }

  //core.hpp
  inline image64(const uint64_t* data, uint64_t width, uint64_t height);
  inline image64(uint64_t*&& data, uint64_t width, uint64_t height);
  inline auto data() -> shared_pointer<uint64_t>;
  inline auto allocate(uint64_t width, uint64_t height, uint64_t color = 0) -> type&;

  //convert.hpp
  static inline auto fromARGB32(const uint32_t* data, uint64_t width, uint64_t height) -> type;
  inline auto toARGB32() const -> shared_pointer<uint32_t>;

private:
  shared_pointer<uint64_t> _data;
  uint64_t _width = 0;
  uint64_t _height = 0;
};

}

#define image image64
#include <nall/image64/core.hpp>
#include <nall/image64/convert.hpp>
#undef image
