#version 150

////////////////////////////////////////////////////////
//	GTU version 0.50	
//	Author: aliaspider - aliaspider@gmail.com
//	License: GPLv3      
////////////////////////////////////////////////////////

#in cropOverscan


in vec4 position;
in vec2 texCoord;

out Vertex{
	vec2 texCoord;
}vertexOut;

void main(void){
	
#ifdef cropOverscan
	gl_Position=position;
	gl_Position.y/=(224.0/240.0);
#else
	gl_Position=position;
#endif
	
	vertexOut.texCoord=texCoord;		
}
