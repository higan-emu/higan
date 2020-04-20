#version 150

#in palette

const int green[12] = {
  0xae, 0xd9, 0x27,
  0x58, 0xa0, 0x28,
  0x20, 0x62, 0x29,
  0x1a, 0x45, 0x2a
};

const int yellow[12] = {
  0xff, 0xf7, 0x7b,
  0xb5, 0xae, 0x4a,
  0x6b, 0x69, 0x31,
  0x21, 0x20, 0x10
};

uniform sampler2D source[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
  #ifndef palette // Standard
  float brightness = 1.0 - texture(source[0], texCoord)[2];
  fragColor = brightness;
  #else           // Color Emulation
  int color = int(texture(source[0], texCoord)[2] * 3) * 3;
  fragColor = vec4(
    palette[color + 0] / 255.0,
    palette[color + 1] / 255.0,
    palette[color + 2] / 255.0,
    0.0
  );
  #endif
}