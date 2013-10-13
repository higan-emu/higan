#version 150

#define round(x) floor( (x) + 0.5 )
#define gamma 2.2
#define TEX2D(c) pow(texture2D(source[0], (c)), vec4(gamma))

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

float intsmear_func(float z)
{
  float z2 = z*z;
  float z4 = z2*z2;
  float z8 = z4*z4;
  return z - 2.0/3.0*z*z2 - 1.0/5.0*z*z4 + 4.0/7.0*z*z2*z4 - 1.0/9.0*z*z8
    - 2.0/11.0*z*z2*z8 + 1.0/13.0*z*z4*z8;
}
float intsmear(float x, float dx)
{
  const float d = 1.5;
  float zl = clamp((x-dx)/d,-1.0,1.0);
  float zh = clamp((x+dx)/d,-1.0,1.0);
  return d * ( intsmear_func(zh) - intsmear_func(zl) )/(2.0*dx);
}

void main()
{
  vec2 texelSize = 1.0 / sourceSize[0].xy;
  vec2 subtexelSize = texelSize / vec2(3.0,1.0);
  vec2 range = sourceSize[0].xy / (targetSize.xy * sourceSize[0].xy);

  float left   = texCoord.x - texelSize.x*0.5;
  float top    = texCoord.y + range.y;
  float right  = texCoord.x + texelSize.x*0.5;
  float bottom = texCoord.y - range.y;

  vec4 lcol, rcol;
  float subpix = mod(texCoord.x/subtexelSize.x+1.5,3.0);
  float rsubpix = range.x/subtexelSize.x;
  lcol = vec4(intsmear(subpix+1.0,rsubpix),intsmear(subpix    ,rsubpix),
	      intsmear(subpix-1.0,rsubpix),0.0);
  rcol = vec4(intsmear(subpix-2.0,rsubpix),intsmear(subpix-3.0,rsubpix),
	      intsmear(subpix-4.0,rsubpix),0.0);

  vec4 topLeftColor     = TEX2D((floor(vec2(left, top)     / texelSize) + 0.5) * texelSize) * lcol;
  vec4 bottomRightColor = TEX2D((floor(vec2(right, bottom) / texelSize) + 0.5) * texelSize) * rcol;
  vec4 bottomLeftColor  = TEX2D((floor(vec2(left, bottom)  / texelSize) + 0.5) * texelSize) * lcol;
  vec4 topRightColor    = TEX2D((floor(vec2(right, top)    / texelSize) + 0.5) * texelSize) * rcol;

  vec2 border = round(texCoord.st/subtexelSize);
  float shry = 0.05;
  vec2 bordert = clamp((border+vec2(0.0,+shry)) * subtexelSize,
		       vec2(left, bottom), vec2(right, top));
  vec2 borderb = clamp((border+vec2(0.0,-shry)) * subtexelSize,
		       vec2(left, bottom), vec2(right, top));
  float totalArea = 2.0 * range.y;

  vec4 averageColor;
  averageColor  = ((top - bordert.y)    / totalArea) * topLeftColor;
  averageColor += ((borderb.y - bottom) / totalArea) * bottomRightColor;
  averageColor += ((borderb.y - bottom) / totalArea) * bottomLeftColor;
  averageColor += ((top - bordert.y)    / totalArea) * topRightColor;

fragColor = pow(averageColor,vec4(1.0/gamma));
}