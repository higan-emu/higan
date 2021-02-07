#version 150
 
in vec4 position;
in vec2 texCoord;
in mat4 RotationMatrix;
 
out Vertex {
   vec2 texCoord1;
   vec2 texCoord2;
} vertexOut;
 
uniform vec4 targetSize;
uniform vec4 sourceSize[];
 
void main() {
mat4 RotationMatrix = mat4( 1.0, 0.0, 0.0, 0.0,
                            0.0, 2.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 0.0,
                            0.0, 0.0, 0.0, 1.0 );

   gl_Position = (RotationMatrix * position);
   vertexOut.texCoord1 = (vec2(position.x, position.y) * mat2(0.0, -1.0, 1.0, 0.0)) + vec2(0.5, 0.0);
   vertexOut.texCoord2 = (vec2(position.x, position.y) * mat2(0.0, 1.0, -1.0, 0.0)) + vec2(0.5, 0.0);
}