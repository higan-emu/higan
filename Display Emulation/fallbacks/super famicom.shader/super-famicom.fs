#version 150

#in coloremulation

#ifdef coloremulation
const uint gamma_ramp[32] = {
  0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
  0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
  0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
  0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff
};
#endif

uniform sampler2D source[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
  vec4 rgbl = texture(source[0], texCoord);
  float r = rgbl[0];
  float g = rgbl[1];
  float b = rgbl[2];
  float l = rgbl[3];
  #ifdef coloremulation
  r = gamma_ramp[uint(rgbl[0] * 31)] / 255.0;
  g = gamma_ramp[uint(rgbl[1] * 31)] / 255.0;
  b = gamma_ramp[uint(rgbl[2] * 31)] / 255.0;
  #endif
  float L = (1.0 + l * 15) / 16.0;
  if(l == 0.0) L *= 0.5;
  fragColor = vec4(
    L * r,
    L * g,
    L * b,
    0.0
  );
}