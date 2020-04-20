/*

   Hyllian's xBR MultiLevel4 Shader - Pass3
   
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
#define ORIG_ID 2
#define y_weighted			(vec3(0.299, 0.587, 0.114)* 48.0)
#define maximo				vec4(256.0)
#define low					vec4(-64.0)
#define high				vec4( 64.0)


#define sym_vectors			transpose(mat2x4(1.0,  1.0,   -1.0, -1.0,    1.0, -1.0,   -1.0,  1.0))

// Bx, Ay, C
vec3 lines[13] = vec3 [](
	vec3(4.0, 4.0, 4.0),  //  0  NL
	vec3(4.0, 4.0, 3.0),  //  1  LV0
	vec3(4.0, 4.0, 2.0),  //  2  LV1
	vec3(8.0, 4.0, 2.0),  //  3  LV2u
	vec3(4.0, 8.0, 2.0),  //  4  LV2l
	vec3(12.0, 4.0, 2.0),  //  5  LV3u
	vec3(4.0,12.0, 2.0),  //  6  LV3l
	vec3(16.0, 4.0, 2.0),  //  7  LV4u
	vec3(4.0,16.0, 2.0),  //  8  LV4l
	vec3(12.0, 4.0, 6.0),  //  9  LV3u
	vec3(4.0,12.0, 6.0),  // 10  LV3l
	vec3(16.0, 4.0, 6.0),  // 11  LV4u
	vec3(4.0,16.0, 6.0)  // 12  LV4l
);

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
in Vertex {
   vec2 texCoord;
};

out vec4 fragColor;

vec4 remapTo01(vec4 v, vec4 _low, vec4 _high)
{
	return clamp((v - _low)/(_high-_low),0.0,1.0);
}

float remapFrom01(float v, float _high)
{
	return ROUND(_high*v);
}

float df(float A, float B)
{
	return abs(A-B);
}

//      A3 B3 C3
//      A1 B1 C1
//A2 A0  A  B  C C4 C6
//D2 D0  D  E  F F4 F6
//G2 G0  G  H  I I4 I6
//      G5 H5 I5
//      G7 H7 I7
	
void main(void){

	float px;

	vec2 pos = fract(texCoord*sourceSize[ORIG_ID].xy)-vec2(0.5, 0.5); // pos = pixel position
	vec2 dir = sign(pos); // dir = pixel direction

	vec2 g1 = dir* vec2(max( dir.y*dir.x,0.0), max(-dir.y*dir.x,0.0))*sourceSize[ORIG_ID].zw;
	vec2 g2 = dir* vec2(max(-dir.y*dir.x,0.0), max( dir.y*dir.x,0.0))*sourceSize[ORIG_ID].zw;

	vec3 E   = texture(source[ORIG_ID], texCoord    ).rgb;
	vec3 F   = texture(source[ORIG_ID], texCoord +g1).rgb;
	vec3 H   = texture(source[ORIG_ID], texCoord +g2).rgb;
	vec3 I   = texture(source[ORIG_ID], texCoord +g1+g2).rgb;
	vec3 F4  = texture(source[ORIG_ID], texCoord +2.0*g1).rgb;
	vec3 H5  = texture(source[ORIG_ID], texCoord +2.0*g2).rgb;

	float e = dot(E, y_weighted);
	float f = dot(F, y_weighted);
	float h = dot(H, y_weighted);
	float i = dot(I, y_weighted);
	float f4= dot(F4, y_weighted);
	float h5= dot(H5, y_weighted);

	vec4 icomp    = (clamp((dir* sym_vectors),0.0,1.0)); // choose info component

	float  infoE    = remapFrom01(dot(texture(source[0], texCoord   ), icomp), 256.0f); // retrieve 1st pass info
	float  infoF    = remapFrom01(dot(texture(source[0], texCoord+g1), icomp), 256.0f); // 1st pass info from neighbor r
	float  infoH    = remapFrom01(dot(texture(source[0], texCoord+g2), icomp), 256.0f); // 1st pass info from neighbor d

	vec4 lparam;
	vec2 addr;

	if (infoF == 8.0)
	{
		lparam.xyz = lines[12];
		px = float(df(f,f4) <= df(f,i));
		addr.x = 2*px + clamp(1.0-px,0.0,1.0);
		addr.y = clamp(1.0-px,0.0,1.0);
	}
	else if (infoH == 7.0)
	{
		lparam.xyz = lines[11];
		px = float(df(h,h5) <= df(h,i));
		addr.x = clamp(1.0-px,0.0,1.0);
		addr.y = 2*px + clamp(1.0-px,0.0,1.0);
	}
	else if (infoF == 6.0)
	{
		lparam.xyz = lines[10];
		px = float(df(f,f4) <= df(f,i));
		addr.x = 2*px + clamp(1.0-px,0.0,1.0);
		addr.y = clamp(1.0-px,0.0,1.0);
	}
	else if (infoH == 5.0)
	{
		lparam.xyz = lines[9];
		px = float(df(h,h5) <= df(h,i));
		addr.x = clamp(1.0-px,0.0,1.0);
		addr.y = 2*px + clamp(1.0-px,0.0,1.0);
	}
	else
	{
		px = float(df(e,f) <= df(e,h));
		addr.x = px;
		addr.y = clamp(1.0-px,0.0,1.0);

		lparam.xyz = (infoE == 8.0) ? lines[8] : ((infoE == 7.0) ? lines[7] : ((infoE == 6.0) ? lines[6] : ((infoE == 5.0) ? lines[5] : ((infoE == 4.0) ? lines[4] : ((infoE == 3.0) ? lines[3] : ((infoE == 2.0) ? lines[2] : ((infoE == 1.0) ? lines[1] : lines[0])))))));
	}

	bool inv = (dir.x*dir.y) < 0.0 ? true : false;

	// Rotate address from relative to absolute.
	addr = addr*dir.yx;
	addr = inv ? addr.yx : addr;

	// Rotate straight line equation from relative to absolute.
	lparam.xy = lparam.xy*dir.yx;
	lparam.xy = inv ? lparam.yx : lparam.xy;

	addr+=vec2(2.0);

	lparam.w = addr.x*8.0 + addr.y;


	fragColor=remapTo01(lparam, low, high);
}

/*
19 1
9  1
4  0
2  0
1  1
0  0

0 0000   ND
1 0001   EDR0
2 0010   EDR
3 0011   EDRU
4 0100   EDRL
5 0101   EDRU3
6 0110   EDRL3

0   1 2 3 4
-2 -1 0 1 2

*/
