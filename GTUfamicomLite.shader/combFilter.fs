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
#define GET_LEVEL(X) ((X)*(255.0f / (128.0f*(1.962f-.518f)))-(.518f / (1.962f-.518f)))

void main() {
	
	float input=GET_LEVEL(texture(source[0],texCoord.xy).r);
	float signal=0.5f*(input+GET_LEVEL(texture(source[0],texCoord.xy-vec2(6.0*targetSize.z,0.0)).r));
	
	float chromaSignal=input-signal;
	float I = chromaSignal * cos (colorPhase * (2.0 * pi / 12.0))*2.0;
	float Q = chromaSignal * sin (colorPhase * (2.0 * pi / 12.0))*2.0;	

	fragColor.rgb = vec3(signal,I+0.5,Q+0.5);
}
