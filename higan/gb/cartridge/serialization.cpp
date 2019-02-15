auto Cartridge::serialize(serializer& s) -> void {
  Thread::serialize(s);
  if(ram.size) s.array(ram.data, ram.size);
  if(rtc.size) s.array(rtc.data, rtc.size);

  s.integer(bootromEnable);

  if(mapper) mapper->serialize(s);
}
