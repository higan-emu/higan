auto Emulator::inputUpdate() -> void {
  if(auto& input = inputManager.input) {
    inputUpdate(input, programWindow.handle());
  }
}

auto Emulator::inputUpdate(Input& input, uintptr context) -> void {
  if(input && input.driver() != settings.input.driver) {
    input.reset();
  }

  if(!input) {
    input.create(settings.input.driver);
    input.setContext(context);
    if(!input.ready()) {
      input.create(settings.input.driver = "None");
    }
  }

  input.setContext(context);
}
