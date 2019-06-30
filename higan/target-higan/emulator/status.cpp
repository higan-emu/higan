auto Emulator::setMessage(const string& message) -> void {
  programWindow.statusMessage.setText(message);
}

auto Emulator::setCaption(const string& caption) -> void {
  programWindow.statusCaption.setText(caption);
}
