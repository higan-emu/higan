#version 150

#in coloremulation

#ifdef coloremulation
const uint curve[32] = {
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0e, 0x10, 0x12,
  0x14, 0x16, 0x18, 0x1c, 0x20, 0x28, 0x38, 0x38,
  0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x80,
  0x88, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0
};
#endif

uniform sampler2D source[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
  vec4 rgba = texture(source[0], texCoord);
  #ifndef coloremulation
  fragColor = rgba;
  #else
  uint R = curve[uint(rgba[0] * 31)];
  uint G = curve[uint(rgba[1] * 31)];
  uint B = curve[uint(rgba[2] * 31)];

  uint Rr = R * 16;
  uint Gr = R *  4;
  uint Br = R *  4;

  uint Rg = G *  8;
  uint Gg = G * 16;
  uint Bg = G *  8;

  uint Rb = B *  0;  //intentionally always zero
  uint Gb = B *  8;
  uint Bb = B * 16;

  // With no pointers, the shader language has no swap method.
  #define swap(a, b) {a ^= b; b ^= a; a ^= b;}
  if(Rr < Rg) swap(Rr, Rg);
  if(Rr < Rb) swap(Rr, Rb);
  if(Rg < Rb) swap(Rg, Rb);

  if(Gr < Gg) swap(Gr, Gg);
  if(Gr < Gb) swap(Gr, Gb);
  if(Gg < Gb) swap(Gg, Gb);

  if(Br < Bg) swap(Br, Bg);
  if(Br < Bb) swap(Br, Bb);
  if(Bg < Bb) swap(Bg, Bb);

  R = (((4 * Rr + 2 * Rg + Rb) * 160) >> 14) + 32;
  G = (((4 * Gr + 2 * Gg + Gb) * 160) >> 14) + 32;
  B = (((4 * Br + 2 * Bg + Bb) * 160) >> 14) + 32;

  fragColor = vec4(
    R / 255.0,
    G / 255.0,
    B / 255.0,
    0.0
  );
  #endif
}