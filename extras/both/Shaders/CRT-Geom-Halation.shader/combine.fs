#version 150
#define halation 0.35

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform sampler2D pixmap[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {

vec4 image = pow(texture2D(source[2], texCoord).rgba, vec4(2.2));
vec4 previous = pow(texture2D(source[0], texCoord).rgba, vec4(2.2));
vec4 combined = mix(previous, image, 1.0 - halation);

fragColor = pow(combined, vec4(1.0 / 2.2));
}
