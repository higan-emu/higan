namespace higan::Core {

struct RealTimeClock : Object {
  DeclareClass(RealTimeClock, "Real Time Clock")
  using Object::Object;

  auto synchronize(uint64_t timestamp = 0) -> void {
    if(!timestamp) timestamp = chrono::timestamp();
    this->timestamp = timestamp;
    if(update) update();
  }

  auto serialize(string& output, string depth) -> void override {
    Object::serialize(output, depth);
    output.append(depth, "  timestamp: ", timestamp, "\n");
  }

  auto unserialize(Markup::Node node) -> void override {
    Object::unserialize(node);
    timestamp = node["timestamp"].natural();
  }

  uint64_t timestamp;

  function<void ()> update;
};

}
