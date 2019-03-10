auto VPU::renderWindow() -> maybe<uint12> {
  if(io.hcounter >= window.hoffset
  && io.vcounter >=  window.voffset
  && io.hcounter <  window.hoffset + window.hlength
  && io.vcounter <  window.voffset + window.vlength) return {};

  if(Model::NeoGeoPocket()) {
    return (uint12)window.color;
  } else {
    return colorPalette[0xf8 + window.color];
  }
}
