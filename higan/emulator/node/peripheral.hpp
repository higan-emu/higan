namespace higan::Core {

struct Peripheral : Node {
};

}

namespace higan::Node {
  using Peripheral = shared_pointer<Core::Peripheral>;
}
