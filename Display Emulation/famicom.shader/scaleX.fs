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
#define a(x) abs(x)
#define d(x,b) (pi*b*min(a(x)+0.5,1.0/b))
#define e(x,b) (pi*b*min(max(a(x)-0.5,-1.0/b),1.0/b))
#define STU(x,b) ((d(x,b)+sin(d(x,b))-e(x,b)-sin(e(x,b)))/(2.0*pi))
#define X(i) (offset-(i))
#define GETC (texture(source[0], vec2(tex.x - X*sourceSize[0].z,tex.y)).xyz)

#define VAL vec3((c.x*STU(X,(signalResolutionY*sourceSize[0].z))),(c.y*STU(X,(signalResolutionI*sourceSize[0].z))),(c.z*STU(X,(signalResolutionQ*sourceSize[0].z))))

#define PROCESS(i) X=X(i);c=GETC;tempColor+=VAL;

uniform sampler2D	source[];
uniform vec4		sourceSize[];

in Vertex {
	vec2 texCoord;
};
out vec4 fragColor;

void main() {
	vec2 	tex=texCoord.xy;
	float	offset	= fract((tex.x * sourceSize[0].x) - 0.5);
	vec3	tempColor = vec3(0.0);	
	float 	X;
	vec3 	c;	
	float range=ceil(0.5+sourceSize[0].x/min(min(signalResolutionY,signalResolutionI),signalResolutionQ));
	range = min ( range, 255.0);
	float i;
	for (i=1.0-range;i<1.0+range;i++){
		PROCESS(i)
	}	

	
	
	tempColor=clamp(YIQ_to_RGB*tempColor,0.0,1.0);
	fragColor = vec4(tempColor,1.0);
}
