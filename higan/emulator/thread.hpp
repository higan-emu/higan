#pragma once

namespace higan {

struct ThreadEntryPoint {
  cothread_t handle;
  function<void ()> entryPoint;
};

struct Thread {
  enum : uintmax { Second = (uintmax)-1 >> 1 };

  struct EntryPoint {
    cothread_t handle = nullptr;
    function<void ()> entryPoint;
  };

  static auto EntryPoints() -> vector<EntryPoint>& {
    static vector<EntryPoint> entryPoints;
    return entryPoints;
  }

  static auto Enter() -> void {
    for(uint64_t index : range(EntryPoints().size())) {
      if(co_active() == EntryPoints()[index].handle) {
        auto entryPoint = EntryPoints()[index].entryPoint;
        EntryPoints().remove(index);
        while(true) entryPoint();
      }
    }
    struct thread_not_found_exception{};
    throw thread_not_found_exception{};
  }

  virtual ~Thread() {
    if(_handle) co_delete(_handle);
  }

  inline auto active() const { return co_active() == _handle; }
  inline auto handle() const { return _handle; }
  inline auto frequency() const { return _frequency; }
  inline auto scalar() const { return _scalar; }
  inline auto clock() const { return _clock; }

  auto setHandle(cothread_t handle) -> void {
    _handle = handle;
  }

  auto setFrequency(double frequency) -> void {
    _frequency = frequency + 0.5;
    _scalar = Second / _frequency;
  }

  auto setScalar(uintmax scalar) -> void {
    _scalar = scalar;
  }

  auto setClock(uintmax clock) -> void {
    _clock = clock;
  }

  auto create(double frequency, function<void ()> entryPoint) -> void {
    if(_handle) co_delete(_handle);
    _handle = co_create(64 * 1024 * sizeof(void*), &Thread::Enter);
    EntryPoints().append({_handle, entryPoint});
    setFrequency(frequency);
    setClock(0);
  }

  __attribute__((deprecated))
  auto create(auto (*entryPoint)() -> void, double frequency) -> void {
    create(frequency, entryPoint);
  }

  inline auto step(uint clocks) -> void {
    _clock += _scalar * clocks;
  }

  auto serialize(serializer& s) -> void {
    s.integer(_frequency);
    s.integer(_scalar);
    s.integer(_clock);
  }

protected:
  cothread_t _handle = nullptr;
  uintmax _frequency = 0;
  uintmax _scalar = 0;
  uintmax _clock = 0;

  friend class Scheduler;
};

}
