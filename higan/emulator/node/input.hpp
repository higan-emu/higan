namespace higan::Core::Input {

struct Input : Node {
  using Node::Node;
};

struct Button : Input {
  using Input::Input;

  boolean value;
};

struct Axis : Input {
  integer value;
  integer minimum = -32768;
  integer maximum = +32767;
};

struct Trigger : Input {
  integer value;
  integer minimum =      0;
  integer maximum = +32767;
};

}

namespace higan::Node::Input {
  using Input   = shared_pointer<Core::Input::Input>;
  using Button  = shared_pointer<Core::Input::Button>;
  using Axis    = shared_pointer<Core::Input::Axis>;
  using Trigger = shared_pointer<Core::Input::Trigger>;
}
