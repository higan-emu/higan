#pragma once

namespace higan {

struct AbstractSetting {
  inline auto serialize(string depth = "") const -> string;

  virtual auto isNothing() const -> bool { return false; }
  virtual auto isBoolean() const -> bool { return false; }
  virtual auto isInteger() const -> bool { return false; }
  virtual auto isNatural() const -> bool { return false; }
  virtual auto isReal()    const -> bool { return false; }
  virtual auto isString()  const -> bool { return false; }

  virtual auto getBoolean() const -> boolean { throw; }
  virtual auto getInteger() const -> integer { throw; }
  virtual auto getNatural() const -> natural { throw; }
  virtual auto getReal()    const -> real    { throw; }
  virtual auto getString()  const -> string  { throw; }

  AbstractSetting& self = *this;
  string name;
  vector<AbstractSetting*> children;
};

template<typename T = nothing_t>
struct Setting : AbstractSetting {
  using type = T;

  Setting() {}
  Setting(string name, T value = {}) {
    this->name = name;
    this->value = value;
  }
  Setting(AbstractSetting& parent, string name, T value = {}) {
    parent.children.append(this);
    this->name = name;
    this->value = value;
  }

  auto operator()() -> T& { return value; }

  auto isNothing() const -> bool override { return is_same_v<T, nothing_t>; }
  auto isBoolean() const -> bool override { return is_same_v<T, boolean>;   }
  auto isInteger() const -> bool override { return is_same_v<T, integer>;   }
  auto isNatural() const -> bool override { return is_same_v<T, natural>;   }
  auto isReal()    const -> bool override { return is_same_v<T, real>;      }
  auto isString()  const -> bool override { return is_same_v<T, string>;    }

  auto getBoolean() const -> boolean override { if constexpr(is_same_v<T, boolean>) return value; throw; }
  auto getInteger() const -> integer override { if constexpr(is_same_v<T, integer>) return value; throw; }
  auto getNatural() const -> natural override { if constexpr(is_same_v<T, natural>) return value; throw; }
  auto getReal()    const -> real    override { if constexpr(is_same_v<T, real>   ) return value; throw; }
  auto getString()  const -> string  override { if constexpr(is_same_v<T, string> ) return value; throw; }

  T value;
};

auto AbstractSetting::serialize(string depth) const -> string {
  string output;

  if(name) {
    output.append(depth, name);
    if(isBoolean()) output.append(": ", getBoolean());
    if(isInteger()) output.append(": ", getInteger());
    if(isNatural()) output.append(": ", getNatural());
    if(isReal())    output.append(": ", getReal());
    if(isString())  output.append(": ", getString());
    output.append("\n");
    depth.append("  ");
  }

  for(auto& child : children) {
    output.append(child->serialize(depth));
  }

  return output;
}

}
