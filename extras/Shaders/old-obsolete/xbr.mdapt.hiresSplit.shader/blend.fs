//this shader will blend the last output with the output of shader3
#define MAIN_SHADER_PASSES_COUNT 8

// blending in linear gamma produces better looking
// pseudo-hires transparancy
// disable for a small performance increase
#define BLEND_IN_LINEAR_GAMMA


#version 150

#define FIRST_OUTPUT (MAIN_SHADER_PASSES_COUNT+1)
uniform sampler2D source[];

in Vertex {
   vec2 texCoord;
};

out vec4 fragColor;


void main (void){

	vec3 c0 =texture(source[FIRST_OUTPUT], texCoord).rgb;
	vec3 c1 =texture(source[0], texCoord).rgb;
	
#ifdef BLEND_IN_LINEAR_GAMMA
	c0=pow(c0,vec3(2.2));
	c1=pow(c1,vec3(2.2));
#endif
	vec3 c=(c0+c1)*0.5;
#ifdef BLEND_IN_LINEAR_GAMMA
	c=pow(c,vec3(1.0/ 2.2));
#endif
	fragColor=vec4(c,1.0);
}


