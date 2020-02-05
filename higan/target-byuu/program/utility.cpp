auto Program::pause(bool state) -> void {
  if(paused == state) return;
  paused = state;
  presentation.pauseEmulation.setChecked(paused);

  if(paused) {
    showMessage("Paused");
    message.text = "Paused";
  }
}

auto Program::paletteUpdate() -> void {
  if(!emulator) return;
  for(auto& screen : emulator->root->find<higan::Node::Screen>()) {
    screen->setLuminance(settings.video.luminance);
    screen->setSaturation(settings.video.saturation);
    screen->setGamma(settings.video.gamma);
  }
}
