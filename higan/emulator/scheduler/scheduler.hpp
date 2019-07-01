struct Thread;

struct Scheduler {
  enum class Mode : uint {
    Run,
    Serialize,
    SerializePrimary,
    SerializeAuxiliary,
  };

  enum class Event : uint {
    Step,
    Frame,
    Serialize,
  };

  Scheduler() = default;
  Scheduler(const Scheduler&) = delete;
  auto operator=(const Scheduler&) = delete;

  inline auto serializing() const -> bool;

  inline auto reset() -> void;
  inline auto threads() const -> uint;

  inline auto setPrimary(Thread& thread) -> void;

  inline auto append(Thread& thread) -> bool;
  inline auto remove(Thread& thread) -> void;

  inline auto enter(Mode mode = Mode::Run) -> Event;
  inline auto exit(Event event) -> void;

  inline auto serialize() -> void;
  inline auto serialize(Thread& thread) -> void;

private:
  cothread_t _host = nullptr;     //program thread (used to exit scheduler)
  cothread_t _resume = nullptr;   //resume thread (used to enter scheduler)
  cothread_t _primary = nullptr;  //primary thread (used to synchronize components)
  Mode _mode = Mode::Run;
  Event _event = Event::Step;
  vector<Thread*> _threads;

  friend class Thread;
};

extern Scheduler scheduler;
