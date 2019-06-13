auto Scheduler::serializing() const -> bool {
  return _mode == Mode::SerializeAuxiliary;
}

auto Scheduler::reset() -> void {
  _host = co_active();
  _threads.reset();
}

auto Scheduler::threads() const -> uint {
  return _threads.size();
}

auto Scheduler::setPrimary(Thread& thread) -> void {
  _primary = _resume = thread.handle();
}

auto Scheduler::append(Thread& thread) -> bool {
  if(_threads.find(&thread)) return false;
  thread._clock += _threads.size();  //this bias prioritizes threads appended earlier first
  return _threads.append(&thread), true;
}

auto Scheduler::remove(Thread& thread) -> void {
  removeWhere(_threads) == &thread;
}

auto Scheduler::enter(Mode mode) -> Event {
  if(mode == Mode::Serialize) {
    //run all threads to safe points, starting with the primary thread.
    for(auto thread : _threads) {
      if(thread->handle() == _primary) serialize(*thread);
    }
    for(auto thread : _threads) {
      if(thread->handle() != _primary) serialize(*thread);
    }
    return Event::Serialize;
  }

  _mode = mode;
  _host = co_active();
  co_switch(_resume);
  return _event;
}

auto Scheduler::exit(Event event) -> void {
  //find the thread that is the furthest behind in time.
  Thread* oldest = _threads[0];
  for(auto thread : _threads) {
    if(thread->clock() < oldest->clock()) oldest = thread;
  }
  //subtract its timestamp from all threads to prevent the clock counters from overflowing.
  auto clocks = oldest->clock();
  for(auto thread : _threads) {
    thread->setClock(thread->clock() - clocks);
  }
  //return to the thread that entered the scheduler originally.
  _event = event;
  _resume = co_active();
  co_switch(_host);
}

//marks a safe point (typically the beginning of the entry point) of a thread.
//the scheduler may exit at these points for the purpose of serialization.
auto Scheduler::serialize() -> void {
  if(co_active() == _primary) {
    if(_mode == Mode::SerializePrimary) return exit(Event::Serialize);
  } else {
    if(_mode == Mode::SerializeAuxiliary) return exit(Event::Serialize);
  }
}

//runs a thread until a safe point is reached.
//if it is not the primary thread, synchronization is disabled until this point.
auto Scheduler::serialize(Thread& thread) -> void {
  if(thread.handle() == _primary) {
    while(enter(Mode::SerializePrimary) != Event::Serialize);
  } else {
    _resume = thread.handle();
    while(enter(Mode::SerializeAuxiliary) != Event::Serialize);
  }
}
