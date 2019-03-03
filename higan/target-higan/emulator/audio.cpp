auto Emulator::audioUpdate() -> void {
  if(auto& audio = sound) {
    audioUpdate(audio, programWindow.handle());
  }
}

auto Emulator::audioUpdate(Audio& audio, uintptr context) -> void {
  if(audio && audio.driver() != settings.audio.driver) {
    audio.reset();
  }

  if(!audio) {
    audio.create(settings.audio.driver);
    audio.setContext(context);
    if(!audio.ready()) {
      audio.create(settings.audio.driver = "None");
    }
  }

  audio.setContext(context);
  if(audio.hasDevice(settings.audio.device)) {
    audio.setDevice(settings.audio.device);
  } else {
    settings.audio.device = audio.device();
  }
  if(audio.hasFrequency(settings.audio.frequency)) {
    audio.setFrequency(settings.audio.frequency);
  } else {
    settings.audio.frequency = audio.frequency();
  }
  if(audio.hasLatency(settings.audio.latency)) {
    audio.setLatency(settings.audio.latency);
  } else {
    settings.audio.latency = audio.latency();
  }
  if(audio.hasExclusive()) {
    audio.setExclusive(settings.audio.exclusive);
  } else {
    settings.audio.exclusive = false;
  }
  if(audio.hasBlocking()) {
    audio.setBlocking(settings.audio.blocking);
  } else {
    settings.audio.blocking = false;
  }
  if(audio.hasDynamic()) {
    audio.setDynamic(settings.audio.dynamic);
  } else {
    settings.audio.dynamic = false;
  }
}

auto Emulator::audioUpdateEffects() -> void {
  if(interface) {
    higan::audio.setFrequency(settings.audio.frequency + settings.audio.skew);
    higan::audio.setVolume(!settings.audio.mute ? settings.audio.volume : 0.0);
    higan::audio.setBalance(settings.audio.balance);
  }
}
