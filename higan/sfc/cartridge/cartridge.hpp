struct Cartridge {
  Node::Port::Cartridge port;
  auto initialize(Node) -> void;
  auto import(Markup::Node) -> void;

  auto pathID() const -> uint { return information.pathID; }
  auto region() const -> string { return information.region; }

  //cartridge.cpp

  auto hashes() const -> vector<string>;
  auto manifests() const -> vector<string>;
  auto titles() const -> vector<string>;

  auto load() -> bool;
  auto save() -> void;
  auto unload() -> void;

  auto serialize(serializer&) -> void;

  ReadableMemory rom;
  WritableMemory ram;

  struct Information {
    uint pathID = 0;
    string region;
    string sha256;
  } information;

  struct Has {
    boolean ICD;
    boolean MCC;
    boolean DIP;
    boolean Event;
    boolean SA1;
    boolean SuperFX;
    boolean ARMDSP;
    boolean HitachiDSP;
    boolean NECDSP;
    boolean EpsonRTC;
    boolean SharpRTC;
    boolean SPC7110;
    boolean SDD1;
    boolean OBC1;
    boolean MSU1;

    boolean GameBoySlot;
    boolean BSMemorySlot;
    boolean SufamiTurboSlotA;
    boolean SufamiTurboSlotB;
  } has;

private:
  Game game;
  Game slotGameBoy;
  Game slotBSMemory;
  Game slotSufamiTurboA;
  Game slotSufamiTurboB;
  Markup::Node board;

  //cartridge.cpp
  auto loadGameBoy() -> bool;
  auto loadBSMemory() -> bool;

  //load.cpp
  auto loadBoard(string) -> Markup::Node;
  auto loadCartridge(Markup::Node) -> void;
  auto loadMemory(Memory&, Markup::Node, bool required) -> void;
  template<typename T> auto loadMap(Markup::Node, T&) -> uint;
  auto loadMap(Markup::Node, const function<uint8 (uint24, uint8)>&, const function<void (uint24, uint8)>&) -> uint;

  auto loadROM(Markup::Node) -> void;
  auto loadRAM(Markup::Node) -> void;
  auto loadICD(Markup::Node) -> void;
  auto loadMCC(Markup::Node) -> void;
  auto loadBSMemory(Markup::Node) -> void;
  auto loadSufamiTurboA(Markup::Node) -> void;
  auto loadSufamiTurboB(Markup::Node) -> void;
  auto loadDIP(Markup::Node) -> void;
  auto loadEvent(Markup::Node) -> void;
  auto loadSA1(Markup::Node) -> void;
  auto loadSuperFX(Markup::Node) -> void;
  auto loadARMDSP(Markup::Node) -> void;
  auto loadHitachiDSP(Markup::Node, uint roms) -> void;
  auto loaduPD7725(Markup::Node) -> void;
  auto loaduPD96050(Markup::Node) -> void;
  auto loadEpsonRTC(Markup::Node) -> void;
  auto loadSharpRTC(Markup::Node) -> void;
  auto loadSPC7110(Markup::Node) -> void;
  auto loadSDD1(Markup::Node) -> void;
  auto loadOBC1(Markup::Node) -> void;
  auto loadMSU1() -> void;

  //save.cpp
  auto saveCartridge(Markup::Node) -> void;
  auto saveMemory(Memory&, Markup::Node) -> void;

  auto saveRAM(Markup::Node) -> void;
  auto saveMCC(Markup::Node) -> void;
  auto saveSA1(Markup::Node) -> void;
  auto saveSuperFX(Markup::Node) -> void;
  auto saveARMDSP(Markup::Node) -> void;
  auto saveHitachiDSP(Markup::Node) -> void;
  auto saveuPD7725(Markup::Node) -> void;
  auto saveuPD96050(Markup::Node) -> void;
  auto saveEpsonRTC(Markup::Node) -> void;
  auto saveSharpRTC(Markup::Node) -> void;
  auto saveSPC7110(Markup::Node) -> void;
  auto saveOBC1(Markup::Node) -> void;

  friend class Interface;
  friend class ICD;
};

extern Cartridge cartridge;
