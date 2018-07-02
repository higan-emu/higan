#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

  // overscan (e.g. 1.02 for 2% overscan)
#define overscan vec2(1.0 , 1.07) // use (1.0, 1.07) for SNES, (1.0, 1.0) for NES
  // aspect ratio
#define aspect vec2(1.0, 0.75)
  // size of curved corners
#define cornersize 0.02
  // border smoothness parameter
  // decrease if borders are too aliased
#define cornersmooth 80.0

#define mod_factor (texCoord.x * targetSize.x)

in Vertex {
  vec2 texCoord;
};

float corner(vec2 coord)
{
  coord = (coord - vec2(0.5)) * overscan + vec2(0.5);
  coord = min(coord, vec2(1.0)-coord) * aspect;
  vec2 cdist = vec2(cornersize);
  coord = (cdist - min(coord,cdist));
  float dist = sqrt(dot(coord,coord));
  return clamp((cdist.x-dist)*cornersmooth,0.0, 1.0);
}

out vec4 fragColor;

void main() {
float round = corner(texCoord);
vec3 screen = texture(source[0], texCoord).rgb;
screen *= vec3(round);
	  
fragColor = vec4(screen, 1.0);
}