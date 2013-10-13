// NTSC Shader - written by Hans-Kristian Arntzen
// License: GPLv3
// pulled from git://github.com/libretro/common-shaders.git on 08/05/2013

#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
uniform int phase;
out vec3 col;
out float mod_phase;
out float chroma_phase;

#define PI 3.14159265
#define CHROMA_MOD_FREQ (PI / 3.0)
#define THREE_PHASE
#define SATURATION 1.0
#define BRIGHTNESS 1.0
#define ARTIFACTING 1.0
#define FRINGING 1.0

in Vertex {
  vec2 texCoord;
};

mat3 mix_mat = mat3(
      BRIGHTNESS, FRINGING, FRINGING,
      ARTIFACTING, 2.0 * SATURATION, 0.0,
      ARTIFACTING, 0.0, 2.0 * SATURATION
);

mat3 yiq2rgb_mat = mat3(
   1.0, 1.0, 1.0,
   0.956, -0.2720, -1.1060,
   0.6210, -0.6474, 1.7046
);

vec3 yiq2rgb(vec3 yiq)
{
   return (yiq * yiq2rgb_mat);
}

mat3 yiq_mat = mat3(
      0.2989, 0.5959, 0.2115,
      0.5870, -0.2744, -0.5229,
      0.1140, -0.3216, 0.3114
);

vec3 rgb2yiq(vec3 col)
{
   return (col * yiq_mat);
}

out vec4 fragColor;

void main() {
vec3 col = texture(source[0], texCoord).rgb;
vec3 yiq = rgb2yiq(col);

float chroma_phase = 0.6667 * PI * (mod((texCoord.y * sourceSize[0].y * (targetSize.y / sourceSize[0].y)), 3.0) + phase);

float mod_phase = chroma_phase + (texCoord.x * sourceSize[0].x * (targetSize.x / sourceSize[0].x)) * CHROMA_MOD_FREQ;

float i_mod = cos(mod_phase);
float q_mod = sin(mod_phase);

yiq.yz *= vec2(i_mod, q_mod);

fragColor = vec4(yiq, 1.0);;
}