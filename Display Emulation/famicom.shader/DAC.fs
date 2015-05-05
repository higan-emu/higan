#version 150

////////////////////////////////////////////////////////
//	GTU-Famicom version 0.50	
//	Author: aliaspider - aliaspider@gmail.com
//	License: GPLv3      
////////////////////////////////////////////////////////

uniform sampler2D	source[];
uniform sampler2D	pixmap[];
uniform vec4		sourceSize[];
uniform vec4 		targetSize;
uniform int 		phase;

in Vertex {
	vec2 texCoord;
	float colorPhase;
};
out vec4 fragColor;


void main() {
	vec4 c=texture2D(source[0], texCoord.xy);
	
	vec2 pixmapCoord;
	pixmapCoord.x= c.z*(15.0/(16.0*4.0))+c.y*(3.0/4.0)+(0.5/(16.0*4.0));
	pixmapCoord.y= 1.0-(mod(colorPhase,12.0)/(12.0*8.0)+c.x*(7.0/8.0)+(0.5/(12.0*8.0)));
	
	fragColor.r=texture(pixmap[0], pixmapCoord.xy).r;
}