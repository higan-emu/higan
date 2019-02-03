#include <emulator/node/property.hpp>

namespace higan::Core {

struct Node : shared_pointer_this<Node> {
  static auto uniqueID(uint64_t initialize = 0) -> uint64_t {
    static uint64_t counter = 0;
    if(initialize) return counter = initialize;
    return counter++;
  }

  Node(string name = {}) : id(uniqueID()), name(name) {
  }

  virtual ~Node() {
  }

  auto reset() -> void {
    if(nodes && detach) for(auto& node : nodes) detach(node);
    nodes.reset();
  }

  auto assign(shared_pointer<Node> node) -> void {
    if(nodes && detach) for(auto& node : nodes) detach(node);
    nodes.reset();
    nodes.append(node);
    if(attach) attach(node);
  }

  auto append(shared_pointer<Node> node) -> void {
    nodes.append(node);
    if(attach) attach(node);
  }

  auto remove(shared_pointer<Node> node) -> void {
    if(auto index = nodes.find(node)) {
      if(detach) detach(nodes[*index]);
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
    output.append(depth, "node\n");
    output.append(depth, "  id: ", id, "\n");
    output.append(depth, "  type: ", type, "\n");
    output.append(depth, "  name: ", name, "\n");
    for(auto& property : properties) {
      output.append(depth, "  property\n");
      output.append(depth, "    name: ", property.name, "\n");
      output.append(depth, "    value: ", property.value, "\n");
    }
    depth.append("  ");
    for(auto node : nodes) output.append(node->serialize(depth));
    return output;
  }

  const uint64_t id;
  bool edge = false;
  string type;
  string name;
  set<Property> properties;
  vector<shared_pointer<Node>> nodes;
  vector<shared_pointer<Node>> list;

  function<void (shared_pointer<Node>)> attach;
  function<void (shared_pointer<Node>)> detach;
};

}

namespace higan::Node {
  using Node = shared_pointer<Core::Node>;
}

#include <emulator/node/system.hpp>
#include <emulator/node/cartridge.hpp>
#include <emulator/node/peripheral.hpp>
#include <emulator/node/port.hpp>
#include <emulator/node/input.hpp>
#include <emulator/node/setting.hpp>
