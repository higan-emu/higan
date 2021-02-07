#version 150

/*

   Hyllian's xBR MultiLevel4 Shader - Pass4
   
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
   float scale_factor;
};

out vec4 FragColor;

// compatibility macros
#define mul(a,b) (b*a)
#define lerp(a,b,c) mix(a,b,c)
#define saturate(c) clamp(c, 0.0, 1.0)
#define frac(x) (fract(x))
#define float2 vec2
#define float3 vec3
#define float4 vec4
#define bool2 bvec2
#define bool3 bvec3
#define bool4 bvec4
#define float3x3 mat3x3
#define float4x3 mat4x3
#define float2x4 mat2x4
#define texture_size (sourceSize[0].xy)
#define video_size (sourceSize[0].xy)
#define output_size (outputSize.xy)

#define round(X) floor((X)+0.5)

const float3 bin            = float3( 4.0,  2.0,  1.0);
const float4 low            = float4(-64.0, -64.0, -64.0, -64.0);
const float4 high           = float4( 64.0,  64.0,  64.0,  64.0);

const float2x4 sym_vectors  = float2x4(1.,  1.,   -1., -1.,    1., -1.,   -1.,  1.);

float4 remapFrom01(float4 v, float4 low, float4 high)
{
	return round(lerp(low, high, v));
}

float c_df(float3 c1, float3 c2)
{
	float3 df = abs(c1 - c2);
	return df.r + df.g + df.b;
}


float4 unpack_info(float i)
{
	float4 info;
	info.x = round(modf(i/2.0, i));
	info.y = round(modf(i/2.0, i));
	info.z = round(modf(i/2.0, i));
	info.w = i;

	return info;
}


float df(float A, float B)
{
	return abs(A-B);
}

#define GET_PIXEL(PARAM, PIXEL)\
	info = PARAM;\
	ay.z = round(  modf( info/1.9999, info )  );\
	ay.y = round(  modf( info/1.9999, info )  );\
	ay.x = round(  modf( info/1.9999, info )  );\
	ax.z = round(  modf( info/1.9999, info )  );\
	ax.y = round(  modf( info/1.9999, info )  );\
	ax.x = round(  info  );\
	iq.x = dot( ax, bin ) - 1.9999;\
	iq.y = dot( ay, bin ) - 1.9999;\
PIXEL = texture( source[3], vTexCoord + iq.x*t1.xy + iq.y*t1.zw ).xyz;\

void main() {
	float2 fp = frac( vTexCoord*sourceSize[0].xy ) - float2( 0.5, 0.5 ); // pos = pixel position

	float2 pxcoord = floor(vTexCoord*sourceSize[0].xy)/sourceSize[0].xy;

	float4 UL = texture(source[0], pxcoord + 0.25*t1.xy + 0.25*t1.zw );
	float4 UR = texture(source[0], pxcoord + 0.75*t1.xy + 0.25*t1.zw );
	float4 DL = texture(source[0], pxcoord + 0.25*t1.xy + 0.75*t1.zw );
	float4 DR = texture(source[0], pxcoord + 0.75*t1.xy + 0.75*t1.zw );

	float4 ulparam = remapFrom01( UL, low, high ); // retrieve 1st pass info
	float4 urparam = remapFrom01( UR, low, high ); // retrieve 1st pass info
	float4 dlparam = remapFrom01( DL, low, high ); // retrieve 1st pass info
	float4 drparam = remapFrom01( DR, low, high ); // retrieve 1st pass info

	float3 E = texture( source[3], vTexCoord ).xyz;

	float3 ax, ay, PX, PY, PZ, PW;
	float info;
	float2 iq;

#ifdef DEBUG
	PX = unpack_info(ulparam.w).xyz;
	PY = unpack_info(urparam.w).xyz;
	PZ = unpack_info(dlparam.w).xyz;
	PW = unpack_info(drparam.w).xyz;
#else	
	GET_PIXEL(ulparam.w, PX);
	GET_PIXEL(urparam.w, PY);
	GET_PIXEL(dlparam.w, PZ);
	GET_PIXEL(drparam.w, PW);
#endif

	float3 fp1 = float3( fp, -1. );

	float3 color;
	float4 fx;

	float4 inc   = float4(abs(ulparam.x/ulparam.y), abs(urparam.x/urparam.y), abs(dlparam.x/dlparam.y), abs(drparam.x/drparam.y));
	float4 level = max(inc, 1.0/inc);

	fx.x    = saturate( dot( fp1, ulparam.xyz ) * scale_factor/( 8. * level.x ) + 0.5 );
	fx.y    = saturate( dot( fp1, urparam.xyz ) * scale_factor/( 8. * level.y ) + 0.5 );
	fx.z    = saturate( dot( fp1, dlparam.xyz ) * scale_factor/( 8. * level.z ) + 0.5 );
	fx.w    = saturate( dot( fp1, drparam.xyz ) * scale_factor/( 8. * level.w ) + 0.5 );

	float3 c1, c2, c3, c4;

	c1 = lerp( E, PX, fx.x );
	c2 = lerp( E, PY, fx.y );
	c3 = lerp( E, PZ, fx.z );
	c4 = lerp( E, PW, fx.w );

	color = c1;
	color = ( (c_df(c2, E) > c_df(color, E)) ) ? c2 : color;
	color = ( (c_df(c3, E) > c_df(color, E)) ) ? c3 : color;
	color = ( (c_df(c4, E) > c_df(color, E)) ) ? c4 : color;

	FragColor = float4( color, 1.0 );
}