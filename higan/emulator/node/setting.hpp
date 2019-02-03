namespace higan::Core::Setting {

struct Setting : Node {
  using Node::Node;
};

template<typename T>
struct Abstract : Setting {
  using type = T;

  Abstract(string name, T value, function<void (T)> modify = {}) : Setting(name) {
    this->currentValue = value;
    this->defaultValue = value;
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

  T currentValue;
  T defaultValue;
  T latchedValue;
  vector<T> allowedValues;

  function<void (T)> modify;
};

}

namespace higan::Node::Setting {
  using Setting = shared_pointer<Core::Setting::Setting>;
  using Boolean = shared_pointer<Core::Setting::Abstract<boolean>>;
  using Integer = shared_pointer<Core::Setting::Abstract<integer>>;
  using Natural = shared_pointer<Core::Setting::Abstract<natural>>;
  using Real    = shared_pointer<Core::Setting::Abstract<real>>;
  using String  = shared_pointer<Core::Setting::Abstract<string>>;
}
