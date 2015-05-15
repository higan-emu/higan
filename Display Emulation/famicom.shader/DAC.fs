#version 150

////////////////////////////////////////////////////////
//	GTU-Famicom version 0.50	
//	Author: aliaspider - aliaspider@gmail.com
//	License: GPLv3      
////////////////////////////////////////////////////////

#define RGB_to_YIQ 	mat3x3( 0.299 , 0.595716 , 0.211456 ,	0.587    , -0.274453 , -0.522591 ,		0.114    , -0.321263 , 0.311135 )


uniform sampler2D	source[];
uniform vec4		sourceSize[];
uniform vec4 		targetSize;
uniform int 		phase;

in Vertex {
	vec2 texCoord;
	float colorPhase;
};
out vec4 fragColor;

#define TO_INT2(X) int(floor(((X)*3.0)+0.5))
#define TO_INT3(X) int(floor(((X)*7.0)+0.5))
#define TO_INT4(X) int(floor(((X)*15.0)+0.5))

bool InColorp (int p, int color) { 
  return ((color + p) % 12 < 6); 
}

float NTSCsignal(int emphasis, int level, int color, int p)
{
    // float black=.518;
	// float white=1.962;
	float attenuation=0.746;
    const float levels[8] = float[] (0.350 , 0.518, 0.962, 1.550,  
							  1.094f, 1.506, 1.962, 1.962); 
    if(color > 13)  level = 1;
    float low  = levels[0 + level];
    float high = levels[4 + level];
    if(color == 0) low = high;
    if(color > 12) high = low;

    
    float signal = InColorp(p, color) ? high : low;

    if( (bool(emphasis & 1) && InColorp(p, 0))
    ||  (bool(emphasis & 2) && InColorp(p, 4))
    ||  (bool(emphasis & 4) && InColorp(p, 8)) ) signal = signal * attenuation;

    return signal;
}


void main() {
	vec4 c=texture2D(source[0], texCoord.xy);
	
	int emphasis = TO_INT3(c.x);
	int level    = TO_INT2(c.y);
	int color    = TO_INT4(c.z);
	
	// overscan fix
	// if (c.a==0.0){
		// emphasis=0;
		// level=1;
		// color=13;
	// }
	
	float signal =NTSCsignal(emphasis,level,color,int(colorPhase + 0.5));	
	
	
	fragColor = vec4(signal);
}
