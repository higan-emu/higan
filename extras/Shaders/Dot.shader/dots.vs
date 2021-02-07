#version 150

in vec4 position;
in vec2 texCoord;

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

      out vec2 c00;
      out vec2 c10;
      out vec2 c20;
      out vec2 c01;
      out vec2 c11;
      out vec2 c21;
      out vec2 c02;
      out vec2 c12;
      out vec2 c22;
      out vec2 pixel_no;

out Vertex {
  vec2 texCoord;
} vertexOut;

void main() {

      gl_Position = position;

    float dx = sourceSize[0].z;
         float dy = sourceSize[0].w;

         vec2 tex = texCoord.xy;
         c00 = tex + vec2(-dx, -dy);
         c10 = tex + vec2(  0, -dy);
         c20 = tex + vec2( dx, -dy);
         c01 = tex + vec2(-dx,   0);
         c11 = tex + vec2(  0,   0);
         c21 = tex + vec2( dx,   0);
         c02 = tex + vec2(-dx,  dy);
         c12 = tex + vec2(  0,  dy);
         c22 = tex + vec2( dx,  dy);
         pixel_no = tex * sourceSize[0].xy;
}