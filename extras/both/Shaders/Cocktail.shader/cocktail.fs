#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];

in Vertex {
  vec2 texCoord1;
  vec2 texCoord2;
};

out vec4 fragColor;

void main(void) {
  fragColor = texture(source[0], texCoord1) + texture(source[0], texCoord2);
}
