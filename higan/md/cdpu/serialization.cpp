auto CDPU::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.array(pram.data(), pram.size());
}
