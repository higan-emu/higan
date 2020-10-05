struct ExpansionPort {
  Node::Port port;
  unique_pointer<Expansion> device;

  ExpansionPort(string name);
  auto load(Node::Object parent) -> void;
  auto unload() -> void;
  auto allocate(string name) -> Node::Peripheral;

  auto connected() -> bool { return device ? true : false; }
  auto romcs() -> bool { return device ? device->romcs() : false; }
  auto mapped(uint16 address, bool io) -> bool { return device ? device->mapped(address, io) : false; }
  auto read(uint16 address) -> uint8 { return device ? device->read(address) : (uint8)0xff; }
  auto write(uint16 address, uint8 data) -> void { if (device) device->write(address, data); }
  auto in(uint16 address) -> uint8 { return device ? device->in(address) : (uint8)0xff;}
  auto out(uint16 address, uint8 data) -> void { if (device) device->out(address, data); }

  auto serialize(serializer&) -> void;

  const string name;
};

extern ExpansionPort expansionPort;
