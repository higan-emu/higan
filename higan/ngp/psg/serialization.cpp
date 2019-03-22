auto PSG::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.integer(tone0.counter);
  s.integer(tone0.pitch);
  s.integer(tone0.output);
  s.integer(tone0.volume.left);
  s.integer(tone0.volume.right);

  s.integer(tone1.counter);
  s.integer(tone1.pitch);
  s.integer(tone1.output);
  s.integer(tone1.volume.left);
  s.integer(tone1.volume.right);

  s.integer(tone2.counter);
  s.integer(tone2.pitch);
  s.integer(tone2.output);
  s.integer(tone2.volume.left);
  s.integer(tone2.volume.right);

  s.integer(noise.counter);
  s.integer(noise.pitch);
  s.integer(noise.enable);
  s.integer(noise.rate);
  s.integer(noise.lfsr);
  s.integer(noise.clock);
  s.integer(noise.output);
  s.integer(noise.volume.left);
  s.integer(noise.volume.right);

  s.integer(psg.enable);

  s.integer(dac.left);
  s.integer(dac.right);

  s.integer(select);
}
