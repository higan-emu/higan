#version 150
#define brightness 2.0

uniform sampler2D source[];
uniform vec4 sourceSize[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {

vec4 image = texture2D(source[3], texCoord);
vec4 previous = texture2D(source[0], texCoord);

fragColor = 1.0 - (1.0 - image) * (1.0 - previous);
}
