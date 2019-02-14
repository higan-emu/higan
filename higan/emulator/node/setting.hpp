namespace higan::Core {

struct Settings : Object {
  DeclareClass(Settings, "Settings")
  using Object::Object;
};

struct Setting : Object {
  DeclareClass(Setting, "Setting")
  using Object::Object;

  virtual auto setLatch() -> void {}

  virtual auto readValue() const -> string { return {}; }
  virtual auto readLatch() const -> string { return {}; }
  virtual auto readAllowedValues() const -> vector<string> { return {}; }
  virtual auto writeValue(string value) -> void {}

  auto serialize(string& output, string depth) -> void override {
    Object::serialize(output, depth);
    output.append(depth, "  dynamic: ", dynamic, "\n");
  }

  auto unserialize(Markup::Node node) -> void override {
    Object::unserialize(node);
    dynamic = node["dynamic"].boolean();
  }

  bool dynamic = false;
};

template<typename Cast, typename Type> struct Abstract : Setting {
  using Setting::Setting;

  Abstract(string name = {}, Type value = {}, function<void (Type)> modify = {}) : Setting(name), defaultValue(value) {
    this->currentValue = value;
    this->latchedValue = value;
    if constexpr(is_same_v<Type, boolean>) this->allowedValues = {false, true};
    this->modify = modify;
  }

  auto value() const -> Type {
    return currentValue;
  }

  auto latch() const -> Type {
    return latchedValue;
  }

  auto setValue(Type value) -> void {
    if(allowedValues && !allowedValues.find(value)) return;
    currentValue = value;
    if(dynamic) latchedValue = currentValue;
    if(modify) modify(value);
  }

  auto setLatch() -> void override {
    latchedValue = currentValue;
  }

  auto readValue() const -> string override { return value(); }
  auto readLatch() const -> string override { return latch(); }
  auto readAllowedValues() const -> vector<string> override {
    vector<string> values;
    for(auto& value : allowedValues) values.append(value);
    return values;
  }

  auto writeValue(string value) -> void override {
    if(allowedValues && !readAllowedValues().find(value)) return;
    if constexpr(is_same_v<Type, boolean>) currentValue = value.boolean();
    if constexpr(is_same_v<Type, natural>) currentValue = value.natural();
    if constexpr(is_same_v<Type, integer>) currentValue = value.integer();
    if constexpr(is_same_v<Type, real   >) currentValue = value.real();
    if constexpr(is_same_v<Type, string >) currentValue = value;
    if(dynamic) latchedValue = currentValue;
    if(modify) modify(currentValue);
  }

  auto load(Node::Object source) -> bool override {
    if(!Setting::load(source)) return false;
    if(auto setting = source->cast<shared_pointer<Cast>>()) {
      if(allowedValues.find(setting->currentValue)) {
        currentValue = setting->currentValue;
        latchedValue = currentValue;
      }
    }
    return true;
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
