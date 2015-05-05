#version 150

////////////////////////////////////////////////////////
//	GTU-Famicom version 0.50	
//	Author: aliaspider - aliaspider@gmail.com
//	License: GPLv3      
////////////////////////////////////////////////////////


#in signalResolutionY
#in signalResolutionI
#in signalResolutionQ

#define YIQ_to_RGB 	mat3x3( 1.0   , 1.0      , 1.0      ,	0.9563   , -0.2721   , -1.1070   ,		0.6210   , -0.6474   , 1.7046   )


#define pi			3.14159265358
float STU0 (float x, float b){	
	x = x*b*pi;
	return clamp((x+sin(x))/(2.0*pi)+0.5,0.0,1.0);
}
float STU(float x, float b){
	return STU0(x+0.5,b)-STU0(x-0.5,b);
}

uniform sampler2D	source[];
uniform vec4		sourceSize[];

in Vertex {
	vec2 texCoord;
};
out vec4 fragColor;
#define PROCESS_YIQ(X) \
c=texture(source[0], vec2(texCoord.x - (X)*sourceSize[0].z,texCoord.y)).xyz;\
tempColor+=vec3((c.x*STU(X,(signalResolutionY*sourceSize[0].z))),\
                (c.y*STU(X,(signalResolutionI*sourceSize[0].z))),\
                (c.z*STU(X,(signalResolutionQ*sourceSize[0].z))));
#define PROCESS_IQ(X) \
c.yz=texture(source[0], vec2(texCoord.x - (X)*sourceSize[0].z,texCoord.y)).yz;\
tempColor.yz+=vec2((c.y*STU((X),(signalResolutionI*sourceSize[0].z))),\
                   (c.z*STU((X),(signalResolutionQ*sourceSize[0].z))));
#define PROCESS_Q(X) \
c.z=texture(source[0], vec2(texCoord.x - (X)*sourceSize[0].z,texCoord.y)).z;\
tempColor.z+=c.z*STU((X),(signalResolutionQ*sourceSize[0].z));

void main() {
	float	offset	= fract((texCoord.x * sourceSize[0].x) - 0.5);
	vec3	tempColor = vec3(0.0);	
	
	
	float range=ceil(0.5+sourceSize[0].x/min(min(signalResolutionY,signalResolutionI),signalResolutionQ));
	range = min ( range, 255.0);
	float X,i;
	vec3 	c;

	float rangeY=ceil(0.5+sourceSize[0].x/signalResolutionY);
	rangeY = min ( range, 255.0);
	float rangeI=ceil(0.5+sourceSize[0].x/signalResolutionI);
	rangeI = min ( range, 255.0);
	float rangeQ=ceil(0.5+sourceSize[0].x/signalResolutionQ);
	rangeQ = min ( range, 255.0);
	
	for (i=1.0-rangeQ;i<1.0-rangeI;i++){
		PROCESS_Q(offset-i)
	}
	for (i=1.0-rangeI;i<1.0-rangeY;i++){
		PROCESS_IQ(offset-i)	
	}
	for (i=1.0-rangeY;i<1.0+rangeY;i++){
		PROCESS_YIQ(offset-i)	
	}	
	for (i=1.0+rangeY;i<1.0+rangeI;i++){
		PROCESS_IQ(offset-i)
	}	
	for (i=1.0+rangeI;i<1.0+rangeQ;i++){
		PROCESS_Q(offset-i)
	}		
	
	tempColor.gb-=vec2(0.5,0.5);
	
	tempColor=clamp(YIQ_to_RGB*tempColor,0.0,1.0);
	fragColor.rgb = tempColor;
}
