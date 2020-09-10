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

vec4 inverse = 1.0 - texture(source[0], texCoord);
vec4 screen = texture(pixmap[0], LUTeffectiveCoord);

fragColor = (screen) * (1.0 - inverse);
}
