#version 150

#in coloremulation
#define GRID_STRENGTH 0.15

#ifdef coloremulation
const uint curve[32] = {
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0e, 0x10, 0x12,
  0x14, 0x16, 0x18, 0x1c, 0x20, 0x28, 0x38, 0x38,
  0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x80,
  0x88, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0
};
#endif

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

void main() {
  vec4 rgba = texture(source[0], texCoord);
  vec4 gbacolor;
  #ifndef coloremulation
  gbacolor = rgba;
  #else
  uint R = curve[uint(rgba[0] * 31)];
  uint G = curve[uint(rgba[1] * 31)];
  uint B = curve[uint(rgba[2] * 31)];

  uint Rr = R * 16;
  uint Gr = R *  4;
  uint Br = R *  4;

  uint Rg = G *  8;
  uint Gg = G * 16;
  uint Bg = G *  8;

  uint Rb = B *  0;  //intentionally always zero
  uint Gb = B *  8;
  uint Bb = B * 16;

  // With no pointers, the shader language has no swap method.
  #define swap(a, b) {a ^= b; b ^= a; a ^= b;}
  if(Rr < Rg) swap(Rr, Rg);
  if(Rr < Rb) swap(Rr, Rb);
  if(Rg < Rb) swap(Rg, Rb);

  if(Gr < Gg) swap(Gr, Gg);
  if(Gr < Gb) swap(Gr, Gb);
  if(Gg < Gb) swap(Gg, Gb);

  if(Br < Bg) swap(Br, Bg);
  if(Br < Bb) swap(Br, Bb);
  if(Bg < Bb) swap(Bg, Bb);

  R = (((4 * Rr + 2 * Rg + Rb) * 160) >> 14) + 32;
  G = (((4 * Gr + 2 * Gg + Gb) * 160) >> 14) + 32;
  B = (((4 * Br + 2 * Bg + Bb) * 160) >> 14) + 32;

  gbacolor = vec4(
    R / 255.0,
    G / 255.0,
    B / 255.0,
    0.0
  );
  #endif
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

  vec2 bordert = clamp((border+vec2(0.0,+GRID_STRENGTH)) * subtexelSize,
		       vec2(left, bottom), vec2(right, top));
  vec2 borderb = clamp((border+vec2(0.0,-GRID_STRENGTH)) * subtexelSize,
		       vec2(left, bottom), vec2(right, top));
  float totalArea = 2.0 * range.y;

  vec4 averageColor;
  averageColor  = ((top - bordert.y)    / totalArea) * topLeftColor;
  averageColor += ((borderb.y - bottom) / totalArea) * bottomRightColor;
  averageColor += ((borderb.y - bottom) / totalArea) * bottomLeftColor;
  averageColor += ((top - bordert.y)    / totalArea) * topRightColor;
  
  fragColor = (gbacolor + pow(averageColor,vec4(1.0/gamma))) / 2.0;
}