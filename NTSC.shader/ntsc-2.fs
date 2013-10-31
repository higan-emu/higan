// NTSC Shader - written by Hans-Kristian Arntzen
// License: GPLv3
// pulled from git://github.com/libretro/common-shaders.git on 08/05/2013

#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
uniform int phase;

// comment the following line to disable gamma correction
#define GAMMA_CORRECTION
#define CRT_GAMMA 2.5
#define MONITOR_GAMMA 2.0

// begin ntsc-decode-filter-3phase.inc //

#define TAPS 24
const float luma_filter_[TAPS + 1] = float[TAPS + 1](
   -0.000071070,
   -0.000032816,
   0.000128784,
   0.000134711,
   -0.000226705,
   -0.000777988,
   -0.000997809,
   -0.000522802,
   0.000344691,
   0.000768930,
   0.000275591,
   -0.000373434,
   0.000522796,
   0.003813817,
   0.007502825,
   0.006786001,
   -0.002636726,
   -0.019461182,
   -0.033792479,
   -0.029921972,
   0.005032552,
   0.071226466,
   0.151755921,
   0.218166470,
   0.243902439);

const float chroma_filter_[TAPS + 1] = float[TAPS + 1](
   0.001845562,
   0.002381606,
   0.003040177,
   0.003838976,
   0.004795341,
   0.005925312,
   0.007242534,
   0.008757043,
   0.010473987,
   0.012392365,
   0.014503872,
   0.016791957,
   0.019231195,
   0.021787070,
   0.024416251,
   0.027067414,
   0.029682613,
   0.032199202,
   0.034552198,
   0.036677005,
   0.038512317,
   0.040003044,
   0.041103048,
   0.041777517,
   0.042004791);
   
 // end ntsc-decode-filter-3phase.inc //

#define fetch_offset(offset, one_x) \
   texture(source[0], texCoord + vec2((offset) * (one_x), 0.0)).xyz

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
   
#ifdef GAMMA_CORRECTION
   vec3 gamma = vec3(CRT_GAMMA / MONITOR_GAMMA);
   rgb = pow(rgb, gamma.rgb);
#endif

fragColor = vec4(rgb, 1.0);
}