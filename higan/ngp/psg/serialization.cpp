auto PSG::serialize(serializer& s) -> void {
  Thread::serialize(s);
  s.integer(psgEnable);
  psgLeft.serialize(s);
  psgRight.serialize(s);
  s.integer(dacLeft);
  s.integer(dacRight);
}
