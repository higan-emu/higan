#version 150
#define brightness 2.0

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform sampler2D pixmap[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {

vec4 inverse = 1.0 - texture2D(source[0], texCoord);
vec4 screen = texture2D(pixmap[0], texCoord);

fragColor = (screen) * (1.0 - inverse);
}
