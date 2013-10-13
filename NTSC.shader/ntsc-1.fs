// NTSC Shader - written by Hans-Kristian Arntzen
// License: GPLv3
// pulled from git://github.com/libretro/common-shaders.git on 08/05/2013

#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
uniform int phase;
out float mod_phase;
out float chroma_phase;

#define TAPS 16
const float filter_[TAPS + 1] = float[TAPS + 1](
   0.000694,
   -0.001053,
   -0.000799,
   0.004045,
   -0.003594,
   -0.003974,
   0.012147,
   -0.007480,
   -0.012927,
   0.028719,
   -0.011851,
   -0.036013,
   0.065682,
   -0.015333,
   -0.124306,
   0.281100,
   0.649884
);

#define PI 3.14159265
#define CHROMA_MOD_FREQ (PI / 3.0)
#define THREE_PHASE
#define SATURATION 1.0
#define BRIGHTNESS 1.0
#define ARTIFACTING 1.0
#define FRINGING 1.0

mat3 mix_mat = mat3(
      BRIGHTNESS, FRINGING, FRINGING,
      ARTIFACTING, 2.0 * SATURATION, 0.0,
      ARTIFACTING, 0.0, 2.0 * SATURATION
);

#define fetch_offset(offset, one_x) \
   texture(source[0], texCoord + vec2((offset) * (one_x), 0.0)).xyz

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
float one_x = 1.0 / sourceSize[0].x;

float chroma_phase = 0.6667 * PI * (mod((texCoord.y * sourceSize[0].y), 3.0) + phase);

float mod_phase = chroma_phase + (texCoord.x * sourceSize[0].x) * CHROMA_MOD_FREQ;

vec3 signal = vec3(0.0);
for (int i = 0; i < TAPS; i++)
{
   float offset = float(i);
   vec3 sums =
      fetch_offset(offset - float(TAPS), one_x) +
      fetch_offset(float(TAPS) - offset, one_x);

   signal += sums * filter_[i];
}
signal += texture(source[0], texCoord).xyz * filter_[TAPS];
signal = (signal * mix_mat);

float i_mod = cos(mod_phase);
float q_mod = sin(mod_phase);

signal.yz *= vec2(i_mod, q_mod);
fragColor = vec4(signal, 1.0);
}