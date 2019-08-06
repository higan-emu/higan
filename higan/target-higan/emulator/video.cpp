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
    settingsMenu.updateShaders();
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

  if(!videoInstance.hasFormat(settings.video.format)) {
    settings.video.format = videoInstance.hasFormats().first();
  }
  videoInstance.setFormat(settings.video.format);

  if(videoInstance.hasShader()) {
    videoInstance.setShader(settings.video.shader);
  }

  videoUpdateColors();
}

auto Emulator::videoUpdateColors() -> void {
  higan::video.setFormat(255 << 16, 255 << 8, 255 << 0);  //ARGB24 fallback
  if(settings.video.format == "ARGB24") higan::video.setFormat(255 << 16, 255 <<  8, 255 << 0);
  if(settings.video.format == "RGBA24") higan::video.setFormat(255 << 24, 255 << 16, 255 << 8);
  if(settings.video.format == "ARGB30") higan::video.setFormat(1023 << 20, 1023 << 10, 1023 << 0);
  if(settings.video.format == "RGBA30") higan::video.setFormat(1023 << 22, 1023 << 12, 1023 << 2);
  higan::video.setLuminance(settings.video.luminance);
  higan::video.setSaturation(settings.video.saturation);
  higan::video.setGamma(settings.video.gamma);
  higan::video.setPalette();
}

auto Emulator::videoUpdateShader() -> void {
  videoInstance.setShader(settings.video.shader);
}

auto Emulator::videoToggleFullscreen() -> void {
  //not all drivers support exclusive fullscreen mode
  if(!videoInstance.hasExclusive()) return;

  if(!videoInstance.exclusive()) {
    videoInstance.setExclusive(true);
    inputInstance.acquire();
  } else {
    videoInstance.setExclusive(false);
    inputInstance.release();
    programWindow.viewport.setFocused();
  }
}
