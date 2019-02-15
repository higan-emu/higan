#if defined(CORE_GB)

namespace higan::GameBoy {

extern Interface* interface;

struct AbstractInterface : Interface {
  auto root() -> Node::Object override;
  auto load(string tree = {}) -> void override;
  auto unload() -> void;
  auto save() -> void override;
  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheats(const vector<string>&) -> void override;
};

struct GameBoyInterface : AbstractInterface {
  auto name() -> string override { return "Game Boy"; }
};

struct GameBoyColorInterface : AbstractInterface {
  auto name() -> string override { return "Game Boy Color"; }
};

struct SuperGameBoyInterface {
  virtual auto audio(const double* samples, uint channels) -> void = 0;
  virtual auto input() -> uint8 = 0;

  virtual auto lcdScanline() -> void = 0;
  virtual auto lcdOutput(uint2 color) -> void = 0;
  virtual auto joypRead() -> uint4 = 0;
  virtual auto joypWrite(bool p15, bool p14) -> void = 0;
};

extern SuperGameBoyInterface* superGameBoy;

}

#endif
