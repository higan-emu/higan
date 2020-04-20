#version 150

in vec4 position;
in vec2 texCoord;

out Vertex {
   vec2 texCoord;
   vec4 t1;
   vec4 t2;
   vec4 t3;
   vec4 t4;
   vec4 t5;
   vec4 t6;
   vec4 t7;
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
   vertexOut.t1 = texCoord.xxxy + vec4(    -dx,   0,  dx, -2.0*dy);  //  A1 B1 C1
   vertexOut.t2 = texCoord.xxxy + vec4(    -dx,   0,  dx,     -dy);  //   A  B  C
   vertexOut.t3 = texCoord.xxxy + vec4(    -dx,   0,  dx,       0);  //   D  E  F
   vertexOut.t4 = texCoord.xxxy + vec4(    -dx,   0,  dx,      dy);  //   G  H  I
   vertexOut.t5 = texCoord.xxxy + vec4(    -dx,   0,  dx,  2.0*dy);  //  G5 H5 I5
   vertexOut.t6 = texCoord.xyyy + vec4(-2.0*dx, -dy,   0,      dy);  //  A0 D0 G0
   vertexOut.t7 = texCoord.xyyy + vec4( 2.0*dx, -dy,   0,      dy);  //  C4 F4 I4 
}