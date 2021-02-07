#version 150

uniform usampler2D source[];
uniform vec4 sourceSize[];
uniform int phase;

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
  uint n = texture(source[0], texCoord).r;
  uint color = n & 0xfu;
  uint level = color < 0xeu ? (n>>4u)&3u : 1u;
  uint emphasis = n >> 6u;

  const float levels[] = float[](0.350, 0.518, 0.962, 1.550,
				 1.094, 1.506, 1.962, 1.962);
  const float attenuation = 0.746;
  const float min = levels[0]*attenuation;
  const float max = levels[7];

  float lo_hi[] = float[](levels[level + 4u*uint(color == 0x0u)],
			  levels[level + 4u*uint(color <  0xdu)]);

  uint x = uint(floor(texCoord.x*sourceSize[0].x*2.0));
  uint y = uint(floor(texCoord.y*sourceSize[0].y));
  float frag[4];
  for (uint i = 0u; i < 4u; i++) {
    uint p = (x*4u+i + y*4u + uint(phase*4)) % 12u;
#define wave(ph,co) (((co)+(ph)+8u)%12u<6u)
    float spot = lo_hi[uint(wave(p,color))];
    if ( (((emphasis&1u)==1u) && wave(p,12u))
	 || (((emphasis&2u)==1u) && wave(p,4u))
	 || (((emphasis&4u)==1u) && wave(p,8u)))
      spot *= attenuation;
    frag[i] = spot;
  }
  fragColor = (vec4(frag[0], frag[1], frag[2], frag[3]) - vec4(min))/vec4(max-min);
}
