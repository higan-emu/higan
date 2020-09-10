#version 150

////////////////////////////////////////////////////////
//	GTU version 0.50	
//	Author: aliaspider - aliaspider@gmail.com
//	License: GPLv3      
////////////////////////////////////////////////////////

#in compositeConnection
#in signalResolution
#in signalResolutionI
#in signalResolutionQ



#define YIQ_to_RGB 	mat3x3( 1.0   , 1.0      , 1.0      ,	0.9563   , -0.2721   , -1.1070   ,		0.6210   , -0.6474   , 1.7046   )




#define pi			3.14159265358
#define a(x) abs(x)
#define d(x,b) (pi*b*min(a(x)+0.5,1.0/b))
#define e(x,b) (pi*b*min(max(a(x)-0.5,-1.0/b),1.0/b))
#define STU(x,b) ((d(x,b)+sin(d(x,b))-e(x,b)-sin(e(x,b)))/(2.0*pi))
#define X(i) (offset-(i))
#define GETC (texture(source[0], vec2(texCoord.x - X*sourceSize[0].z,texCoord.y)).xyz)

#ifdef compositeConnection
#define VAL vec3((c.x*STU(X,(signalResolution*sourceSize[0].z))),(c.y*STU(X,(signalResolutionI*sourceSize[0].z))),(c.z*STU(X,(signalResolutionQ*sourceSize[0].z))))
#else
#define VAL (c*STU(X,(signalResolution*sourceSize[0].z)))
#endif //COMPOSITE_CONNECTION

#define PROCESS(i) X=X(i);c=GETC;tempColor+=VAL;

uniform sampler2D	source[];
uniform vec4		sourceSize[];

in Vertex {
	vec2 texCoord;
};
out vec4 fragColor;


void main() {
	
	float	offset	= fract((texCoord.x * sourceSize[0].x) - 0.5);
	vec3	tempColor = vec3(0.0);	
	float 	X;
	vec3 	c;	
#ifdef compositeConnection	
	float range=ceil(0.5+sourceSize[0].x/min(min(signalResolution,signalResolutionI),signalResolutionQ));
#else
	float range=ceil(0.5+sourceSize[0].x/signalResolution);
#endif
	float i;
	for (i=-range;i<range+2.0;i++){
		PROCESS(i)
	}
	
#ifdef compositeConnection
	tempColor=clamp(YIQ_to_RGB*tempColor,0.0,1.0);
#endif
	tempColor=clamp(tempColor,0.0,1.0);
	fragColor = vec4(tempColor,1.0);
}
