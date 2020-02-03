auto Program::updateMessage() -> void {
  if(chrono::millisecond() - message.timestamp >= 2000) {
    presentation.statusBar.setText(message.text);
  }
}

auto Program::showMessage(const string& text) -> void {
  message.timestamp = chrono::millisecond();
  presentation.statusBar.setText(text);
}
