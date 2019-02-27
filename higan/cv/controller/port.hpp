struct ControllerPort {
  //port.cpp
  ControllerPort(string_view name);
  auto load(Node::Object, Node::Object) -> void;
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;

  auto read() -> uint8 { if(device) return device->read(); return 0xff; }
  auto write(uint8 data) -> void { if(device) return device->write(data); }

  auto serialize(serializer&) -> void;

private:
  const string name;
  Node::Port port;
  unique_pointer<Controller> device;
  friend class Controller;
};

extern ControllerPort controllerPort1;
extern ControllerPort controllerPort2;
