auto Notification::setEnabled(bool enabled) -> void {
  _enabled = enabled;
}

auto Notification::setSource(string_view source) -> void {
  _source = source;
}

auto Notification::setName(string_view name) -> void {
  _name = name;
}

auto Notification::event(string_view annex) -> void {
  string output{_source, " [", _name};
  if(annex) output.append(": ", annex);
  platform->log(output.strip().append("]"));
}
