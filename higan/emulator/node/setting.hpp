namespace higan::Core {

struct Settings : Object {
  DeclareClass(Settings, "Settings")
  using Object::Object;
};

struct Setting : Object {
  DeclareClass(Setting, "Setting")
  using Object::Object;

  virtual auto getValue() const -> string { return {}; }
  virtual auto getLatch() const -> string { return {}; }
};

template<typename Cast, typename Type> struct Abstract : Setting {
  using Setting::Setting;

  Abstract(string name = {}, Type value = {}, function<void (Type)> modify = {}) : Setting(name), defaultValue(value) {
    this->currentValue = value;
    if constexpr(is_same_v<Type, boolean>) this->allowedValues = {false, true};
    this->modify = modify;
  }

  auto value() const -> Type {
    return currentValue;
  }

  auto latch() const -> Type {
    return latchedValue;
  }

  auto getValue() const -> string override { return value(); }
  auto getLatch() const -> string override { return latch(); }

  auto& setValue(Type value) {
    if(allowedValues && !allowedValues.find(value)) return *this;
    currentValue = value;
    if(modify) modify(value);
    return *this;
  }

  auto& setLatch() {
    latchedValue = currentValue;
    return *this;
  }

  auto serialize(string& output, string depth) -> void override {
    Setting::serialize(output, depth);
    output.append(depth, "  value: ", currentValue, "\n");
    if(latchedValue) output.append(depth, "  latch: ", latchedValue, "\n");
  }

  auto unserialize(Markup::Node node) -> void override {
    Setting::unserialize(node);
    if constexpr(is_same_v<Type, boolean>) currentValue = node["value"].boolean(), latchedValue = node["latch"].boolean();
    if constexpr(is_same_v<Type, natural>) currentValue = node["value"].natural(), latchedValue = node["latch"].natural();
    if constexpr(is_same_v<Type, integer>) currentValue = node["value"].integer(), latchedValue = node["latch"].integer();
    if constexpr(is_same_v<Type, real>)    currentValue = node["value"].real(),    latchedValue = node["latch"].real();
    if constexpr(is_same_v<Type, string>)  currentValue = node["value"].text(),    latchedValue = node["latch"].text();
  }

  auto copy(Node::Object node) -> bool override {
    if(!Setting::copy(node)) return false;
    if(auto source = node->cast<shared_pointer<Cast>>()) {
      currentValue = source->currentValue;
      latchedValue = source->latchedValue;
    }
    return true;
  }

  const Type defaultValue;
  Type currentValue;
  Type latchedValue;
  vector<Type> allowedValues;

  function<void (Type)> modify;
};

struct Boolean : Abstract<Boolean, boolean> {
  DeclareClass(Boolean, "Setting.Boolean")
  using Abstract::Abstract;
};

struct Natural : Abstract<Natural, natural> {
  DeclareClass(Natural, "Setting.Natural")
  using Abstract::Abstract;
};

struct Integer : Abstract<Integer, integer> {
  DeclareClass(Integer, "Setting.Integer")
  using Abstract::Abstract;
};

struct Real : Abstract<Real, real> {
  DeclareClass(Real, "Setting.Real")
  using Abstract::Abstract;
};

struct String : Abstract<String, string> {
  DeclareClass(String, "Setting.String")
  using Abstract::Abstract;
};

}
