#version 150

////////////////////////////////////////////////////////
//	GTU-Famicom version 0.50	
//	Author: aliaspider - aliaspider@gmail.com
//	License: GPLv3      
////////////////////////////////////////////////////////


uniform sampler2D	source[];
uniform vec4 		targetSize;

in Vertex {
	vec2 texCoord;
	float colorPhase;
};
out vec4 fragColor;
#define pi			3.14159265358

#in combFilter

void main() {

	float current=texture(source[0],texCoord.xy).r;	
	
#ifdef combFilter
	float prev6=texture(source[0],texCoord.xy-vec2(6.0*targetSize.z,0.0));
	float signal=(current+prev6)/2.0;
	float chromaSignal=current-signal;
	float I = chromaSignal * cos (colorPhase * (2.0 * pi / 12.0))*2.0;
	float Q = chromaSignal * sin (colorPhase * (2.0 * pi / 12.0))*2.0;	
	
#else
	float signal=current;
	float I = signal * cos (colorPhase * (2.0 * pi / 12.0))*2.0;
	float Q = signal * sin (colorPhase * (2.0 * pi / 12.0))*2.0;
#endif
	
	fragColor = vec4(signal,I,Q,1.0);	
}
