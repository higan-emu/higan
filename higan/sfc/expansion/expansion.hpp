struct Expansion : Thread {
  Expansion();
  virtual ~Expansion();
  virtual auto main() -> void;
};

struct ExpansionPort {
  static auto create(string_view name) -> Node::Port;

  Node::Port port;
  auto initialize(Node::Object) -> void;
  auto bind(Node::Port) -> void;

  ExpansionPort(string_view name);
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;

  auto serialize(serializer&) -> void;

  const string name;
private:
  Expansion* device = nullptr;
};

extern ExpansionPort expansionPort;

#include <sfc/expansion/satellaview/satellaview.hpp>
#include <sfc/expansion/21fx/21fx.hpp>
