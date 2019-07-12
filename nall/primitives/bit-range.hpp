#pragma once

namespace nall {

//warning: so that BitRange can modify the underlying number directly, it must bind a reference.
//as a result, auto value = number.bits() will capture by-reference, rather than by-value.

template<int Precision> struct BitRange {
  static_assert(Precision >= 1 && Precision <= 64);
  static inline constexpr auto bits() -> uint { return Precision; }
  using utype =
    conditional_t<bits() <=  8,  uint8_t,
    conditional_t<bits() <= 16, uint16_t,
    conditional_t<bits() <= 32, uint32_t,
    conditional_t<bits() <= 64, uint64_t,
    void>>>>;

  BitRange(const BitRange&) = delete;

  inline BitRange(utype& source, int index) : target(source) {
    if(index < 0) index = Precision + index;
    mask = 1ull << index;
    shift = index;
  }

  inline BitRange(utype& source, int lo, int hi) : target(source) {
    if(lo < 0) lo = Precision + lo;
    if(hi < 0) hi = Precision + hi;
    if(lo > hi) swap(lo, hi);
    mask = ~0ull >> 64 - (hi - lo + 1) << lo;
    shift = lo;
  }

  inline auto& operator=(const BitRange& source) {
    target = target & ~mask | (source.target & mask) << shift;
    return *this;
  }

  inline operator utype() const {
    return (target & mask) >> shift;
  }

  inline auto operator++(int) {
    auto value = (target & mask) >> shift;
    target = target & ~mask | target + (1 << shift) & mask;
    return value;
  }

  inline auto operator--(int) {
    auto value = (target & mask) >> shift;
    target = target & ~mask | target - (1 << shift) & mask;
    return value;
  }

  inline auto& operator++() {
    target = target & ~mask | target + (1 << shift) & mask;
    return *this;
  }

  inline auto& operator--() {
    target = target & ~mask | target - (1 << shift) & mask;
    return *this;
  }

  template<typename T> inline auto& operator=(const T& source) {
    target = target & ~mask | source << shift & mask;
    return *this;
  }

  template<typename T> inline auto& operator*=(const T& source) {
    auto value = ((target & mask) >> shift) * source;
    target = target & ~mask | value << shift & mask;
    return *this;
  }

  template<typename T> inline auto& operator/=(const T& source) {
    auto value = ((target & mask) >> shift) / source;
    target = target & ~mask | value << shift & mask;
    return *this;
  }

  template<typename T> inline auto& operator%=(const T& source) {
    auto value = ((target & mask) >> shift) % source;
    target = target & ~mask | value << shift & mask;
    return *this;
  }

  template<typename T> inline auto& operator+=(const T& source) {
    auto value = ((target & mask) >> shift) + source;
    target = target & ~mask | value << shift & mask;
    return *this;
  }

  template<typename T> inline auto& operator-=(const T& source) {
    auto value = ((target & mask) >> shift) - source;
    target = target & ~mask | value << shift & mask;
    return *this;
  }

  template<typename T> inline auto& operator<<=(const T& source) {
    auto value = ((target & mask) >> shift) << source;
    target = target & ~mask | value << shift & mask;
    return *this;
  }

  template<typename T> inline auto& operator>>=(const T& source) {
    auto value = ((target & mask) >> shift) >> source;
    target = target & ~mask | value << shift & mask;
    return *this;
  }

  template<typename T> inline auto& operator&=(const T& source) {
    target = target & (~mask | source << shift & mask);
    return *this;
  }

  template<typename T> inline auto& operator^=(const T& source) {
    target = target ^ source << shift & mask;
    return *this;
  }

  template<typename T> inline auto& operator|=(const T& source) {
    target = target | source << shift & mask;
    return *this;
  }

private:
  utype& target;
  utype mask;
  uint shift;
};

}
