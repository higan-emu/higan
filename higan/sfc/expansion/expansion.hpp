struct Expansion : Thread {
  Expansion();
  virtual ~Expansion();
  static auto Enter() -> void;
  virtual auto main() -> void;
};

struct ExpansionPort {
  Node::Port::Peripheral port;
  auto initialize(Node) -> void;

  auto connect(uint deviceID) -> void;

  auto power() -> void;
  auto unload() -> void;
  auto serialize(serializer&) -> void;

  Expansion* device = nullptr;
};

extern ExpansionPort expansionPort;

#include <sfc/expansion/satellaview/satellaview.hpp>
#include <sfc/expansion/21fx/21fx.hpp>
