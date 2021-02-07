#version 150

uniform sampler2D source[];
uniform vec2 targetSize;
uniform vec4 sourceSize[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

#define CRTgamma 2.5
#define display_gamma 2.2
#define TEX2D(c) pow(texture2D(source[0],(c)),vec4(CRTgamma))

void main()
{
  vec2 xy = texCoord;
  float oney = 1.0 / sourceSize[0].y;

  float wid = 6.0;

  float c1 = exp(-1.0/wid/wid);
  float c2 = exp(-4.0/wid/wid);
  float c3 = exp(-9.0/wid/wid);
  float c4 = exp(-16.0/wid/wid);
  float norm = 1.0 / (1.0 + 2.0*(c1+c2+c3+c4));

  vec4 sum = vec4(0.0);

  sum += TEX2D(xy + vec2(0.0, -2.0 * oney)) * vec4(c4);
  sum += TEX2D(xy + vec2(0.0, -1.5 * oney)) * vec4(c3);
  sum += TEX2D(xy + vec2(0.0, -1.0 * oney)) * vec4(c2);
  sum += TEX2D(xy + vec2(0.0, -0.5 * oney)) * vec4(c1);
  sum += TEX2D(xy);
  sum += TEX2D(xy + vec2(0.0, 0.5 * oney)) * vec4(c1);
  sum += TEX2D(xy + vec2(0.0, 1.0 * oney)) * vec4(c2);
  sum += TEX2D(xy + vec2(0.0, 1.5 * oney)) * vec4(c3);
  sum += TEX2D(xy + vec2(0.0, 2.0 * oney)) * vec4(c4);

  fragColor = pow(sum*vec4(norm),vec4(1.0/display_gamma));
}
