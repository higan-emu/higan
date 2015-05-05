#version 150

////////////////////////////////////////////////////////
//	GTU-Famicom version 0.50	
//	Author: aliaspider - aliaspider@gmail.com
//	License: GPLv3      
////////////////////////////////////////////////////////


#in cropOverscan
// #define cropOverscan

uniform vec4 targetSize;
uniform int  phase;

in vec4 position;
in vec2 texCoord;

out Vertex{
	vec2 texCoord;
	float colorPhase;
}vertexOut;

void main(void){	
	gl_Position=position;
	vertexOut.texCoord=texCoord;	
	vec2 pos = (texCoord.xy*targetSize.xy)-0.5;	
	vertexOut.colorPhase=8.0+pos.x+pos.y*4.0+(phase%2)*4.0;
}
