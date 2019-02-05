namespace higan::Core {

struct Input : Object {
  DeclareClass(Input, "Input")
  using Object::Object;
};

struct Button : Input {
  DeclareClass(Button, "Button")
  using Input::Input;

  boolean value;
};

struct Axis : Input {
  DeclareClass(Axis, "Axis")
  using Input::Input;

  integer value;
  integer minimum = -32768;
  integer maximum = +32767;
};

struct Trigger : Input {
  DeclareClass(Trigger, "Trigger")
  using Input::Input;

  integer value;
  integer minimum =      0;
  integer maximum = +32767;
};

}
