#version 150

/*

   Hyllian's xBR MultiLevel4 Shader - Pass2
   
   Copyright (C) 2011-2015 Hyllian - sergiogdb@gmail.com

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

*/

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 outputSize;

in Vertex {
   vec2 vTexCoord;
   vec4 t1;
   vec4 t2;
   vec4 t3;
   vec4 t4;
   vec4 t5;
   vec4 t6;
   vec4 t7;
};

out vec4 FragColor;

// compatibility macros
#define mul(a,b) (b*a)
#define float2 vec2
#define float3 vec3
#define float4 vec4
#define bool2 bvec2
#define bool3 bvec3
#define bool4 bvec4
#define float3x3 mat3x3
#define float4x3 mat4x3
#define texture_size (sourceSize[0].xy)
#define video_size (sourceSize[0].xy)
#define output_size (outputSize.xy)

#define round(X) floor((X)+0.5)

const float cf2             = 2.0;
const float cf3             = 4.0;
const float cf4             = 4.0;
const float4 eq_threshold   = float4(15.0, 15.0, 15.0, 15.0);
const float4 eq_threshold2  = float4( 5.0,  5.0,  5.0,  5.0);
const float4 eq_threshold3  = float4(25.0, 25.0, 25.0, 25.0);
const float y_weight        = 48.0;
const float u_weight        = 7.0;
const float v_weight        = 6.0;
const float3x3 yuv          = float3x3(0.299, 0.587, 0.114, -0.169, -0.331, 0.499, 0.499, -0.418, -0.0813);
const float3x3 yuv_weighted = float3x3(y_weight*yuv[0], u_weight*yuv[1], v_weight*yuv[2]);
const float4 maximo         = float4(255.0, 255.0, 255.0, 255.0);

float4 df(float4 A, float4 B)
{
	return float4(abs(A-B));
}

bool4 rd(float4 A, float4 B, float4 C, float4 D)
{
    return greaterThan((df(C,D)/(df(A,B)+0.000000001)) , vec4(2.,2.,2.,2.));
}

bool4 id(float4 A, float4 B, float4 C, float4 D)
{
    return greaterThan(df(C,D) , df(A,B));
}

float4 remapTo01(float4 v, float4 high)
{
	return (v/high);
}

float4 remapFrom01(float4 v, float4 high)
{
	return round(high*v);
}


bool4 eq(float4 A, float4 B)
{
	return lessThan(df(A, B) , eq_threshold);
}

bool4 eq2(float4 A, float4 B)
{
	return lessThan(df(A, B) , eq_threshold2);
}

bool4 eq3(float4 A, float4 B)
{
	return lessThan(df(A, B) , eq_threshold3);
}

float4 weighted_distance(float4 a, float4 b, float4 c, float4 d, float4 e, float4 f, float4 g, float4 h)
{
	return (df(a,b) + df(a,c) + df(d,e) + df(d,f) + 4.0*df(g,h));
}

void main() {
	vec4 PA  = texture(source[0], t2.xw);
	vec4 PB  = texture(source[0], t2.yw);
	vec4 PC  = texture(source[0], t2.zw);

	vec4 PD  = texture(source[0], t3.xw);
	vec4 PE  = texture(source[0], t3.yw);
	vec4 PF  = texture(source[0], t3.zw);

	vec4 PG  = texture(source[0], t4.xw);
	vec4 PH  = texture(source[0], t4.yw);
	vec4 PI  = texture(source[0], t4.zw);
	
	vec3 A1 = texture(source[1], t1.xw).rgb;
	vec3 B1 = texture(source[1], t1.yw).rgb;
	vec3 C1 = texture(source[1], t1.zw).rgb;

	vec3 A  = texture(source[1], t2.xw).rgb;
	vec3 B  = texture(source[1], t2.yw).rgb;
	vec3 C  = texture(source[1], t2.zw).rgb;

	vec3 D  = texture(source[1], t3.xw).rgb;
	vec3 E  = texture(source[1], t3.yw).rgb;
	vec3 F  = texture(source[1], t3.zw).rgb;

	vec3 G  = texture(source[1], t4.xw).rgb;
	vec3 H  = texture(source[1], t4.yw).rgb;
	vec3 I  = texture(source[1], t4.zw).rgb;

	vec3 G5 = texture(source[1], t5.xw).rgb;
	vec3 H5 = texture(source[1], t5.yw).rgb;
	vec3 I5 = texture(source[1], t5.zw).rgb;

	vec3 A0 = texture(source[1], t6.xy).rgb;
	vec3 D0 = texture(source[1], t6.xz).rgb;
	vec3 G0 = texture(source[1], t6.xw).rgb;

	vec3 C4 = texture(source[1], t7.xy).rgb;
	vec3 F4 = texture(source[1], t7.xz).rgb;
	vec3 I4 = texture(source[1], t7.xw).rgb;
	
	float4 b = mul( float4x3(B, D, H, F), yuv_weighted[0] );
	float4 c = mul( float4x3(C, A, G, I), yuv_weighted[0] );
	float4 e = mul( float4x3(E, E, E, E), yuv_weighted[0] );
	float4 d = b.yzwx;
	float4 f = b.wxyz;
	float4 g = c.zwxy;
	float4 h = b.zwxy;
	float4 i = c.wxyz;

	float4 i4 = mul( float4x3(I4, C1, A0, G5), yuv_weighted[0] );
	float4 i5 = mul( float4x3(I5, C4, A1, G0), yuv_weighted[0] );
	float4 h5 = mul( float4x3(H5, F4, B1, D0), yuv_weighted[0] );
	float4 f4 = h5.yzwx;

	float4 pe = remapFrom01(PE, maximo);
	float4 pf = remapFrom01(PF, maximo);
	float4 ph = remapFrom01(PH, maximo);
	float4 pb = remapFrom01(PB, maximo);
	float4 pd = remapFrom01(PD, maximo);

	float4 f2 = float4(pf.z, pb.w, pd.x, ph.y);
	float4 h2 = float4(ph.z, pf.w, pb.x, pd.y);
	float4 f1 = float4(pf.y, pb.z, pd.w, ph.x);
	float4 h3 = float4(ph.w, pf.x, pb.y, pd.z);

	bool4 nbrs;	
//	bool4 nbrs = ((pe.yzwx > 1.0) || (pe.wxyz > 1.0)) ? bool4(true) : bool4(false);
	nbrs.x =     ((pe.y    > 1.0) || (pe.w    > 1.0));
	nbrs.y =     ((pe.z    > 1.0) || (pe.x    > 1.0));
	nbrs.z =     ((pe.w    > 1.0) || (pe.y    > 1.0));
	nbrs.w =     ((pe.x    > 1.0) || (pe.z    > 1.0));

	bool4 jag1;	
//	bool4 jag1 = ((f2   > 1.0) || (h2   > 1.0)) ? bool4(true) : bool4(false);
	jag1.x =     ((f2.x > 1.0) || (h2.x > 1.0));
	jag1.y =     ((f2.y > 1.0) || (h2.y > 1.0));
	jag1.z =     ((f2.z > 1.0) || (h2.z > 1.0));
	jag1.w =     ((f2.w > 1.0) || (h2.w > 1.0));

	bool4 jag2;	
//	bool4 jag2 = ((f2   > 2.0) || (h2   > 2.0)) ? bool4(true) : bool4(false);
	jag2.x =     ((f2.x > 2.0) || (h2.x > 2.0));
	jag2.y =     ((f2.y > 2.0) || (h2.y > 2.0));
	jag2.z =     ((f2.z > 2.0) || (h2.z > 2.0));
	jag2.w =     ((f2.w > 2.0) || (h2.w > 2.0));

	bool4 jag3;	
//	bool4 jag3 = ((f2   > 4.0) || (h2   > 4.0)) ? bool4(true) : bool4(false);
	jag3.x =     ((f2.x > 4.0) || (h2.x > 4.0));
	jag3.y =     ((f2.y > 4.0) || (h2.y > 4.0));
	jag3.z =     ((f2.z > 4.0) || (h2.z > 4.0));
	jag3.w =     ((f2.w > 4.0) || (h2.w > 4.0));
	
//	pe =   (pe   == 7.0 || pe   == 8.0) ? ((jag3  ) ? pe   : (pe   - float(2.0))) : pe;
	pe.x = (pe.x == 7.0 || pe.x == 8.0) ? ((jag3.x) ? pe.x : (pe.x - float(2.0))) : pe.x;
	pe.y = (pe.y == 7.0 || pe.y == 8.0) ? ((jag3.y) ? pe.y : (pe.y - float(2.0))) : pe.y;
	pe.z = (pe.z == 7.0 || pe.z == 8.0) ? ((jag3.z) ? pe.z : (pe.z - float(2.0))) : pe.z;
	pe.w = (pe.w == 7.0 || pe.w == 8.0) ? ((jag3.w) ? pe.w : (pe.w - float(2.0))) : pe.w;
	
//	pe =   (pe   == 5.0 || pe   == 6.0) ? ((jag2  ) ? pe   : (pe   - float(2.0))) : pe;
	pe.x = (pe.x == 5.0 || pe.x == 6.0) ? ((jag2.x) ? pe.x : (pe.x - float(2.0))) : pe.x;
	pe.y = (pe.y == 5.0 || pe.y == 6.0) ? ((jag2.y) ? pe.y : (pe.y - float(2.0))) : pe.y;
	pe.z = (pe.z == 5.0 || pe.z == 6.0) ? ((jag2.z) ? pe.z : (pe.z - float(2.0))) : pe.z;
	pe.w = (pe.w == 5.0 || pe.w == 6.0) ? ((jag2.w) ? pe.w : (pe.w - float(2.0))) : pe.w;

	bool4 jag91;	
//	bool4 jag91 = ((id(h,i,e,h)   || id(i4,i,f4,i4)  ) && (f2   > 1.0) && (f1   > 1.0));
	jag91.x =     ((id(h,i,e,h).x || id(i4,i,f4,i4).x) && (f2.x > 1.0) && (f1.x > 1.0));
	jag91.y =     ((id(h,i,e,h).y || id(i4,i,f4,i4).y) && (f2.y > 1.0) && (f1.y > 1.0));
	jag91.z =     ((id(h,i,e,h).z || id(i4,i,f4,i4).z) && (f2.z > 1.0) && (f1.z > 1.0));
	jag91.w =     ((id(h,i,e,h).w || id(i4,i,f4,i4).w) && (f2.w > 1.0) && (f1.w > 1.0));

	bool4 jag92;
//	bool4 jag92 = ((id(f,i,e,f)   || id(i5,i,h5,i5)  ) && (h2   > 1.0) && (h3   > 1.0));
	jag92.x =     ((id(f,i,e,f).x || id(i5,i,h5,i5).x) && (h2.x > 1.0) && (h3.x > 1.0));
	jag92.y =     ((id(f,i,e,f).y || id(i5,i,h5,i5).y) && (h2.y > 1.0) && (h3.y > 1.0));
	jag92.z =     ((id(f,i,e,f).z || id(i5,i,h5,i5).z) && (h2.z > 1.0) && (h3.z > 1.0));
	jag92.w =     ((id(f,i,e,f).w || id(i5,i,h5,i5).w) && (h2.w > 1.0) && (h3.w > 1.0));
	
	bvec4 jag93 = ( rd(h,g,e,g));
	bvec4 jag94 = ( rd(f,c,e,c));

	bool4 jag9;
//	bool4 jag9  = (!(jag91   && jag93   || jag92   && jag94  ));
	jag9.x =      (!(jag91.x && jag93.x || jag92.x && jag94.x));
	jag9.y =      (!(jag91.y && jag93.y || jag92.y && jag94.y));
	jag9.z =      (!(jag91.z && jag93.z || jag92.z && jag94.z));
	jag9.w =      (!(jag91.w && jag93.w || jag92.w && jag94.w));

//	pe   = ((pe   == 0.0) || (!nbrs   || jag1  ) && jag9  ) ? pe   : float4(1.0);
	pe.x = ((pe.x == 0.0) || (!nbrs.x || jag1.x) && jag9.x) ? pe.x :  float(1.0);
	pe.y = ((pe.y == 0.0) || (!nbrs.y || jag1.y) && jag9.y) ? pe.y :  float(1.0);
	pe.z = ((pe.z == 0.0) || (!nbrs.z || jag1.z) && jag9.z) ? pe.z :  float(1.0);
	pe.w = ((pe.w == 0.0) || (!nbrs.w || jag1.w) && jag9.w) ? pe.w :  float(1.0);
	
FragColor = vec4(remapTo01(pe, maximo));
}