auto PSG::serialize(serializer& s) -> void {
  Thread::serialize(s);
  s.integer(psg.enable);
  s.integer(dac.left);
  s.integer(dac.right);
}
