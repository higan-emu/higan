#include <spec/spec.hpp>

namespace higan::Spectrum {

Keyboard keyboard;

auto Keyboard::load(Node::Object parent) -> void {
  port = parent->append<Node::Port>("Keyboard");
  port->setFamily("ZX Spectrum");
  port->setType("Keyboard");
  port->setHotSwappable(true);
  port->setAllocate([&](auto name) { return allocate(port, name); });
  port->setConnect([&] { connect(); });
  port->setDisconnect([&] { disconnect(); });
  port->setSupported({"Original"});
}

auto Keyboard::unload() -> void {
  disconnect();
  port = {};
}

auto Keyboard::allocate(Node::Port parent, string name) -> Node::Peripheral {
  return layout = parent->append<Node::Peripheral>(name);
}

auto Keyboard::connect() -> void {
  Markup::Node document;
  if(auto fp = platform->open(layout, "layout.bml", File::Read)) {
    document = BML::unserialize(fp->reads());
  }

  for(uint _key : range(40)) {
    string label{_key};
    if(auto key = document[{"layout/key[", _key, "]"}]) {
      label = key.text();
    }
    keys[_key] = layout->append<Node::Button>(label);
  }
}

auto Keyboard::disconnect() -> void {
  layout = {};
  for(uint _key : range(40)) {
      keys[_key] = {};
  }
}

auto Keyboard::power() -> void {

}

auto Keyboard::read(uint8 row) -> uint5 {
  constexpr uint rows[8][5] = {
      {38, 29, 30, 31, 32},
      {20, 21, 22, 23, 24},
      {10, 11, 12, 13, 14},
      { 0,  1,  2,  3,  4},
      { 9,  8,  7,  6,  5},
      {19, 18, 17, 16, 15},
      {36, 28, 27, 26, 25},
      {37, 39, 35, 34, 33},
  };

  uint5 data = 0x1f;

  for(uint _key : range(5)) {
    if(auto node = keys[rows[row][_key]]) {
      platform->input(node);
      data.bit(_key) = !node->value();
    }
  }
  return data;
}

}
