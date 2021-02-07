#version 150

uniform sampler2D source[];
uniform vec2 targetSize;
uniform vec4 sourceSize[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

#define SHARPNESS (8.0 / (sourceSize[0].x / 512.0))

void main()
{
vec4 sum = vec4(0.0);
  
   sum += texture(source[0], vec2(texCoord.x - 4.0 / (SHARPNESS * 128.0), texCoord.y)) * 0.05;
   sum += texture(source[0], vec2(texCoord.x - 3.0 / (SHARPNESS * 128.0), texCoord.y)) * 0.09;
   sum += texture(source[0], vec2(texCoord.x - 2.0 / (SHARPNESS * 128.0), texCoord.y)) * 0.12;
   sum += texture(source[0], vec2(texCoord.x - 1.0 / (SHARPNESS * 128.0), texCoord.y)) * 0.15;
   sum += texture(source[0], vec2(texCoord.x, texCoord.y)) * 0.16;
   sum += texture(source[0], vec2(texCoord.x + 1.0 / (SHARPNESS * 128.0), texCoord.y)) * 0.15;
   sum += texture(source[0], vec2(texCoord.x + 2.0 / (SHARPNESS * 128.0), texCoord.y)) * 0.12;
   sum += texture(source[0], vec2(texCoord.x + 3.0 / (SHARPNESS * 128.0), texCoord.y)) * 0.09;
   sum += texture(source[0], vec2(texCoord.x + 4.0 / (SHARPNESS * 128.0), texCoord.y)) * 0.05;

   
   fragColor = sum;
}