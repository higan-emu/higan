#if defined(Hiro_Monitor)
struct Monitor {
  Monitor() = delete;

  static auto count() -> uint;
  static auto dpi(maybe<uint> monitor = nothing) -> Position;
  static auto geometry(maybe<uint> monitor = nothing) -> Geometry;
  static auto primary() -> uint;
  static auto workspace(maybe<uint> monitor = nothing) -> Geometry;
};

//DPI scale X
inline auto operator"" _sx(unsigned long long x) -> float {
  static auto scale = Monitor::dpi().x() / 96.0;
  return round(x * scale);
}

//DPI scale Y
inline auto operator"" _sy(unsigned long long y) -> float {
  static auto scale = Monitor::dpi().y() / 96.0;
  return round(y * scale);
}
#endif
