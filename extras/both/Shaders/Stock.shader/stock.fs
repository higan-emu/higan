#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

in Vertex {
   vec2 vTexCoord;
};

out vec4 FragColor;

void main() {
   vec2 test = sourceSize[0].xy;
   FragColor = texture(source[0], vTexCoord);
}