auto Emulator::updateMessage() -> void {
  if(chrono::millisecond() - state.message.timestamp >= 2000) {
    programWindow.statusMessage.setText(state.message.text);
  }
}

auto Emulator::showMessage(const string& message) -> void {
  state.message.timestamp = chrono::millisecond();
  programWindow.statusMessage.setText(message);
}

auto Emulator::setCaption(const string& caption) -> void {
  programWindow.statusCaption.setText(caption);
}
