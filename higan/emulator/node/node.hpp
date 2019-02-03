#include <emulator/node/property.hpp>

namespace higan::Core {

struct Node;

//horrible implementation of run-time introspection:
//allow converting a unique class string to a derived Node type.

struct Class {
  struct Instance {
    const string identifier;
    const function<shared_pointer<Node> ()> create;
  };

  static auto classes() -> vector<Instance>& {
    static vector<Instance> classes;
    return classes;
  }

  template<typename T> static auto register() -> void {
    if(!classes().find([&](auto instance) { return instance.identifier == T::identifier; })) {
      classes().append({T::identifier, &T::create});
    } else {
      throw;
    }
  }

  static auto create(string identifier) -> shared_pointer<Node> {
    if(auto index = classes().find([&](auto instance) { return instance.identifier == identifier; })) {
      return classes()[*index].create();
    }
    if(identifier == "Node") throw;
    return create("Node");
  }

  template<typename T> struct Register {
    Register() { Class::register<T>(); }
  };
};

#define DeclareClass(Type, Identity) \
  static inline const string identifier = Identity; \
  virtual auto identity() const -> string { return identifier; } \
  static auto create() -> shared_pointer<Node> { return new Type; } \
  private: static inline Class::Register<Type> register; public: \

struct Node : shared_pointer_this<Node> {
  DeclareClass(Node, "Node")

  Node(string name = {}) : name(name) {}
  virtual ~Node() = default;

  auto reset() -> void {
    if(nodes && detach) for(auto& node : nodes) detach(node);
    for(auto& node : nodes) node->parent.reset();
    nodes.reset();
  }

  template<typename T = Node> auto append(shared_pointer<T> node) -> shared_pointer<T> {
    nodes.append(node);
    node->parent = shared();
    if(attach) attach(node);
    return node;
  }

  auto remove(shared_pointer<Node> node) -> void {
    if(auto index = nodes.find(node)) {
      if(detach) detach(nodes[*index]);
      node->parent.reset();
      nodes.remove(*index);
    }
  }

  template<typename T> auto cast() -> shared_pointer<typename T::type> {
    if(dynamic_cast<typename T::type*>(this)) return shared();
    return {};
  }

  template<typename T> auto find() -> vector<shared_pointer<typename T::type>> {
    vector<shared_pointer<typename T::type>> result;
    if(dynamic_cast<typename T::type*>(this)) {
      if(auto instance = shared()) result.append(instance);
    }
    for(auto& node : nodes) result.append(node->find<T>());
    return result;
  }

  auto first() -> shared_pointer<Node> {
    if(nodes) return nodes.first();
    return {};
  }

  auto property(string name) const -> string {
    if(auto property = properties.find(name)) return property->value;
    return {};
  }

  auto setProperty(string name, string value) -> void {
    if(auto property = properties.find(name)) {
      if(value) property->value = value;
      else properties.remove(*property);
    } else {
      if(value) properties.insert({name, value});
    }
  }

  auto serialize(string depth = "") -> string {
    string output;
    output.append(depth, "node: ", identity(), "\n");
    depth.append("  ");
    serializeNode(output, depth);
    for(auto& property : properties) {
      output.append(depth, "property\n");
      output.append(depth, "  name: ", property.name, "\n");
      output.append(depth, "  value: ", property.value, "\n");
    }
    for(auto node : nodes) output.append(node->serialize(depth));
    return output;
  }

  virtual auto serializeNode(string& output, string& depth) -> void {
    output.append(depth, "name: ", name, "\n");
  }

  auto unserialize(string markup) -> void {
    unserialize(BML::unserialize(markup)["node"]);
  }

  auto unserialize(Markup::Node markup) -> void {
    if(name != markup["name"].text()) return;

    unserializeNode(markup);
    properties.reset();
    for(auto& property : markup.find("property")) {
      properties.insert({property["name"].text(), property["value"].text()});
    }
    if(import) import(markup);
    for(auto& leaf : markup.find("node")) {
      for(auto& node : nodes) {
        if(node->name == leaf["name"].text()) {
          node->unserialize(leaf);
        }
      }
    }
  }

  virtual auto unserializeNode(Markup::Node markup) -> void {
    name = markup["name"].text();
  }

  auto find(string path) const -> shared_pointer<Node> {
    auto part = path.split("/", 1L);
    auto index = part[0].natural();
    if(index >= nodes.size()) return {};
    if(part.size() == 1 || !part[1]) return nodes[index];
    return nodes[index]->find(part[1]);
  }

  auto path() const -> string {
    if(auto parent = this->parent) {
      if(auto acquired = parent.acquire()) {
        if(auto self = shared()) {
          uint index = 0;
          for(auto& node : acquired->nodes) {
            if(node == self) return {acquired->path(), index, "/"};
            index++;
          }
        }
      }
    }
    return {};
  }

  auto begin() { return nodes.begin(); }
  auto end() { return nodes.end(); }

  string name;
  string kind;
  set<Property> properties;
  vector<shared_pointer<Node>> list;
  shared_pointer_weak<Node> parent;

  function<void (shared_pointer<Node>)> attach;
  function<void (shared_pointer<Node>)> detach;
  function<void (Markup::Node)> import;

private:
  vector<shared_pointer<Node>> nodes;
};

}

#include <emulator/node/system.hpp>
#include <emulator/node/cartridge.hpp>
#include <emulator/node/peripheral.hpp>
#include <emulator/node/port.hpp>
#include <emulator/node/input.hpp>
#include <emulator/node/setting.hpp>

namespace higan {

//if we were to use simple namespaces and using statements alone, we would end up with:
//Node::Node, Node::Port::Port, Node::Input::Input, etc.

//in order to remove the repetition from the base types, nested structs are needed instead:
//Node, Node::Port, Node::Input, etc.

//this has the downside of sealing off higan::Node from additional types being added here.
//however, in practice, the platform layer should not be casting to emulator-specific types.
//any nodes the platform layer needs should be abstractly pulled into emulator/node.hpp instead.

struct Node : shared_pointer<Core::Node> {
  using shared_pointer<Core::Node>::shared_pointer;
  Node(shared_pointer<Core::Node> source) : shared_pointer(source) {}

  using System     = shared_pointer<Core::System>;
  using Cartridge  = shared_pointer<Core::Cartridge>;
  using Peripheral = shared_pointer<Core::Peripheral>;

  struct Port : shared_pointer<Core::Port::Port> {
    using shared_pointer<Core::Port::Port>::shared_pointer;
    Port(shared_pointer<Core::Port::Port> source) : shared_pointer(source) {}

    using Video      = shared_pointer<Core::Port::Video>;
    using Audio      = shared_pointer<Core::Port::Audio>;
    using Cartridge  = shared_pointer<Core::Port::Cartridge>;
    using Peripheral = shared_pointer<Core::Port::Peripheral>;
  };

  struct Input : shared_pointer<Core::Input::Input> {
    using shared_pointer<Core::Input::Input>::shared_pointer;
    Input(shared_pointer<Core::Input::Input> source) : shared_pointer(source) {}

    using Button  = shared_pointer<Core::Input::Button>;
    using Axis    = shared_pointer<Core::Input::Axis>;
    using Trigger = shared_pointer<Core::Input::Trigger>;
  };

  struct Setting : shared_pointer<Core::Setting::Setting> {
    using shared_pointer<Core::Setting::Setting>::shared_pointer;
    Setting(shared_pointer<Core::Setting::Setting> source) : shared_pointer(source) {}

    using Boolean = shared_pointer<Core::Setting::Abstract<boolean>>;
    using Integer = shared_pointer<Core::Setting::Abstract<integer>>;
    using Natural = shared_pointer<Core::Setting::Abstract<natural>>;
    using Real    = shared_pointer<Core::Setting::Abstract<real>>;
    using String  = shared_pointer<Core::Setting::Abstract<string>>;
  };

  static inline auto create(string identifier) -> shared_pointer<Core::Node> {
    return Core::Class::create(identifier);
  }
};

}
