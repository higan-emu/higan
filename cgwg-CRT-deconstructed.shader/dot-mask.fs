#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

#define mod_factor (texCoord.x * targetSize.x)

in Vertex {
  vec2 texCoord;
};

//#define MONOCHROME //emulates a black-and-white television

out vec4 fragColor;

vec3 desaturate(vec3 col)
{
   return vec3(dot(col, vec3(0.2126, 0.7152, 0.0722)));
}

void main() {
vec3 screen = texture(source[0], texCoord).rgb;

#ifdef MONOCHROME
screen = desaturate(screen);
#endif

  // Output pixels are alternately tinted green and magenta.
vec3 dotMaskWeights = mix(
          vec3(1.0, 0.7, 1.0),
          vec3(0.7, 1.0, 0.7),
          floor(mod(mod_factor, 2.0))
      );

screen *= dotMaskWeights;
screen *= 1.15; //compensate for darkening
	  
fragColor = vec4(screen, 1.0);
}