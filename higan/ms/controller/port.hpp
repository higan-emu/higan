struct ControllerPort {
  Node::Port port;
  auto load(Node::Object, Node::Object) -> void;

  ControllerPort(string_view name);
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;

  auto read() -> uint8 { if(device) return device->read(); return 0xff; }
  auto write(uint8 data) -> void { if(device) return device->write(data); }

  auto serialize(serializer&) -> void;

  const string name;
private:
  unique_pointer<Controller> device;
  friend class Controller;
};

extern ControllerPort controllerPort1;
extern ControllerPort controllerPort2;
