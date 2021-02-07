#version 150
#define MAIN_SHADER_PASSES_COUNT 8
#define NUMBER_OF_PREVIOUS_PASSES (MAIN_SHADER_PASSES_COUNT+1)
uniform sampler2D source[];

in Vertex {
   vec2 texCoord;
};

out vec4 fragColor;


void main(void){

	fragColor=texture(source[NUMBER_OF_PREVIOUS_PASSES],texCoord);

}

