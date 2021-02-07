#version 150

#in saturation
#in hue
#in contrast
#in brightness
#in gamma

#define black 0.518
#define white 1.962
#define attenuation 0.746
const float levels[8] = {
  0.350, 0.518, 0.962, 1.550,
  1.094, 1.506, 1.962, 1.962
};

uniform sampler2D source[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

bool wave(int p, int color) {
  return (color + p + 8) % 12 < 6;
};

float gammaAdjust(float f) {
  return f < 0.0 ? 0.0 : pow(f, 2.2 / gamma);
};

void main() {
  vec4 elc0 = texture(source[0], texCoord);
  int emphasis = int(elc0[0] * 7);

  int color = int(elc0[2] * 15), level = color < 0xe ? int(elc0[1] * 3) : 1;

  float lo_and_hi[2] = float[](
    levels[level + (color == 0x0 ? 4 : 0)],
    levels[level + (color <  0xd ? 4 : 0)]
  );

  float y = 0.0, i = 0.0, q = 0.0;
  for(int p = 0; p < 12; p++) {
    float spot = lo_and_hi[wave(p, color) ? 1 : 0];

    if(((emphasis & 1) == 1 && wave(p, 12))
    || ((emphasis & 2) == 2 && wave(p,  4))
    || ((emphasis & 4) == 4 && wave(p,  8))
    ) spot *= attenuation;

    float v = (spot - black) / (white - black);

    v = (v - 0.5) * contrast + 0.5;
    v *= brightness / 12.0;

    y += v;
    i += v * cos((3.141592653 / 6.0) * (p + hue));
    q += v * sin((3.141592653 / 6.0) * (p + hue));
  }

  i *= saturation;
  q *= saturation;

  fragColor = vec4(
    gammaAdjust(y +  0.946882 * i +  0.623557 * q),
    gammaAdjust(y + -0.274788 * i + -0.635691 * q),
    gammaAdjust(y + -1.108545 * i +  1.709007 * q),
    0.0
  );
}