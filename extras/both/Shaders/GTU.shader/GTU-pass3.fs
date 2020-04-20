#version 150

////////////////////////////////////////////////////////
//	GTU version 0.40	
//	Author: aliaspider - aliaspider@gmail.com
//	License: GPLv3      
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
// SETTINGS 
////////////////////////////////////////////////////////

//#define	CROP_OVERSCAN
#define	TV_COLOR_LEVELS
//#define	COMPOSITE_CONNECTION
//#define	NO_SCANLINES
#define	TV_HORIZONTAL_RESOLUTION	400.0
#define	TV_VERTICAL_RESOLUTION	300.0
#define	SIGNAL_RESOLUTION	280.0
#define	SIGNAL_RESOLUTION_I	83.0
#define	SIGNAL_RESOLUTION_Q	25.0
#define	TV_DISPLAY_GAMMA	2.4
#define	OUTPUT_DISPLAY_GAMMA	2.2

////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////



#define pi			3.14159265358
#define a(x) abs(x)
#define d(x,b) (pi*b*min(a(x)+0.5,1.0/b))
#define e(x,b) (pi*b*min(max(a(x)-0.5,-1.0/b),1.0/b))
#define STU(x,b) ((d(x,b)+sin(d(x,b))-e(x,b)-sin(e(x,b)))/(2.0*pi))
#define X(i) (offset-(i))

#define GETC (texture(source[0], vec2(texCoord.x - X*sourceSize[0].z,texCoord.y)).xyz)
#define VAL (c*STU(X,(TV_HORIZONTAL_RESOLUTION*sourceSize[0].z)))

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
	PROCESS(-6)PROCESS(-5)PROCESS(-4)PROCESS(-3)PROCESS(-2)PROCESS(-1)PROCESS( 0)
	PROCESS( 7)PROCESS( 6)PROCESS( 5)PROCESS( 4)PROCESS( 3)PROCESS( 2)PROCESS( 1)
	
	// tempColor=pow(tempColor,vec3(1.0/2.2));
	fragColor = vec4(tempColor,1.0);
}
