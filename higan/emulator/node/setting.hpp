namespace higan::Core::Setting {

struct Setting : Node {
  DeclareClass(Setting, "Setting")

  using Node::Node;

  auto serializeNode(string& output, string& depth) -> void override {
    Node::serializeNode(output, depth);
  }

  auto unserializeNode(Markup::Node node) -> void override {
    Node::unserializeNode(node);
  }
};

template<typename T> struct Abstract : Setting {
  DeclareClass(Abstract<T>, (
  is_same_v<T, boolean> ? "Setting::Boolean" :
  is_same_v<T, integer> ? "Setting::Integer" :
  is_same_v<T, natural> ? "Setting::Natural" :
  is_same_v<T, real>    ? "Setting::Real"    :
  is_same_v<T, string>  ? "Setting::String"  :
                          "Setting::Abstract"))

  Abstract(string name = {}, T value = {}, function<void (T)> modify = {}) : Setting(name), defaultValue(value) {
    this->currentValue = value;
    if constexpr(is_same_v<T, boolean>) allowedValues = {false, true};
    this->modify = modify;
  }

  auto latch() -> T {
    return latchedValue = currentValue;
  }

  auto value() const -> T {
    return currentValue;
  }

  auto& setValue(T value) {
    if(allowedValues && !allowedValues.find(value)) return *this;
    currentValue = value;
    if(modify) modify(value);
    return *this;
  }

  auto serializeNode(string& output, string& depth) -> void override {
    Setting::serializeNode(output, depth);
    output.append(depth, "currentValue: ", currentValue, "\n");
    output.append(depth, "latchedValue: ", latchedValue, "\n");
  }

  auto unserializeNode(Markup::Node node) -> void override {
    Setting::unserializeNode(node);
    if constexpr(is_same_v<T, boolean>) {
      currentValue = node["currentValue"].boolean();
      latchedValue = node["latchedValue"].boolean();
    }
    if constexpr(is_same_v<T, integer>) {
      currentValue = node["currentValue"].integer();
      latchedValue = node["latchedValue"].integer();
    }
    if constexpr(is_same_v<T, natural>) {
      currentValue = node["currentValue"].natural();
      latchedValue = node["latchedValue"].natural();
    }
    if constexpr(is_same_v<T, real>) {
      currentValue = node["currentValue"].real();
      latchedValue = node["latchedValue"].real();
    }
    if constexpr(is_same_v<T, string>) {
      currentValue = node["currentValue"].text();
      latchedValue = node["latchedValue"].text();
    }
  }

  const T defaultValue;
  T currentValue;
  T latchedValue;
  vector<T> allowedValues;

  function<void (T)> modify;
};

}
