#version 150

uniform sampler2D source[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

#define saturation 1.0
#define luminance 1.00
#define gamma vec3(1.7)

vec3 desaturate(vec3 col)
{
   return vec3(dot(col, vec3(0.2126, 0.7152, 0.0722)));
}




void main() {
//combine
	// the LUT average color is vec3 (0.498,0.572,0.644)
	// multiplying it with sum will compensate for running the gaussian pass on the original texture instead 
	// of running it on the output of shadhow-mask.fs
	// had to use 0.528 for red instead of 0.498 , not sure why
   	vec3 image = texture(source[1], texCoord).rgb*vec3 (0.528,0.572,0.644);
	
	vec3 previous = texture(source[0], texCoord).rgb;
	vec3 res = 1.0 - (1.0 - image) * (1.0 - image) * (1.0 - previous);
//compensate for the missing gaussian-scanline.fs pass
	res*=1.05;
	
   res = mix(desaturate(res), res, saturation);
   res = pow(res, gamma);
   res *= luminance;

   fragColor = vec4(res , 1.0);
}