#version 150

////////////////////////////////////////////////////////
//	GTU-Famicom version 0.50	
//	Author: aliaspider - aliaspider@gmail.com
//	License: GPLv3      
////////////////////////////////////////////////////////


uniform sampler2D	source[];
uniform vec4		sourceSize[];
uniform int 		phase;

#in signalResolution
#in addNoise
#in noiseStrengh

in Vertex {
	vec2 texCoord;
};
out vec4 fragColor;
#define pi			3.14159265358
#define a(x) abs(x)
#define d(x,b) (pi*b*min(a(x)+0.5,1.0/b))
#define e(x,b) (pi*b*min(max(a(x)-0.5,-1.0/b),1.0/b))
#define STU(x,b) ((d(x,b)+sin(d(x,b))-e(x,b)-sin(e(x,b)))/(2.0*pi))
#define X(i) (offset-(i))
#define S(i) (texture(source[0], vec2(texCoord.x - X(i)*sourceSize[0].z,texCoord.y)).x)
#define VAL(i) (S(i)*STU(X(i),(signalResolution*sourceSize[0].z)))

#in phaseOffset

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
	
	float offset	= fract((texCoord.x * sourceSize[0].x) - 0.5);

	float signal = 0.0;
	
	float range=ceil(0.5+sourceSize[0].x/signalResolution);
	range = min ( range, 255.0);

	float i;
	for (i=1-range;i<1+range;i++){
		signal+=VAL(i);
	}
	
	float black=.518f, white=1.962f;
    signal = (signal-black) / (white-black);
#ifdef addNoise
	vec2 pos =(texCoord.xy * sourceSize[0].xy);
	signal -=0.5;
	signal +=(rand(vec2(pos.x*pos.y,phase))-0.5)*noiseStrengh;
	signal +=0.5;
#endif
	
	fragColor = vec4(signal);	

}
