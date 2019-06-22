auto Emulator::videoUpdate() -> void {
  if(videoInstance && videoInstance.driver() != settings.video.driver) {
    videoInstance.reset();
  }

  if(!videoInstance) {
    videoInstance.create(settings.video.driver);
    videoInstance.setContext(programWindow.viewport.handle());
    if(!videoInstance.ready()) {
      videoInstance.reset();
      videoInstance.create(settings.video.driver = "None");
    }
  }

  if(videoInstance.hasExclusive()) {
  //handled by fullscreen codepath
  } else {
    settings.video.exclusive = false;
  }

  if(videoInstance.hasBlocking()) {
    videoInstance.setBlocking(settings.video.blocking);
  } else {
    settings.video.blocking = false;
  }

  if(videoInstance.hasFlush()) {
    videoInstance.setFlush(settings.video.flush);
  } else {
    settings.video.flush = false;
  }
}

auto Emulator::videoUpdateColors() -> void {
  if(interface) {
    for(auto& screen : higan::video.screens) {
      screen->setDepth(settings.video.format == "RGB30" ? 30 : 24);
      screen->setLuminance(settings.video.luminance);
      screen->setSaturation(settings.video.saturation);
      screen->setGamma(settings.video.gamma);
      screen->setPalette();
    }
  }
}
