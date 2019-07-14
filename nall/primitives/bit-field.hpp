#pragma once

namespace nall {

template<int Precision> struct BitField {
  static_assert(Precision >= 1 && Precision <= 64);
  static inline constexpr auto bits() -> uint { return Precision; }
  using utype =
    conditional_t<bits() <=  8,  uint8_t,
    conditional_t<bits() <= 16, uint16_t,
    conditional_t<bits() <= 32, uint32_t,
    conditional_t<bits() <= 64, uint64_t,
    void>>>>;

  BitField(const BitField& source) = delete;

  inline auto& operator=(const BitField& source) {
    target = target & ~mask | (bool)source << shift;
    return *this;
  }

  template<typename T> inline BitField(T* source, int index) : target((utype&)*source) {
    static_assert(sizeof(T) == sizeof(utype));
    if(index < 0) index = Precision + index;
    mask = 1ull << index;
    shift = index;
  }

  inline auto bit() const {
    return shift;
  }

  inline operator bool() const {
    return target & mask;
  }

  inline auto& operator=(bool source) {
    target = target & ~mask | source << shift;
    return *this;
  }

  inline auto& operator&=(bool source) {
    target = target & source << shift;
    return *this;
  }

  inline auto& operator^=(bool source) {
    target = target ^ source << shift;
    return *this;
  }

  inline auto& operator|=(bool source) {
    target = target | source << shift;
    return *this;
  }

private:
  utype& target;
  utype mask;
  uint shift;
};

}
