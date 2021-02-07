#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 outputSize;

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

#define width 0.3
#define display_gamma 2.2
#define TEX2D(c) (texture(source[0],(c)))

void main()
{
  vec2 xy = texCoord.st;
  float oney = 1.0/sourceSize[0].y;

//  float wid = width*outputSize.y/sourceSize[0].y;
  float wid = width*4.0;

  float c1 = exp(-1.0/wid/wid);
  float c2 = exp(-4.0/wid/wid);
  float c3 = exp(-9.0/wid/wid);
  float c4 = exp(-16.0/wid/wid);
  //float norm = 1.0 / (1.0 + 2.0*(c1+c2+c3+c4));
  float norm = 0.9;

  vec4 sum = vec4(0.0);

  sum += TEX2D(xy + vec2(0.0, -4.0 * oney)) * vec4(c4);
  sum += TEX2D(xy + vec2(0.0, -3.0 * oney)) * vec4(c3);
  sum += TEX2D(xy + vec2(0.0, -2.0 * oney)) * vec4(c2);
  sum += TEX2D(xy + vec2(0.0, -1.0 * oney)) * vec4(c1);
  sum += TEX2D(xy);
  sum += TEX2D(xy + vec2(0.0, +1.0 * oney)) * vec4(c1);
  sum += TEX2D(xy + vec2(0.0, +2.0 * oney)) * vec4(c2);
  sum += TEX2D(xy + vec2(0.0, +3.0 * oney)) * vec4(c3);
  sum += TEX2D(xy + vec2(0.0, +4.0 * oney)) * vec4(c4);

  fragColor = sum*vec4(norm);
}
