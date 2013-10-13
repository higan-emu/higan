// NTSC Shader - written by Hans-Kristian Arntzen
// License: GPLv3
// pulled from git://github.com/libretro/common-shaders.git on 08/05/2013

#version 150

in vec4 position;
in vec2 texCoord;

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

out Vertex {
  vec2 texCoord;
} vertexOut;

void main() {
  gl_Position = position;
  vertexOut.texCoord = texCoord - vec2(0.5 / sourceSize[0].x, 0.0); // Compensate for decimate-by-2.
}
