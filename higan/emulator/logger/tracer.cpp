auto Tracer::setEnabled(bool enabled) -> void {
  _enabled = enabled;
}

auto Tracer::setSource(string_view source) -> void {
  _source = source;
}

auto Tracer::setDepth(uint depth) -> void {
  _history.reset();
  _history.resize(depth);
  for(auto& history : _history) history = ~0ull;
}

auto Tracer::setAddressBits(uint addressBits) -> void {
  _addressBits = addressBits;
}

auto Tracer::address(uint64 address) -> bool {
  _address = address;
  if(!_history) return false;

  for(auto history : _history) {
    if(address == history) {
      _omitted++;
      return false;
    }
  }

  for(auto index : range(_history.size() - 1)) {
    _history[index] = _history[index + 1];
  }
  _history.last() = address;

  return true;
}

auto Tracer::instruction(string_view instruction, string_view context, string_view annex) -> void {
  if(_omitted) {
    platform->log({_source, " [omitted: ", _omitted, "]"});
    _omitted = 0;
  }
  string output{
    _source, "  ",
    hex(_address, (_addressBits + 3) >> 2), "  ",
    instruction, "  ",
    context, "  ",
    annex
  };
  platform->log(output.strip());
}
