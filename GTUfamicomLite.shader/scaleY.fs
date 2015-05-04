#version 150

////////////////////////////////////////////////////////
//	GTU version 0.50	
//	Author: aliaspider - aliaspider@gmail.com
//	License: GPLv3      
////////////////////////////////////////////////////////


#in tvVerticalResolution
#in noScanlines
#in blackLevel
#in contrast
#in gamma
#in perfTest2


uniform sampler2D	source[];
uniform vec4		sourceSize[];
uniform vec4		targetSize;

in Vertex {
	vec2 texCoord;
	float range;
};
out vec4 fragColor;

#define pi			3.14159265358
float STU0 (float x, float b){	
	x = x*b*pi;
	return clamp(((x+sin(x))/(2.0*pi))+0.5,0.0,1.0);
}
float STU(float x, float b){
	return STU0(x+0.5,b)-STU0(x-0.5,b);
}
float STUR(float x, float b,float r){
	return (STU0(x+r,b)-STU0(x-r,b));
}
#ifdef noScanlines		
#define PROCESS(Y) \
c=texture(source[0], vec2(texCoord.x,texCoord.y - (Y)*sourceSize[0].w)).xyz;\
tempColor+=c*STU((Y),(tvVerticalResolution*sourceSize[0].w));
#define FINISH
#else
#define PROCESS(Y) \
c=texture(source[0], vec2(texCoord.x,texCoord.y - (Y)*sourceSize[0].w)).xyz;\
tempColor+=c*STUR((Y),tvVerticalResolution*sourceSize[0].w,0.5*(sourceSize[0].y*targetSize.w));
#define FINISH tempColor*=(sourceSize[0].w*targetSize.y);
#endif

void main() {
	
	float	offset	= fract(texCoord.y * sourceSize[0].y - 0.5);
	vec3	tempColor = vec3(0.0);	

	float range=ceil(0.5+sourceSize[0].y/tvVerticalResolution);
	range = min ( range, 255.0);
	float Y;
	vec3 	c;

	PROCESS(offset-2)
	PROCESS(offset-1)
	PROCESS(offset-0)
	PROCESS(offset+1)
	FINISH
	
	tempColor-=vec3(blackLevel);
	tempColor*=(contrast/vec3(1.0-blackLevel));
	tempColor=pow(tempColor,vec3(gamma));
	fragColor = vec4(tempColor, 1.0);
}