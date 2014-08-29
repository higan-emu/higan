#version 150

in vec4 position;
in vec2 texCoord;
 
out Vertex {
   vec2 texCoord;
} vertexOut;
 


void main(void)
{
	gl_Position=position;	
	vertexOut.texCoord = vec2(texCoord.x-0.1/8192.0,texCoord.y);
}
