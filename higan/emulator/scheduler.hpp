#pragma once

namespace higan {

struct Scheduler {
  enum class Mode : uint {
    Run,
    SynchronizePrimary,
    SynchronizeAuxiliary,
  };

  enum class Event : uint {
    Step,
    Frame,
    Synchronize,
  };

  inline auto synchronizing() const -> bool { return _mode == Mode::SynchronizeAuxiliary; }

  auto reset() -> void {
    _host = co_active();
    _threads.reset();
  }

  auto primary(Thread& thread) -> void {
    _primary = _resume = thread.handle();
  }

  auto append(Thread& thread) -> bool {
    if(_threads.find(&thread)) return false;
    thread._clock += _threads.size();  //this bias prioritizes threads appended earlier first
    return _threads.append(&thread), true;
  }

  auto remove(Thread& thread) -> void {
    removeWhere(_threads) == &thread;
  }

  auto enter(Mode mode = Mode::Run) -> Event {
    _mode = mode;
    _host = co_active();
    co_switch(_resume);
    return _event;
  }

  //marks a safe point (typically the beginning of the entry point) of a thread.
  //the scheduler may exit at these points for the purpose of serialization.
  inline auto resume() -> void {
    if(co_active() == _primary) {
      if(_mode == Mode::SynchronizePrimary) return exit(Event::Synchronize);
    } else {
      if(_mode == Mode::SynchronizeAuxiliary) return exit(Event::Synchronize);
    }
  }

  inline auto resume(Thread& thread) -> void {
    if(_mode != Mode::SynchronizeAuxiliary) co_switch(thread.handle());
  }

  auto exit(Event event) -> void {
    uintmax minimum = -1;
    for(auto thread : _threads) {
      if(thread->_clock < minimum) minimum = thread->_clock;
    }
    for(auto thread : _threads) {
      thread->_clock -= minimum;
    }

    _event = event;
    _resume = co_active();
    co_switch(_host);
  }

  //switches to the thread that is currently the furthest behind in time.
  //if that is still the active thread, then do nothing.
  inline auto synchronize() -> void {
    uintmax minimum = -1;
    Thread* context = nullptr;
    for(auto thread : _threads) {
      if(thread->_clock < minimum) {
        minimum = thread->_clock;
        context = thread;
      }
    }
    if(!context->active()) {
      resume(*context);
    }
  }

  inline auto synchronize(Thread& thread) -> void {
    if(thread.handle() == _primary) {
      while(enter(Mode::SynchronizePrimary) != Event::Synchronize);
    } else {
      _resume = thread.handle();
      while(enter(Mode::SynchronizeAuxiliary) != Event::Synchronize);
    }
  }

private:
  cothread_t _host = nullptr;     //program thread (used to exit scheduler)
  cothread_t _resume = nullptr;   //resume thread (used to enter scheduler)
  cothread_t _primary = nullptr;  //primary thread (used to synchronize components)
  Mode _mode = Mode::Run;
  Event _event = Event::Step;
  vector<Thread*> _threads;
};

}
