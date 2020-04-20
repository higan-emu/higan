#version 150

////////////////////////////////////////////////////////
//	GTU version 0.50	
//	Author: aliaspider - aliaspider@gmail.com
//	License: GPLv3      
////////////////////////////////////////////////////////

#in compositeConnection

#define RGB_to_YIQ 	mat3x3( 0.299 , 0.595716 , 0.211456 ,	0.587    , -0.274453 , -0.522591 ,		0.114    , -0.321263 , 0.311135 )


uniform sampler2D	source[];
uniform vec4		sourceSize[];

in Vertex {
	vec2 texCoord;
};
out vec4 fragColor;

#define TO_5_BIT(X) floor(((X)*31.0)+0.5)
#define TO_4_BIT(X) floor(((X)*15.0)+0.5)

void main() {
	vec4 c=texture2D(source[0], texCoord.xy);
	c.r=TO_5_BIT(c.r);
	c.g=TO_5_BIT(c.g);
	c.b=TO_5_BIT(c.b);
	c.a=TO_4_BIT(c.a);	
	c.a=(c.a==0.0)?1.0/32.0:(1.0+c.a)/16.0;
	c.rgb*=c.a;	
	c.rgb/=vec3(31.0);
	
#ifdef compositeConnection
	c.rgb=RGB_to_YIQ*c.rgb;
#endif
	
	fragColor = vec4(c.rgb, 1.0);	
}
