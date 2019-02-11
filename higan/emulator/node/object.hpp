namespace higan::Core {

//identifier is static, allowing template<typename T> to access via T::identifier
//identity() is virtual, allowing T* to access via T->identity()

#define DeclareClass(Type, Name) \
  static inline const string identifier = Name; \
  virtual auto identity() const -> string { return Name; } \
  static auto create() -> Node::Object { return new Type; } \
  private: static inline Class::Register<Type> register; public: \

struct Object : shared_pointer_this<Object> {
  DeclareClass(Object, "Object")

  Object(string name = {}) : name(name) {}
  virtual ~Object() = default;

  auto prepend(Node::Object node) -> Node::Object {
    if(auto found = find(node)) return found;
    nodes.prepend(node);
    node->parent = shared();
    PlatformAttach(node);
    return node;
  }

  template<typename T, typename... P> auto prepend(P&&... p) -> Node::Object {
    using Type = typename T::type;
    return prepend(shared_pointer<Type>::create(forward<P>(p)...));
  }

  auto append(Node::Object node) -> Node::Object {
    if(auto found = find(node)) return found;
    nodes.append(node);
    node->parent = shared();
    PlatformAttach(node);
    return node;
  }

  template<typename T, typename... P> auto append(P&&... p) -> Node::Object {
    using Type = typename T::type;
    return append(shared_pointer<Type>::create(forward<P>(p)...));
  }

  auto remove(Node::Object node) -> void {
    if(auto index = nodes.find(node)) {
      PlatformDetach(node);
      node->reset();
      node->parent.reset();
      nodes.remove(*index);
    }
  }

  auto reset() -> void {
    for(auto& node : nodes) {
      PlatformDetach(node);
      node->reset();
      node->parent.reset();
    }
    nodes.reset();
  }

  template<typename T> auto cast() -> shared_pointer<typename T::type> {
    if(dynamic_cast<typename T::type*>(this)) return shared();
    return {};
  }

  template<typename T> auto is() -> bool {
    return (bool)cast<T>();
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

  auto find(Node::Object source) -> Node::Object {
    if(!source) return {};
    for(auto& node : nodes) {
      if(node->identity() == source->identity() && node->name == source->name) return node;
    }
    return {};
  }

  template<typename T = Node::Object> auto find(string name) -> Node::Object {
    using Type = typename T::type;
    for(auto& node : nodes) {
      if(node->identity() == Type::identifier && node->name == name) return node;
    }
    return {};
  }

  template<typename T = string> auto property(const string& name) const -> T {
    if(auto property = properties.find(name)) {
      if(property->value.is<T>()) return property->value.get<T>();
    }
    return {};
  }

  template<typename T = string, typename U = string> auto setProperty(const string& name, const U& value = {}) -> void {
    if constexpr(is_same_v<T, string> && !is_same_v<U, string>) return setProperty(name, string{value});
    if(auto property = properties.find(name)) {
      if((const T&)value) property->value = (const T&)value;
      else properties.remove(*property);
    } else {
      if((const T&)value) properties.insert({name, (const T&)value});
    }
  }

  virtual auto serialize(string& output, string depth) -> void {
    output.append(depth, "node: ", identity(), "\n");
    output.append(depth, "  name: ", name, "\n");
    for(auto& property : properties) {
      if(!property.value.is<string>()) continue;
      output.append(depth, "  property\n");
      output.append(depth, "    name: ", property.name, "\n");
      output.append(depth, "    value: ", property.value.get<string>(), "\n");
    }
    depth.append("  ");
    for(auto& node : nodes) {
      node->serialize(output, depth);
    }
  }

  virtual auto unserialize(Markup::Node markup) -> void {
    if(!markup) return;
    name = markup["name"].text();
    properties.reset();
    for(auto& property : markup.find("property")) {
      properties.insert({property["name"].text(), property["value"].text()});
    }
    for(auto& leaf : markup.find("node")) {
      auto node = Class::create(leaf.text());
      append(node);
      node->unserialize(leaf);
    }
  }

  virtual auto load(Node::Object source) -> bool {
    if(!source || identity() != source->identity() || name != source->name) return false;
    properties = source->properties;
    return true;
  }

  auto save() -> string {
    string markup;
    serialize(markup, {});
    return markup;
  }

  auto begin() { return nodes.begin(); }
  auto end() { return nodes.end(); }

  string name;
  set<Property> properties;
  shared_pointer_weak<Object> parent;

private:
  vector<Node::Object> nodes;
};

}
