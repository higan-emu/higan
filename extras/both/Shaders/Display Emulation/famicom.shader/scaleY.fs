#version 150

////////////////////////////////////////////////////////
//	GTU version 0.50	
//	Author: aliaspider - aliaspider@gmail.com
//	License: GPLv3      
////////////////////////////////////////////////////////


#in tvVerticalResolution
#in noScanlines
#in blackLevel
#in contrast
#in gamma

#define SCANLINE_WIDTH (1.5*sourceSize[0].y/tvVerticalResolution)

uniform sampler2D	source[];
uniform sampler2D 	texture[];
uniform vec4		sourceSize[];
uniform vec4		targetSize;

in Vertex {
	vec2 texCoord;
};
out vec4 fragColor;

#define pi			3.14159265358
#define normalGauss(x) ((exp(-(x)*(x)*0.5))/sqrt(2.0*pi))
float normalGaussIntegral(float x)
{
	float a1 = 0.4361836;
	float a2 = -0.1201676;
	float a3 = 0.9372980;
	float p = 0.3326700;
	float t = 1.0 / (1.0 + p*abs(x));
	return (0.5-normalGauss(x) * (t*(a1 + t*(a2 + a3*t))))*sign(x);
}


vec3 scanlines( float x , vec3 c){
	float temp=sqrt(2*pi)*(tvVerticalResolution*sourceSize[0].w);
	
	float rrr=0.5*(sourceSize[0].y*targetSize.w);
	float x1=(x+rrr)*temp;
	float x2=(x-rrr)*temp;
	c.r=(c.r*(normalGaussIntegral(x1)-normalGaussIntegral(x2)));	
	c.g=(c.g*(normalGaussIntegral(x1)-normalGaussIntegral(x2)));	
	c.b=(c.b*(normalGaussIntegral(x1)-normalGaussIntegral(x2)));
	c*=(sourceSize[0].w*targetSize.y);
	return c;
}

#define Y(j) (offset.y-(j))
#define a(x) abs(x)
#define d(x,b) (pi*b*min(a(x)+0.5,1.0/b))
#define e(x,b) (pi*b*min(max(a(x)-0.5,-1.0/b),1.0/b))
#define STU(x,b) ((d(x,b)+sin(d(x,b))-e(x,b)-sin(e(x,b)))/(2.0*pi))

#define SOURCE(j) vec2(texCoord.x,texCoord.y - Y(j)*sourceSize[0].w)
#define C(j) (texture2D(source[0], SOURCE(j)).xyz)

#ifdef noScanlines
#define VAL(j) (C(j)*STU(Y(j),(tvVerticalResolution*sourceSize[0].w)))
#else
#define VAL(j) (scanlines(Y(j),C(j)))
#endif



void main() {
	
	vec2	offset	= fract((texCoord.xy * sourceSize[0].xy) - 0.5);
	vec3	tempColor = vec3(0.0);	

	float range=ceil(0.5+sourceSize[0].y/tvVerticalResolution);
	range = min ( range, 255.0);
	float i;
	for (i=1.0-range;i<1.0+range;i++){
		tempColor+=VAL(i);
	}	
	tempColor-=vec3(blackLevel);
	tempColor*=(contrast/vec3(1.0-blackLevel));
	tempColor=pow(tempColor,vec3(gamma));
	fragColor = vec4(tempColor, 1.0);
}