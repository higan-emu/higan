#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];

uniform sampler2D frame[];
uniform vec4 frameSize[];
vec4 color;

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
  color = texture(frame[7], texCoord);
  color = (color + texture(frame[6], texCoord)) / 2.0;
  color = (color + texture(frame[5], texCoord)) / 2.0;
  color = (color + texture(frame[4], texCoord)) / 2.0;
  color = (color + texture(frame[3], texCoord)) / 2.0;
  color = (color + texture(frame[2], texCoord)) / 2.0;
  color = (color + texture(frame[1], texCoord)) / 2.0;
  color = (color + texture(frame[0], texCoord)) / 2.0;
  color = (color + texture(source[0], texCoord)) / 2.0;
  fragColor = color;
}
