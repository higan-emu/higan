auto VDP::serialize(serializer& s) -> void {
  Thread::serialize(s);

  vce.serialize(s);
  vdc0.serialize(s); if(Model::SuperGrafx())
  vdc1.serialize(s); if(Model::SuperGrafx())
  vpc.serialize(s);

  s.integer(io.hcounter);
  s.integer(io.vcounter);
}

auto VCE::serialize(serializer& s) -> void {
}

auto VDC::serialize(serializer& s) -> void {
}

auto VDC::Object::serialize(serializer& s) -> void {
  s.integer(y);
  s.integer(x);
  s.integer(mode);
  s.integer(pattern);
  s.integer(palette);
  s.integer(priority);
  s.integer(width);
  s.integer(height);
  s.integer(hflip);
  s.integer(vflip);
  s.integer(first);
}

auto VPC::serialize(serializer& s) -> void {
  for(auto& setting : settings) {
    s.integer(setting.enableVDC0);
    s.integer(setting.enableVDC1);
    s.integer(setting.priority);
  }
  s.array(window);
  s.integer(select);
}
