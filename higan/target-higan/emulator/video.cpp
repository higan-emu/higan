auto Emulator::videoUpdate() -> void {
  for(auto& viewport : viewports) {
    videoUpdate(viewport->video, viewport->handle());
  }
}

auto Emulator::videoUpdate(Video& video, uintptr context) -> void {
  if(video && video.driver() != settings.video.driver) {
    video.reset();
  }

  if(!video) {
    video.create(settings.video.driver);
    video.setContext(context);
    if(!video.ready()) {
      video.reset();
      video.create(settings.video.driver = "None");
    }
  }

  video.setContext(context);
  if(video.hasFormat(settings.video.format)) {
    video.setFormat(settings.video.format);
  } else {
    settings.video.format = video.format();
  }
  if(video.hasExclusive()) {
  //handled by fullscreen codepath
  } else {
    settings.video.exclusive = false;
  }
  if(video.hasBlocking()) {
    video.setBlocking(settings.video.blocking);
  } else {
    settings.video.blocking = false;
  }
  if(video.hasFlush()) {
    video.setFlush(settings.video.flush);
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
