#version 150
 
in vec4 position;
in vec2 texCoord;
 
out Vertex {
   vec2 texCoord;
} vertexOut;
 
uniform vec4 targetSize;
uniform vec4 sourceSize[];
 
#define vid_scale floor(targetSize.y / sourceSize[0].y)
#define vid_out   (sourceSize[0] * vid_scale)
 
void main() {
   gl_Position = position / vec4(vec2(targetSize / vid_out), 1.0, 1.0);
   vertexOut.texCoord = texCoord;
}