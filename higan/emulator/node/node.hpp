#include <emulator/node/property.hpp>

namespace higan::Object {

//higan::Object namespace should never be accessed outside of higan/emulator/node/
//higan::Node namespace should always be used instead for reference-counted Objects

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

#define DeclareClass(Type, Identifier) \
  static inline const string identifier = Identifier; \
  virtual auto type() const -> string { return identifier; } \
  static auto create() -> shared_pointer<Node> { return new Type; } \
  private: static inline Class::Register<Type> register; public: \

struct Node : shared_pointer_this<Node> {
  DeclareClass(Node, "Node")

  Node(string name = {}) : name(name) {}
  virtual ~Node() = default;

  auto reset() -> void {
    for(auto& node : nodes) node->parent.reset();
    nodes.reset();
  }

  auto prepend(shared_pointer<Node> node) -> shared_pointer<Node> {
    if(auto found = find(node)) return found;
    nodes.prepend(node);
    node->parent = shared();
    return node;
  }

  template<typename T, typename... P> auto prepend(P&&... p) -> shared_pointer<Node> {
    using Type = typename T::type;
    return prepend(shared_pointer<Type>::create(forward<P>(p)...));
  }

  auto append(shared_pointer<Node> node) -> shared_pointer<Node> {
    if(auto found = find(node)) return found;
    nodes.append(node);
    node->parent = shared();
    return node;
  }

  template<typename T, typename... P> auto append(P&&... p) -> shared_pointer<Node> {
    using Type = typename T::type;
    return append(shared_pointer<Type>::create(forward<P>(p)...));
  }

  auto remove(shared_pointer<Node> node) -> void {
    if(auto index = nodes.find(node)) {
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

  template<typename T> auto find(uint index) -> shared_pointer<typename T::type> {
    auto result = find<T>();
    if(index < result.size()) return result[index];
    return {};
  }

  auto find(shared_pointer<Node> source) -> shared_pointer<Node> {
    if(!source) return {};
    for(auto& node : nodes) {
      if(node->type() == source->type() && node->name == source->name) return node;
    }
    return {};
  }

  template<typename T> auto find(string name) -> shared_pointer<Node> {
    using Type = typename T::type;
    for(auto& node : nodes) {
      if(node->type() == Type::identifier && node->name == name) return node;
    }
    return {};
  }

  auto property(string name) const -> string {
    if(auto property = properties.find(name)) return property->value;
    return {};
  }

  auto setProperty(string name, string value = {}) -> void {
    if(auto property = properties.find(name)) {
      if(value) property->value = value;
      else properties.remove(*property);
    } else {
      if(value) properties.insert({name, value});
    }
  }

  virtual auto serialize(string& output, string depth) -> void {
    output.append(depth, "node\n");
    output.append(depth, "  type: ", type(), "\n");
    output.append(depth, "  name: ", name, "\n");
    for(auto& property : properties) {
      output.append(depth, "  property\n");
      output.append(depth, "    name: ", property.name, "\n");
      output.append(depth, "    value: ", property.value, "\n");
    }
    depth.append("  ");
    for(auto& node : nodes) {
      node->serialize(output, depth);
    }
  }

  virtual auto unserialize(Markup::Node markup) -> void {
    name = markup["name"].text();
    properties.reset();
    for(auto& property : markup.find("property")) {
      properties.insert({property["name"].text(), property["value"].text()});
    }
    for(auto& leaf : markup.find("node")) {
      auto node = Class::create(leaf["type"].text());
      append(node);
      node->unserialize(leaf);
    }
  }

  virtual auto copy(shared_pointer<Node> source) -> void {
    name = source->name;
    properties = source->properties;
    for(auto& from : source->nodes) {
      auto type = from->type();
      auto offset = from->offset();
      if(!offset) continue;

      bool found = false;
      for(auto& to : nodes) {
        auto index = to->offset();
        if(!index) continue;

        if(type == to->type() && *offset == *index) {
          found = true;
          to->copy(from);
          break;
        }
      }
    }
  }

  //retrieve a child node from the root node using a string
  auto path(string path) const -> shared_pointer<Node> {
    auto part = path.split("/", 1L);
    auto side = part[0].split(",", 1L);
    uint index = 0;
    uint match = side[0].natural();
    auto type = side(1);
    for(auto& node : nodes) {
      if(node->type() != type) continue;
      if(index == match && !part(1)) return node;
      if(index == match) return node->path(part[1]);
      index++;
    }
    return {};
  }

  //retrieve a string to locate a child node from the root node
  auto path() const -> string {
    if(auto parent = this->parent) {
      if(auto acquired = parent.acquire()) {
        if(auto offset = this->offset()) {
          return {acquired->path(), *offset, ",", type(), "/"};
        }
      }
    }
    return {};
  }

  //returns the Nth occurrence of this node's type in its parent node list
  //this is done so that exact ordering of nodes isn't required: only exact type ordering
  auto offset() const -> maybe<uint> {
    if(auto parent = this->parent) {
      if(auto acquired = parent.acquire()) {
        if(auto self = shared()) {
          auto type = this->type();
          uint offset = 0;
          for(auto& node : acquired->nodes) {
            if(node->type() != type) continue;
            if(node == self) return offset;
            offset++;
          }
        }
      }
    }
    return nothing;
  }

  auto begin() { return nodes.begin(); }
  auto end() { return nodes.end(); }

  string name;
  set<Property> properties;
  shared_pointer_weak<Node> parent;

private:
  vector<shared_pointer<Node>> nodes;
};

}

#include <emulator/node/system.hpp>
#include <emulator/node/video.hpp>
#include <emulator/node/audio.hpp>
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

struct Node : shared_pointer<Object::Node> {
  using shared_pointer<Object::Node>::shared_pointer;
  Node(shared_pointer<Object::Node> source) : shared_pointer(source) {}

  using System     = shared_pointer<Object::System>;
  using Video      = shared_pointer<Object::Video>;
  using Audio      = shared_pointer<Object::Audio>;
  using Peripheral = shared_pointer<Object::Peripheral>;
  using Port       = shared_pointer<Object::Port>;

  struct Input : shared_pointer<Object::Input::Input> {
    using shared_pointer<Object::Input::Input>::shared_pointer;
    Input(shared_pointer<Object::Input::Input> source) : shared_pointer(source) {}

    using Button  = shared_pointer<Object::Input::Button>;
    using Axis    = shared_pointer<Object::Input::Axis>;
    using Trigger = shared_pointer<Object::Input::Trigger>;
  };

  struct Setting : shared_pointer<Object::Setting::Setting> {
    using shared_pointer<Object::Setting::Setting>::shared_pointer;
    Setting(shared_pointer<Object::Setting::Setting> source) : shared_pointer(source) {}

    using Boolean = shared_pointer<Object::Setting::Boolean>;
  };

  static inline auto create(string identifier) -> shared_pointer<Object::Node> {
    return Object::Class::create(identifier);
  }

  static inline auto serialize(shared_pointer<Object::Node> node) -> string {
    string result;
    node->serialize(result, {});
    return result;
  }

  static inline auto unserialize(string markup) -> shared_pointer<Object::Node> {
    auto document = BML::unserialize(markup);
    auto node = Object::Class::create(document["node/type"].text());
    node->unserialize(document["node"]);
    return node;
  }
};

}
