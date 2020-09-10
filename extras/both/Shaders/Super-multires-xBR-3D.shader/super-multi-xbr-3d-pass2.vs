#version 150
 
in vec4 position;
in vec2 texCoord;
in vec4 sourceSize[];
in vec4 targetSize;
 
out Vertex {
   vec2 texCoord;
} vertexOut;

#define SourceSize sourceSize[0]

void main()
{
   vertexOut.texCoord = texCoord.xy * 1.0001;
   gl_Position = position;
}