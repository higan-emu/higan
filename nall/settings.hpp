#pragma once

#include <nall/maybe.hpp>
#include <nall/primitives.hpp>
#include <nall/string.hpp>

namespace nall {

template<typename T> struct Setting;

struct Settings {
  virtual auto value() const -> string = 0;
  virtual auto setValue(string value) -> bool = 0;

  inline Settings(string name = "") : _name(name) {}
  inline auto name() const -> string { return _name; }
  inline auto operator[](string path) const -> maybe<Settings&>;

  inline auto valid() const -> vector<string> { return _valid; }
  inline auto setValid(vector<string> values) -> void { _valid = values; }

  inline auto onModify(const function<void()>& modify) -> void { _modify = modify; }
  inline auto doModify() -> void { if(_modify) _modify(); }

  inline auto serialize(string depth = "") const -> string;
  inline auto unserialize(string source) -> void;

protected:
  inline auto unserialize(Markup::Node node) -> void;

  const string _name;
  vector<string> _valid;
  vector<Settings*> _children;
  function<void ()> _modify;
  template<typename T> friend class Setting;
};

template<typename T = string> struct Setting : Settings {
  using type = T;

  auto value() const -> string override {
    if constexpr(is_same_v<T, nothing_t>) return {};
    else return {_value};
  }

  auto setValue(string value) -> bool override {
    if(_valid && !_valid.find(value)) return false;
    return assign(T{(const char*)value}), true;
  }

  auto latched() const -> const T& { return _latched; }
  auto latch() -> void { _latched = _value; }

  Setting() {}
  Setting(string name, T value = {}) : Settings(name), _value(value) {}
  Setting(Settings* parent, string name, T value = {}) : Settings(name), _value(value) {
    if(!parent) throw;
    parent->_children.append(this);
  }

  auto operator()() const -> const T& { return _value; }
  auto operator()(const T& value) -> const T& { return assign(value), _value; }

protected:
  auto assign(const T& value) -> void {
    _value = value;
    doModify();
  }

  T _value;
  T _latched;
};

auto Settings::operator[](string path) const -> maybe<Settings&> {
  if(!path) return nothing;
  auto part = path.split("/", 1L);
  auto name = part.takeFirst();
  if(_name != name) return nothing;
  if(!part) return *this;
  path = part.merge("/");
  for(auto& child : _children) {
    if(auto setting = (*child)[part.merge("/")]) {
      return setting;
    }
  }
  return nothing;
}

auto Settings::serialize(string depth) const -> string {
  string output;

  if(_name) {
    output.append(depth, _name);
    if(auto _value = value()) output.append(": ", _value);
    output.append("\n");
    depth.append("  ");
  }

  for(auto& child : _children) {
    output.append(child->serialize(depth));
  }

  return output;
}

auto Settings::unserialize(string source) -> void {
  unserialize(BML::unserialize(source)[0]);
}

auto Settings::unserialize(Markup::Node node) -> void {
  if(_name == node.name()) setValue(node.text());
  for(auto& child : _children) {
    if(auto leaf = node[child->name()]) child->unserialize(leaf);
  }
}

}
