auto MCD::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.array(pram.data(), pram.size());
  cdc.serialize(s);
  cdd.serialize(s);
  timer.serialize(s);
  gpu.serialize(s);
  pcm.serialize(s);
}

auto MCD::IRQ::serialize(serializer& s) -> void {
}

auto MCD::CDC::serialize(serializer& s) -> void {
}

auto MCD::CDD::serialize(serializer& s) -> void {
}

auto MCD::Timer::serialize(serializer& s) -> void {
}

auto MCD::GPU::serialize(serializer& s) -> void {
}

auto MCD::PCM::serialize(serializer& s) -> void {
}
