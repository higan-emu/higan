/*

   Hyllian's xBR MultiLevel4 Shader - Pass4
   
   Ported by Aliaspider.
   
   Copyright (C) 2011/2013 Hyllian/Jararaca - sergiogdb@gmail.com

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#version 150
#define ROUND(X) floor((X)+0.5)
#define ORIG_ID 3
#define bin		vec3( 4.0,  2.0,  1.0)

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
in Vertex {
   vec2 texCoord;
};

out vec4 fragColor;

vec4 remapFrom01(vec4 v)
{
	return floor((v*vec4(128.0))-vec4(63.5));
}

float c_df(vec3 c1, vec3 c2)
{
	vec3 df = abs(c1 - c2);
	return df.r + df.g + df.b;
}


float df(float A, float B)
{
	return abs(A-B);
}

#define GET_PIXEL(PARAM, PIXEL)\
	info = PARAM;\
	ay.z = ROUND( modf( info/2.0, info )  );\
	ay.y = ROUND( modf( info/2.0, info )  );\
	ay.x = ROUND( modf( info/2.0, info )  );\
	ax.z = ROUND( modf( info/2.0, info )  );\
	ax.y = ROUND( modf( info/2.0, info )  );\
	ax.x = ROUND(  info );\
	iq.x = dot( ax, bin ) - 2.0;\
	iq.y = dot( ay, bin ) - 2.0;\
	PIXEL = texture( source[ORIG_ID], pxcoord + iq*sourceSize[ORIG_ID].zw ).xyz;\


void main(void){

	float scale_factor = targetSize.x*sourceSize[ORIG_ID].z;

	vec2 fp = fract( texCoord*sourceSize[ORIG_ID].xy ) - vec2( 0.5, 0.5 );

	vec2 pxcoord = (floor(texCoord*sourceSize[ORIG_ID].xy)+vec2(0.5,0.5))*sourceSize[ORIG_ID].zw;
	
	vec4 UL = texture(source[0], pxcoord + vec2(-0.25,-0.25)*sourceSize[ORIG_ID].zw);
	vec4 UR = texture(source[0], pxcoord + vec2( 0.25,-0.25)*sourceSize[ORIG_ID].zw);
	vec4 DL = texture(source[0], pxcoord + vec2(-0.25, 0.25)*sourceSize[ORIG_ID].zw);
	vec4 DR = texture(source[0], pxcoord + vec2( 0.25, 0.25)*sourceSize[ORIG_ID].zw);

	vec4 ulparam = remapFrom01( UL ); // retrieve previous passes info
	vec4 urparam = remapFrom01( UR ); // retrieve previous passes info
	vec4 dlparam = remapFrom01( DL ); // retrieve previous passes info
	vec4 drparam = remapFrom01( DR ); // retrieve previous passes info

	vec3 E = texture( source[ORIG_ID], pxcoord ).xyz;

	vec3 ax, ay, PX, PY, PZ, PW;
	float info;
	vec2 iq;
	
	GET_PIXEL(ulparam.w, PX);
	GET_PIXEL(urparam.w, PY);
	GET_PIXEL(dlparam.w, PZ);
	GET_PIXEL(drparam.w, PW);

	vec3 fp1 = vec3( fp, -1 );

	vec3 color;
	vec4 fx;

	vec4 inc   = vec4(abs(ulparam.x/ulparam.y), abs(urparam.x/urparam.y), abs(dlparam.x/dlparam.y), abs(drparam.x/drparam.y));
	vec4 level = max(inc, 1.0/inc);

	fx.x    = clamp( dot( fp1, ulparam.xyz ) * scale_factor/( 8.0 * level.x ) + 0.5 ,0.0,1.0);
	fx.y    = clamp( dot( fp1, urparam.xyz ) * scale_factor/( 8.0 * level.y ) + 0.5 ,0.0,1.0);
	fx.z    = clamp( dot( fp1, dlparam.xyz ) * scale_factor/( 8.0 * level.z ) + 0.5 ,0.0,1.0);
	fx.w    = clamp( dot( fp1, drparam.xyz ) * scale_factor/( 8.0 * level.w ) + 0.5 ,0.0,1.0);

	vec3 c1, c2, c3, c4;

	c1 = mix( E, PX, fx.x );
	c2 = mix( E, PY, fx.y );
	c3 = mix( E, PZ, fx.z );
	c4 = mix( E, PW, fx.w );

	color = c1;
	color = ( (c_df(c2, E) > c_df(color, E)) ) ? c2 : color;
	color = ( (c_df(c3, E) > c_df(color, E)) ) ? c3 : color;
	color = ( (c_df(c4, E) > c_df(color, E)) ) ? c4 : color;

	fragColor=vec4( color, 1.0 );
}


