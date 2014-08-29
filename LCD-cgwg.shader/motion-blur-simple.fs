#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];

uniform sampler2D history[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
vec4 color = vec4(0.0);
  color = texture(history[7], texCoord);
  color = (color + texture(history[6], texCoord)) / 2.0;
  color = (color + texture(history[5], texCoord)) / 2.0;
  color = (color + texture(history[4], texCoord)) / 2.0;
  color = (color + texture(history[3], texCoord)) / 2.0;
  color = (color + texture(history[2], texCoord)) / 2.0;
  color = (color + texture(history[1], texCoord)) / 2.0;
  color = (color + texture(history[0], texCoord)) / 2.0;
  color = (color + texture(source[0], texCoord)) / 2.0;
  fragColor = color;
}
