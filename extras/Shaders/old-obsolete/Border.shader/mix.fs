#version 150

uniform sampler2D source[];
uniform sampler2D pixmap[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main(void) {
	vec4 border = texture(pixmap[0], texCoord).rgba;
	vec4 screen = texture(source[0], texCoord).rgba;
	fragColor = mix(screen, border, border.a);
}