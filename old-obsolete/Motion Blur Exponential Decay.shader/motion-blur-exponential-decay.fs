#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];

uniform sampler2D frame[];
uniform vec4 frameSize[];
out vec4 color;

#define response_time 0.333	//simulate response time, higher values result in longer color transition periods - [0, 1]

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
  color = abs(texture(frame[6], texCoord).rgba);
  color += (abs(texture(frame[5], texCoord).rgba) - color) * response_time;
  color += (abs(texture(frame[4], texCoord).rgba) - color) * pow(response_time, 2.0);
  color += (abs(texture(frame[3], texCoord).rgba) - color) * pow(response_time, 3.0);
  color += (abs(texture(frame[2], texCoord).rgba) - color) * pow(response_time, 4.0);
  color += (abs(texture(frame[1], texCoord).rgba) - color) * pow(response_time, 5.0);
  color += (abs(texture(frame[0], texCoord).rgba) - color) * pow(response_time, 6.0);
  color += (abs(texture(source[0], texCoord).rgba) - color) * pow(response_time, 7.0);
  fragColor = color;
}