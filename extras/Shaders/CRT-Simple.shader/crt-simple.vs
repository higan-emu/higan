#version 150

in vec4 position;
in vec2 texCoord;

out Vertex {
  vec2 texCoord;
  // Define some calculations that will be used in fragment shader.
  vec2 one;
  float mod_factor;
} vertexOut;

        uniform vec4 targetSize;
        uniform vec2 outputSize;
        uniform vec4 sourceSize[];

void main() {
  gl_Position = position;
  vertexOut.texCoord = texCoord;


   // The size of one texel, in texture-coordinates.
   vertexOut.one = 1.0 / sourceSize[0].xy;

   // Resulting X pixel-coordinate of the pixel we're drawing.
   vertexOut.mod_factor = texCoord.x * targetSize.x;
}
