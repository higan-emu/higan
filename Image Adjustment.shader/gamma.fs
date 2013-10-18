#version 150

uniform sampler2D source[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

#define monitor_gamma 2.2
#define crt_gamma 2.4
#define saturation 1.0
#define luminance 1.0

vec3 desaturate(vec3 col)
{
   return vec3(dot(col, vec3(0.2126, 0.7152, 0.0722)));
}

vec3 gamma = vec3(1.0 - (crt_gamma - monitor_gamma));

void main() {
   vec3 res = texture(source[0], texCoord).rgb;
   res = mix(desaturate(res), res, saturation);
   res = pow(res, 1.0 / gamma.rgb);
   fragColor = vec4(clamp(res * luminance, 0.0, 1.0), 1.0);
}