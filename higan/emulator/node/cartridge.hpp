namespace higan::Core {

struct Cartridge : Node {
};

}

namespace higan::Node {
  using Cartridge = shared_pointer<Core::Cartridge>;
}
