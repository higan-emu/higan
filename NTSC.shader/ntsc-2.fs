// NTSC Shader - written by Hans-Kristian Arntzen
// License: GPLv3
// pulled from git://github.com/libretro/common-shaders.git on 08/05/2013

#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
uniform int phase;

#define TAPS 32
const float luma_filter_[TAPS + 1] = float[TAPS + 1](
   -0.000000,
   -0.000009,
   -0.000022,
   0.000011,
   0.000128,
   0.000276,
   0.000292,
   -0.000000,
   -0.000603,
   -0.001204,
   -0.001244,
   -0.000266,
   0.001610,
   0.003428,
   0.003711,
   0.001362,
   -0.003259,
   -0.007882,
   -0.009126,
   -0.004494,
   0.005417,
   0.016004,
   0.020197,
   0.012294,
   -0.007654,
   -0.031563,
   -0.045146,
   -0.033879,
   0.009360,
   0.078889,
   0.156343,
   0.217032,
   0.239992
);

const float chroma_filter_[TAPS + 1] = float[TAPS + 1](
   0.000000,
   0.000009,
   0.000036,
   0.000075,
   0.000114,
   0.000134,
   0.000106,
   -0.000000,
   -0.000219,
   -0.000582,
   -0.001107,
   -0.001792,
   -0.002608,
   -0.003488,
   -0.004323,
   -0.004963,
   -0.005219,
   -0.004878,
   -0.003718,
   -0.001532,
   0.001847,
   0.006520,
   0.012500,
   0.019691,
   0.027887,
   0.036772,
   0.045933,
   0.054894,
   0.063144,
   0.070186,
   0.075573,
   0.078956,
   0.080109
);

#define fetch_offset(offset, one_x) \
   texture(source[0], texCoord + vec2((offset) * (one_x), 0.0)).xyz
#define CRT_GAMMA 2.5
#define MONITOR_GAMMA 2.0

// NTSC RGB>YUV //

mat3 yiq2rgb_mat = mat3(
   1.0,     1.0,     1.0,
   0.956,  -0.2720, -1.1060,
   0.6210, -0.6474,  1.7046
);

vec3 yiq2rgb(vec3 yiq)
{
   return (yiq * yiq2rgb_mat);
}

mat3 yiq_mat = mat3(
      0.2989,  0.5959,  0.2115,
      0.5870, -0.2744, -0.5229,
      0.2115, -0.3216,  0.3114
);

vec3 rgb2yiq(vec3 col)
{
   return (col * yiq_mat);
}

// END NTSC RGB>YUV //

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
// NTSC PASS3 DECODE //

float one_x = 1.0 / sourceSize[0].x;
vec3 signal = vec3(0.0);
for (int i = 0; i < TAPS; i++)
{
   float offset = float(i);

   vec3 sums = fetch_offset(offset - float(TAPS), one_x) +
      fetch_offset(float(TAPS) - offset, one_x);

   signal += sums * vec3(luma_filter_[i], chroma_filter_[i], chroma_filter_[i]);
}
signal += texture(source[0], texCoord).xyz *
   vec3(luma_filter_[TAPS], chroma_filter_[TAPS], chroma_filter_[TAPS]);

// END NTSC PASS3 DECODE //
   vec3 rgb = yiq2rgb(signal);

fragColor = vec4(rgb, 1.0);
}