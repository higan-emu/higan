namespace higan::Core {

struct System : Node {
};

}

namespace higan::Node {
  using System = shared_pointer<Core::System>;
}
