#version 150

in vec4 position;
in vec2 texCoord;

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

out Vertex {
  vec2 texCoord;
  vec2 LUTeffectiveCoord;
} vertexOut;

void main() {

      gl_Position = position;
	  texCoord = texCoord;
	  LUTeffectiveCoord = vec2(fract(texCoord.x * sourceSize[0].x / 1.0), fract(texCoord.y * sourceSize[0].y / 1.0));  
}