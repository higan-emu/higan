#version 150

#in coloremulation
#in red
#in green
#in blue
#in gain
#in gamma
#in blacklevel
#in ambient
#in BGR
#in persistence

#define outgamma 2.2

#ifdef coloremulation
const uint curve[32] = {
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0e, 0x10, 0x12,
  0x14, 0x16, 0x18, 0x1c, 0x20, 0x28, 0x38, 0x38,
  0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x80,
  0x88, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0
};
#endif

#define fetch_offset(coord,offset) (pow(vec3(gain) * texelFetchOffset(source[0], (coord), 0, (offset)).rgb + vec3(blacklevel), vec3(gamma)) + vec3(ambient))

// integral of (1 - x^2 - x^4 + x^6)^2
const float coeffs_x[] = float[](1.0, -2.0/3.0, -1.0/5.0, 4.0/7.0, -1.0/9.0, -2.0/11.0, 1.0/13.0);
// integral of (1 - 2x^4 + x^6)^2
const float coeffs_y[] = float[](1.0,      0.0, -4.0/5.0, 2.0/7.0,  4.0/9.0, -4.0/11.0, 1.0/13.0); 
float intsmear_func(float z, float coeffs[7])
{
  float z2 = z*z;
  float zn = z;
  float ret = 0.0;
  for (int i = 0; i < 7; i++) {
    ret += zn*coeffs[i];
    zn *= z2;
  }
  return ret;
}
float intsmear(float x, float dx, float d, float coeffs[7])
{
  float zl = clamp((x-dx*0.5)/d,-1.0,1.0);
  float zh = clamp((x+dx*0.5)/d,-1.0,1.0);
  return d * ( intsmear_func(zh,coeffs) - intsmear_func(zl,coeffs) )/dx;
}

#define round(x) floor( (x) + 0.5 )
#define gamma 2.2

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
uniform sampler2D history[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
//motion blur
vec3 color = texture(history[7], texCoord).rgb;
color = mix(texture(history[6], texCoord).rgb, color, persistence);
color = mix(texture(history[5], texCoord).rgb, color, persistence);
color = mix(texture(history[4], texCoord).rgb, color, persistence);
color = mix(texture(history[3], texCoord).rgb, color, persistence);
color = mix(texture(history[2], texCoord).rgb, color, persistence);
color = mix(texture(history[1], texCoord).rgb, color, persistence);
color = mix(texture(history[0], texCoord).rgb, color, persistence);
color = mix(texture(source[0], texCoord).rgb, color, persistence);
  vec4 rgba = vec4(pow(color, vec3(gamma)), 1.0);

//palette shift
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
  
//pixel grid
  vec2 texelSize = 1.0 / sourceSize[0].xy;
  vec2 range = sourceSize[0].xy / (targetSize.xy * sourceSize[0].xy);

  vec3 cred   = pow(red,   vec3(outgamma));
  vec3 cgreen = pow(green, vec3(outgamma));
  vec3 cblue  = pow(blue,  vec3(outgamma));

  ivec2 tli = ivec2(floor(texCoord/texelSize-vec2(0.4999)));

  vec3 lcol, rcol;
  float subpix = (texCoord.x/texelSize.x - 0.4999 - float(tli.x))*3.0;
  float rsubpix = range.x/texelSize.x * 3.0;
  lcol = vec3(intsmear(subpix+1.0,rsubpix, 1.5, coeffs_x),
              intsmear(subpix    ,rsubpix, 1.5, coeffs_x),
              intsmear(subpix-1.0,rsubpix, 1.5, coeffs_x));
  rcol = vec3(intsmear(subpix-2.0,rsubpix, 1.5, coeffs_x),
              intsmear(subpix-3.0,rsubpix, 1.5, coeffs_x),
              intsmear(subpix-4.0,rsubpix, 1.5, coeffs_x));
#ifdef BGR
  lcol.rgb = lcol.bgr;
  rcol.rgb = rcol.bgr;
#endif
  float tcol, bcol;
  subpix = texCoord.y/texelSize.y - 0.4999 - float(tli.y);
  rsubpix = range.y/texelSize.y;
  tcol = intsmear(subpix    ,rsubpix, 0.63, coeffs_y);
  bcol = intsmear(subpix-1.0,rsubpix, 0.63, coeffs_y);

  vec3 topLeftColor     = fetch_offset(tli, ivec2(0,0)) * lcol * vec3(tcol);
  vec3 bottomRightColor = fetch_offset(tli, ivec2(1,1)) * rcol * vec3(bcol);
  vec3 bottomLeftColor  = fetch_offset(tli, ivec2(0,1)) * lcol * vec3(bcol);
  vec3 topRightColor    = fetch_offset(tli, ivec2(1,0)) * rcol * vec3(tcol);

  vec3 averageColor = topLeftColor + bottomRightColor + bottomLeftColor + topRightColor;

  averageColor = mat3x3(cred, cgreen, cblue) * averageColor;
  vec4 intermed = vec4(averageColor, 1.0);
  
  fragColor =  (gbacolor + pow(intermed,vec4(1.0/gamma))) / 2.0;
}