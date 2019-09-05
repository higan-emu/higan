struct System {
  Node::Object node;

  enum class Model : uint { PCEngine, SuperGrafx };

  inline auto model() const -> Model { return information.model; }
  inline auto colorburst() const -> double { return information.colorburst; }

  //system.cpp
  auto run() -> void;
  auto runToSave() -> void;

  auto load(Node::Object&, Node::Object) -> void;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;

  //serialization.cpp
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

private:
  struct Information {
    Model model = Model::PCEngine;
    double colorburst = Constants::Colorburst::NTSC;
    uint32 serializeSize;
  } information;

  //serialization.cpp
  auto serializeInit() -> void;
  auto serializeAll(serializer&) -> void;
  auto serialize(serializer&) -> void;
};

extern System system;

auto Model::PCEngine() -> bool { return system.model() == System::Model::PCEngine; }
auto Model::SuperGrafx() -> bool { return system.model() == System::Model::SuperGrafx; }
