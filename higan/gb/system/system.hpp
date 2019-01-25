enum class Input : uint {
  Up, Down, Left, Right, B, A, Select, Start,
};

struct System {
  enum class Model : uint {
    GameBoy,
    GameBoyColor,
    SuperGameBoy,
  };
  higan::Memory::Readable<uint8> bootROM;  //todo: no higan:: prefix

  inline auto loaded() const -> bool { return _loaded; }
  inline auto model() const -> Model { return _model; }
  inline auto clocksExecuted() const -> uint { return _clocksExecuted; }

  auto run() -> void;
  auto runToSave() -> void;

  auto init() -> void;
  auto load(Interface*, Model, maybe<uint> = nothing) -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;

  //video.cpp
  auto configureVideoPalette() -> void;
  auto configureVideoEffects() -> void;

  //serialization.cpp
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

  auto serialize(serializer&) -> void;
  auto serializeAll(serializer&) -> void;
  auto serializeInit() -> void;

  Interface* interface = nullptr;

  struct Information {
    string manifest;
  } information;

  bool _loaded = false;
  Model _model = Model::GameBoy;
  uint _serializeSize = 0;
  uint _clocksExecuted = 0;
};

#include <gb/interface/interface.hpp>

extern System system;

auto Model::GameBoy() -> bool { return system.model() == System::Model::GameBoy; }
auto Model::GameBoyColor() -> bool { return system.model() == System::Model::GameBoyColor; }
auto Model::SuperGameBoy() -> bool { return system.model() == System::Model::SuperGameBoy; }
