struct Expansion : Thread {
  Expansion();
  virtual ~Expansion();
  virtual auto main() -> void;
};

struct ExpansionPort {
  Node::Port port;
  auto load(Node::Object parent, Node::Object from) -> void;

  ExpansionPort(string_view name);
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;

  auto serialize(serializer&) -> void;

  const string name;
  unique_pointer<Expansion> device;
};

extern ExpansionPort expansionPort;

#include <sfc/expansion/satellaview/satellaview.hpp>
#include <sfc/expansion/21fx/21fx.hpp>
