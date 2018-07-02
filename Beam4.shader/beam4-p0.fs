#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 outputSize;

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

#define CRT_gamma 2.2
#define display_gamma 2.2
#define TEX2D(c) pow(texture(source[0],(c)), vec4(2.2))

#define risetime 1.1
#define falltime 0.3
#define distortion 0.1
#define offpeakiness 0.1

vec2 radialDistortion(vec2 coord) {
  vec2 cc = coord - 0.5;
  float dist = dot(cc, cc) * distortion;				
  return (coord + cc * (1.0 + dist) * dist) * sourceSize[0].xy / outputSize.xy;
}

void main()
{
  vec2 xy = texCoord.st - vec2(0.0, 0.5/sourceSize[0].y);

  float emu_pos = xy.y * sourceSize[0].y;
  float screen_pos = emu_pos * outputSize.y / sourceSize[0].y;
  float line_y = floor(emu_pos) * outputSize.y / sourceSize[0].y;
  float line_y2 = ceil(emu_pos) * outputSize.y / sourceSize[0].y;
  float x1 = screen_pos-line_y;
  float x2 = line_y2-screen_pos;

  vec2 uv_ratio     = fract(xy*sourceSize[0].xy-vec2(0.5));
  vec2 one          = 1.0/sourceSize[0].xy;

  xy = (floor(xy * sourceSize[0].xy - vec2(0.5)) + vec2(0.5)) / sourceSize[0].xy;

  float wrise = risetime*sourceSize[0].x/256.0;
  float wfall = falltime*sourceSize[0].x/256.0;

  vec2 onex = vec2(one.x, 0.0);

  vec4 ct0= TEX2D(xy);
  vec4 ct1= TEX2D(xy-onex);
  vec4 ct2= TEX2D(xy-2*onex);
  vec4 ct3= TEX2D(xy-3*onex);
  vec4 ct4= TEX2D(xy-4*onex);

  vec4 col = ct4;
  bvec4 rising = greaterThan(ct3, ct4);
  col += (ct3 - col) * (1.0-exp(-vec4(1.0)/mix(vec4(wfall),vec4(wrise),rising)));
  rising = greaterThan(ct2, ct3);
  col += (ct2 - col) * (1.0-exp(-vec4(1.0)/mix(vec4(wfall),vec4(wrise),rising)));
  rising = greaterThan(ct1, ct2);
  col += (ct1 - col) * (1.0-exp(-vec4(1.0)/mix(vec4(wfall),vec4(wrise),rising)));
  rising = greaterThan(ct0, ct1);
  col += (ct0 - col) * (1.0-exp(-vec4(uv_ratio.x)/mix(vec4(wfall),vec4(wrise),rising)));

  if (x1 < 1.0)
    fragColor = col*vec4(offpeakiness/2+(1-offpeakiness)*(1-x1));
  else if(x2 < 1.0)
    fragColor = col*vec4(offpeakiness/2+(1-offpeakiness)*(1-x2));
  else
    fragColor = vec4(0.0);
}
