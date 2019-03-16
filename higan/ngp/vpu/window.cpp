auto VPU::renderWindow() -> bool {
  if(io.hcounter >= window.hoffset
  && io.vcounter >=  window.voffset
  && io.hcounter <  window.hoffset + window.hlength
  && io.vcounter <  window.voffset + window.vlength) return false;

  if(Model::NeoGeoPocket()) {
    window.output = window.color;
  } else {
    window.output = colorPalette[0xf8 + window.color];
  }
  return true;
}
