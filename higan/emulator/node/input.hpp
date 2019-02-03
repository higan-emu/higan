namespace higan::Object::Input {

struct Input : Node {
  DeclareClass(Input, "Input")

  using Node::Node;
};

struct Button : Input {
  DeclareClass(Button, "Input.Button")

  using Input::Input;

  boolean value;
};

struct Axis : Input {
  DeclareClass(Axis, "Input.Axis")

  using Input::Input;

  integer value;
  integer minimum = -32768;
  integer maximum = +32767;
};

struct Trigger : Input {
  DeclareClass(Trigger, "Input.Trigger")

  using Input::Input;

  integer value;
  integer minimum =      0;
  integer maximum = +32767;
};

}
