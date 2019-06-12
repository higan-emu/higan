auto Thread::EntryPoints() -> vector<EntryPoint>& {
  static vector<EntryPoint> entryPoints;
  return entryPoints;
}

auto Thread::Enter() -> void {
  for(uint64_t index : range(EntryPoints().size())) {
    if(co_active() == EntryPoints()[index].handle) {
      auto entryPoint = EntryPoints()[index].entryPoint;
      EntryPoints().remove(index);
      while(true) {
        scheduler.serialize();
        entryPoint();
      }
    }
  }
  struct ThreadNotFound{};
  throw ThreadNotFound{};
}

Thread::~Thread() {
  destroy();
}

auto Thread::active() const -> bool { return co_active() == _handle; }
auto Thread::handle() const -> cothread_t { return _handle; }
auto Thread::frequency() const -> uintmax { return _frequency; }
auto Thread::scalar() const -> uintmax { return _scalar; }
auto Thread::clock() const -> uintmax { return _clock; }

auto Thread::setHandle(cothread_t handle) -> void {
  _handle = handle;
}

auto Thread::setFrequency(double frequency) -> void {
  _frequency = frequency + 0.5;
  _scalar = Second / _frequency;
}

auto Thread::setScalar(uintmax scalar) -> void {
  _scalar = scalar;
}

auto Thread::setClock(uintmax clock) -> void {
  _clock = clock;
}

auto Thread::create(double frequency, function<void ()> entryPoint) -> void {
  if(!_handle) {
    _handle = co_create(64 * 1024 * sizeof(void*), &Thread::Enter);
  } else {
    co_derive(_handle, 64 * 1024 * sizeof(void*), &Thread::Enter);
  }
  EntryPoints().append({_handle, entryPoint});
  setFrequency(frequency);
  setClock(0);
  scheduler.append(*this);
}

auto Thread::destroy() -> void {
  scheduler.remove(*this);
  if(_handle) co_delete(_handle);
  _handle = nullptr;
}

auto Thread::step(uint clocks) -> void {
  _clock += _scalar * clocks;
}

//switches to the thread that is currently the furthest behind in time.
//if that is still the active thread, then do nothing.
auto Thread::synchronize() -> bool {
  Thread& oldest = scheduler.oldest();
  if(oldest.active()) return true;
  scheduler.resume(oldest);
  return false;
}

//synchronize to one thread.
auto Thread::synchronize(Thread& a) -> void {
  if(a.clock() < clock()) {
    scheduler.resume(a);
  }
}

//synchronize to two threads.
auto Thread::synchronize(Thread& a, Thread& b) -> void {
  if(a.clock() < clock()) {
    if(b.clock() < clock()) {
      scheduler.resume(a.clock() < b.clock() ? a : b);
    } else {
      scheduler.resume(a);
    }
  } else if(b.clock() < clock()) {
    scheduler.resume(b);
  }
}

//synchronize to three threads.
//beyond three does not scale; use synchronize() for four or more threads.
auto Thread::synchronize(Thread& a, Thread& b, Thread& c) -> void {
  if(a.clock() < clock()) {
    if(b.clock() < clock()) {
      if(c.clock() < clock()) {
        if(a.clock() < b.clock()) {
          scheduler.resume(a.clock() < c.clock() ? a : c);
        } else {
          scheduler.resume(b.clock() < c.clock() ? b : c);
        }
      } else {
        scheduler.resume(a.clock() < b.clock() ? a : b);
      }
    } else if(c.clock() < clock()) {
      scheduler.resume(a.clock() < c.clock() ? a : c);
    } else {
      scheduler.resume(a);
    }
  } else if(b.clock() < clock()) {
    if(c.clock() < clock()) {
      scheduler.resume(b.clock() < c.clock() ? b : c);
    } else {
      scheduler.resume(b);
    }
  } else if(c.clock() < clock()) {
    scheduler.resume(c);
  }
}

auto Thread::serialize(serializer& s) -> void {
  s.integer(_frequency);
  s.integer(_scalar);
  s.integer(_clock);
}
