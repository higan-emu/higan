#version 150

in vec4 position;
in vec2 texCoord;

out Vertex {
   vec2 texCoord;
   vec4 t1;
} vertexOut;

   uniform vec4 targetSize;
   uniform vec4 outputSize;
   uniform vec4 sourceSize[];

void main() {
   gl_Position = position;

   //     A1 B1 C1
   //  A0  A  B  C C4
   //  D0  D  E  F F4
   //  G0  G  H  I I4
   //     G5 H5 I5

   float dx = sourceSize[0].z;
   float dy = sourceSize[0].w;

   vertexOut.texCoord = texCoord;
   vertexOut.t1 = vec4(dx, 0, 0, dy);  // F H
}