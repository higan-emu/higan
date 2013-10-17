/*

   Hyllian's xBR MultiLevel4 Shader - Pass1
   
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
#define cf2				2.0
#define cf3				4.0
#define cf4 			4.0
#define eq_threshold	vec4( 2.0,  2.0,  2.0,  2.0)
#define Yuv_weight		vec3(4.0, 1.0, 2.0)
#define yuvT			(mat3x3(0.299, 0.587, 0.114, -0.169, -0.331, 0.499, 0.499, -0.418, -0.0813))
#define maximo			vec4(256.0)

uniform sampler2D source[];
uniform vec4 sourceSize[];
in Vertex {
   vec2 texCoord;
};

out vec4 fragColor;

vec4 df(vec4 A, vec4 B)
{
	return vec4(abs(A-B));
}

bvec4 id(vec4 A, vec4 B, vec4 C, vec4 D)
{
    return greaterThan(df(C,D) , df(A,B));
}

vec4 remapTo01(vec4 v, vec4 high)
{
	return (v/high);
}

bvec4 eq(vec4 A, vec4 B)
{
	return lessThan(df(A, B) , eq_threshold);
}

vec4 weighted_distance(vec4 a, vec4 b, vec4 c, vec4 d, vec4 e, vec4 f, vec4 g, vec4 h)
{
	return (df(a,b) + df(a,c) + df(d,e) + df(d,f) + 4.0*df(g,h));
}



#define TEX(dx,dy) texture(source[0], texCoord+vec2((dx),(dy))*sourceSize[0].zw).rgb

//      A3 B3 C3
//      A1 B1 C1
//A2 A0  A  B  C C4 C6
//D2 D0  D  E  F F4 F6
//G2 G0  G  H  I I4 I6
//      G5 H5 I5
//      G7 H7 I7
	
void main(void){
	bvec4 edr0, edr, edr_left, edr_up, edr3_left, edr3_up, edr4_left, edr4_up; // edr = edge detection rule
	bvec4 interp_restriction_lv1, interp_restriction_lv2_left, interp_restriction_lv2_up;
	bvec4 interp_restriction_lv3_left, interp_restriction_lv3_up;
	bvec4 interp_restriction_lv4_left, interp_restriction_lv4_up;

	
											vec3 A3=TEX(-1,-3);	vec3 B3=TEX( 0,-3);	vec3 C3=TEX( 1,-3);	
											vec3 A1=TEX(-1,-2);	vec3 B1=TEX( 0,-2);	vec3 C1=TEX( 1,-2);
	vec3 A2=TEX(-3,-1);	vec3 A0=TEX(-2,-1);	vec3 A =TEX(-1,-1);	vec3 B =TEX( 0,-1);	vec3 C =TEX( 1,-1);	vec3 C4=TEX( 2,-1);	vec3 C6=TEX( 3,-1);
	vec3 D2=TEX(-3, 0);	vec3 D0=TEX(-2, 0);	vec3 D =TEX(-1, 0);	vec3 E =TEX( 0, 0);	vec3 F =TEX( 1, 0);	vec3 F4=TEX( 2, 0);	vec3 F6=TEX( 3, 0);
	vec3 G2=TEX(-3, 1);	vec3 G0=TEX(-2, 1);	vec3 G =TEX(-1, 1);	vec3 H =TEX( 0, 1);	vec3 I =TEX( 1, 1);	vec3 I4=TEX( 2, 1);	vec3 I6=TEX( 3, 1);
											vec3 G5=TEX(-1, 2);	vec3 H5=TEX( 0, 2);	vec3 I5=TEX( 1, 2);
											vec3 G7=TEX(-1, 3);	vec3 H7=TEX( 0, 3);	vec3 I7=TEX( 1, 3);

	mat4x3 bdhf = transpose(transpose(mat4x3(B, D, H, F))* yuvT );
	bdhf = mat4x3(abs(bdhf[0]), abs(bdhf[1]), abs(bdhf[2]), abs(bdhf[3]));
	vec4 b = transpose(bdhf)*Yuv_weight;

	bdhf = transpose(transpose(mat4x3(C, A, G, I))* yuvT );
	bdhf = mat4x3(abs(bdhf[0]), abs(bdhf[1]), abs(bdhf[2]), abs(bdhf[3]));
	vec4 c = transpose(bdhf)* Yuv_weight;

	bdhf = transpose(transpose(mat4x3(E, E, E, E))* yuvT );
	bdhf = mat4x3(abs(bdhf[0]), abs(bdhf[1]), abs(bdhf[2]), abs(bdhf[3]));
	vec4 e = transpose(bdhf)*Yuv_weight;

	vec4 d = b.yzwx;
	vec4 f = b.wxyz;
	vec4 g = c.zwxy;
	vec4 h = b.zwxy;
	vec4 i = c.wxyz;

	bdhf = transpose(transpose(mat4x3(I4, C1, A0, G5))* yuvT );
	bdhf = mat4x3(abs(bdhf[0]), abs(bdhf[1]), abs(bdhf[2]), abs(bdhf[3]));
	vec4 i4 = transpose(bdhf)*  Yuv_weight;

	bdhf = transpose(transpose(mat4x3(I5, C4, A1, G0))*yuvT );
	bdhf = mat4x3(abs(bdhf[0]), abs(bdhf[1]), abs(bdhf[2]), abs(bdhf[3]));
	vec4 i5 = transpose(bdhf)*  Yuv_weight;

	bdhf =transpose( transpose(mat4x3(H5, F4, B1, D0))* yuvT );
	bdhf = mat4x3(abs(bdhf[0]), abs(bdhf[1]), abs(bdhf[2]), abs(bdhf[3]));
	vec4 h5 = transpose(bdhf)*Yuv_weight;

	vec4 f4 = h5.yzwx;

	vec4 c1 = i4.yzwx;
	vec4 g0 = i5.wxyz;
	vec4 b1 = h5.zwxy;
	vec4 d0 = h5.wxyz;

	bdhf =transpose( transpose(mat4x3(I6, C3, A2, G7))* yuvT );
	bdhf = mat4x3(abs(bdhf[0]), abs(bdhf[1]), abs(bdhf[2]), abs(bdhf[3]));
	vec4 i6 = transpose(bdhf)*  Yuv_weight;

	bdhf =transpose( transpose(mat4x3(I7, C6, A3, G2))* yuvT );
	bdhf = mat4x3(abs(bdhf[0]), abs(bdhf[1]), abs(bdhf[2]), abs(bdhf[3]));
	vec4 i7 = transpose(bdhf)*  Yuv_weight;

	bdhf =transpose( transpose(mat4x3(H7, F6, B3, D2))* yuvT);
	bdhf = mat4x3(abs(bdhf[0]), abs(bdhf[1]), abs(bdhf[2]), abs(bdhf[3]));
	vec4 h7 = transpose(bdhf)*  Yuv_weight;

	vec4 f6 = h7.yzwx;

	vec4 c3 = i6.yzwx;
	vec4 g2 = i7.wxyz;
	vec4 b3 = h7.zwxy;
	vec4 d2 = h7.wxyz;

	interp_restriction_lv1      = (notEqual(e,f)  &&  notEqual(e,h));
	interp_restriction_lv2_left = (notEqual(e,g)  &&  notEqual(d,g)  && (eq(e, d) || eq(h, g)));
	interp_restriction_lv2_up   = (notEqual(e,c)  &&  notEqual(b,c)  && (eq(e, b) || eq(f, c)));
	interp_restriction_lv3_left = (notEqual(e,g0) && notEqual(d0,g0) && (eq(d,d0) || eq(g,g0)));
	interp_restriction_lv3_up   = (notEqual(e,c1) && notEqual(b1,c1) && (eq(b,b1) || eq(c,c1)));
	interp_restriction_lv4_left = (notEqual(e,g2) && notEqual(d2,g2) && (eq(d0,d2) || eq(g0,g2)));
	interp_restriction_lv4_up   = (notEqual(e,c3) && notEqual(b3,c3) && (eq(b1,b3) || eq(c1,c3)));
	

	vec4 wd1 = weighted_distance( e, c, g, i, h5, f4, h, f);
	vec4 wd2 = weighted_distance( h, d, i5, f, i4, b, e, i);

	edr0      = lessThanEqual(wd1,wd2) && interp_restriction_lv1;
	edr       = lessThan(wd1 ,wd2) && interp_restriction_lv1 && ( not(eq(f,b)) && not(id(f,c,f,b)) || not(eq(h,d)) && not(id(h,g,h,d)) || eq(e,g) || eq(e,c) );
	edr_left  = lessThanEqual((cf2*df(f,g))  , df(h,c))   && interp_restriction_lv2_left && edr;
	edr_up    = greaterThanEqual(df(f,g)  , (cf2*df(h,c)))   && interp_restriction_lv2_up   && edr;
	edr3_left = lessThanEqual((cf3*df(f,g0)) , df(h,c1)) && interp_restriction_lv3_left && edr_left;
	edr3_up   = greaterThanEqual(df(f,g0) , (cf3*df(h,c1))) && interp_restriction_lv3_up   && edr_up;
	edr4_left = lessThanEqual((cf4*df(f,g2)) , df(h,c3)) && interp_restriction_lv4_left && edr3_left;
	edr4_up   = greaterThanEqual(df(f,g2) , (cf4*df(h,c3))) && interp_restriction_lv4_up   && edr3_up;


	vec4 info;
	bvec4 cond1=(edr4_left && not(edr4_up));
	bvec4 cond2=(edr4_up && not(edr4_left));
	bvec4 cond3=(edr3_left && not(edr3_up));
	bvec4 cond4=(edr3_up && not(edr3_left));
	bvec4 cond5=(edr_left && not(edr_up));
	bvec4 cond6=(edr_up && not(edr_left));
	bvec4 cond7=edr;
	bvec4 cond8=edr0;
	
	info.x=cond1.x?8.0:cond2.x?7.0:cond3.x?6.0:cond4.x?5.0:cond5.x?4.0:cond6.x?3.0:cond7.x?2.0:cond8.x?1.0:0.0;
	info.y=cond1.y?8.0:cond2.y?7.0:cond3.y?6.0:cond4.y?5.0:cond5.y?4.0:cond6.y?3.0:cond7.y?2.0:cond8.y?1.0:0.0;
	info.z=cond1.z?8.0:cond2.z?7.0:cond3.z?6.0:cond4.z?5.0:cond5.z?4.0:cond6.z?3.0:cond7.z?2.0:cond8.z?1.0:0.0;
	info.w=cond1.w?8.0:cond2.w?7.0:cond3.w?6.0:cond4.w?5.0:cond5.w?4.0:cond6.w?3.0:cond7.w?2.0:cond8.w?1.0:0.0;
	
	fragColor=vec4(remapTo01(info, maximo));

}

