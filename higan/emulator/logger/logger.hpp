#pragma once

namespace higan {

struct Logger;
struct Notification;
struct Tracer;

struct Logger {
  Logger() = default;
  Logger(const Logger&) = delete;
  auto operator=(const Logger&) = delete;

  auto reset(Interface* = nullptr) -> void;

  auto attached(shared_pointer<Notification>&) const -> bool;
  auto attach(shared_pointer<Notification>&) -> void;
  auto detach(shared_pointer<Notification>&) -> void;

  auto attached(shared_pointer<Tracer>&) const -> bool;
  auto attach(shared_pointer<Tracer>&) -> void;
  auto detach(shared_pointer<Tracer>&) -> void;

  vector<shared_pointer<Notification>> notifications;
  vector<shared_pointer<Tracer>> tracers;
};

struct Notification {
  inline auto enabled() const -> bool { return _enabled; }
  inline auto source() const -> string { return _source; }
  inline auto name() const -> string { return _name; }

  auto setEnabled(bool enabled) -> void;
  auto setSource(string_view source) -> void;
  auto setName(string_view name) -> void;

  noinline auto event(string_view annex = {}) -> void;

private:
  bool _enabled = false;
  string _source;
  string _name;
};

struct Tracer {
  inline auto enabled() const -> bool { return _enabled; }
  inline auto source() const -> string { return _source; }
  inline auto depth() const -> uint { return _history.size(); }
  inline auto addressBits() const -> uint { return _addressBits; }

  auto setEnabled(bool enabled) -> void;
  auto setSource(string_view source) -> void;
  auto setDepth(uint depth) -> void;
  auto setAddressBits(uint addressBits) -> void;

  noinline auto address(uint64 address) -> bool;
  noinline auto instruction(string_view instruction, string_view context, string_view annex = {}) -> void;

private:
  bool _enabled = false;
  string _source;
  vector<uint64> _history;
  uint64 _address = 0;
  uint64 _omitted = 0;
  uint _addressBits = 0;
};

extern Logger logger;

}
