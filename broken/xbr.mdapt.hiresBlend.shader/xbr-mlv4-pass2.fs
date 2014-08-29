/*

   Hyllian's xBR MultiLevel4 Shader - Pass2
   
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
#define ROUND(X) 		floor((X)+0.5)
#define y_weighted		(vec3(0.299, 0.587, 0.114)* 48.0)
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

bvec4 rd(vec4 A, vec4 B, vec4 C, vec4 D)
{
    return greaterThan((df(C,D)/(df(A,B)+0.000000001)) , vec4(2.0));
}

bvec4 id(vec4 A, vec4 B, vec4 C, vec4 D)
{
    return greaterThan(df(C,D) , df(A,B));
}


vec4 remapTo01(vec4 v, vec4 high)
{
	return (v/high);
}

vec4 remapFrom01(vec4 v, vec4 high)
{
	return ROUND(high*v);
}


#define ORIG_ID 1
#define TEX(dx,dy) texture(source[0], texCoord+vec2((dx),(dy))*sourceSize[0].zw)
#define TEX_ORIG(dx,dy) texture(source[ORIG_ID], texCoord+vec2((dx),(dy))*sourceSize[ORIG_ID].zw).rgb

//    A1 B1 C1
// A0  A  B  C C4
// D0  D  E  F F4
// G0  G  H  I I4
//    G5 H5 I5
	
void main(void){


	vec4 PA = TEX(-1,-1);	vec4 PB = TEX( 0,-1);	vec4 PC = TEX( 1,-1);
	vec4 PD = TEX(-1, 0);	vec4 PE = TEX( 0, 0);	vec4 PF = TEX( 1, 0);
	vec4 PG = TEX(-1, 1);	vec4 PH = TEX( 0, 1);	vec4 PI = TEX( 1, 1);
	
								vec3 A1 = TEX_ORIG(-1,-2);	vec3 B1 = TEX_ORIG( 0,-2);	vec3 C1 = TEX_ORIG( 1,-2);
	vec3 A0 = TEX_ORIG(-2,-1);	vec3 A  = TEX_ORIG(-1,-1);	vec3 B  = TEX_ORIG( 0,-1);	vec3 C  = TEX_ORIG( 1,-1);	vec3 C4 = TEX_ORIG( 2,-1);
	vec3 D0 = TEX_ORIG(-2, 0);	vec3 D  = TEX_ORIG(-1, 0);	vec3 E  = TEX_ORIG( 0, 0);	vec3 F  = TEX_ORIG( 1, 0);	vec3 F4 = TEX_ORIG( 2, 0);
	vec3 G0 = TEX_ORIG(-2, 1);	vec3 G  = TEX_ORIG(-1, 1);	vec3 H  = TEX_ORIG( 0, 1);	vec3 I  = TEX_ORIG( 1, 1);	vec3 I4 = TEX_ORIG( 2, 1);
								vec3 G5 = TEX_ORIG(-1, 2);	vec3 H5 = TEX_ORIG( 0, 2);	vec3 I5 = TEX_ORIG( 1, 2);
	

	vec4 b = (transpose(mat4x3(B, D, H, F))* y_weighted );
	vec4 c = (transpose( mat4x3(C, A, G, I))* y_weighted );
	vec4 e = (transpose( mat4x3(E, E, E, E))* y_weighted );
	vec4 d = b.yzwx;
	vec4 f = b.wxyz;
	vec4 g = c.zwxy;
	vec4 h = b.zwxy;
	vec4 i = c.wxyz;

	vec4 i4 = (transpose(mat4x3(I4, C1, A0, G5))* y_weighted );
	vec4 i5 = (transpose(mat4x3(I5, C4, A1, G0))* y_weighted );
	vec4 h5 = (transpose(mat4x3(H5, F4, B1, D0))* y_weighted );
	vec4 f4 = h5.yzwx;


	vec4 pe = remapFrom01(PE, maximo);
	vec4 pf = remapFrom01(PF, maximo);
	vec4 ph = remapFrom01(PH, maximo);
	vec4 pb = remapFrom01(PB, maximo);
	vec4 pd = remapFrom01(PD, maximo);

	vec4 f2 = vec4(pf.z, pb.w, pd.x, ph.y);
	vec4 h2 = vec4(ph.z, pf.w, pb.x, pd.y);
	vec4 f1 = vec4(pf.y, pb.z, pd.w, ph.x);
	vec4 h3 = vec4(ph.w, pf.x, pb.y, pd.z);

	bvec4 nbrs = (greaterThan(pe.yzwx 	, vec4(1.0)) 	|| greaterThan(pe.wxyz 	, vec4(1.0)));
	bvec4 jag1 = (greaterThan(f2 		, vec4(1.0)) 	|| greaterThan(h2 		, vec4(1.0)));
	bvec4 jag2 = (greaterThan(f2 		, vec4(2.0)) 	|| greaterThan(h2 		, vec4(2.0)));
	bvec4 jag3 = (greaterThan(f2 		, vec4(4.0)) 	|| greaterThan(h2 		, vec4(4.0)));

	bvec4 cond1=(equal(pe,vec4(7.0)) || equal(pe,vec4(8.0)));
	
	pe.x=cond1.x?jag3.x?pe.x: (pe.x-2.0) : pe.x;
	pe.y=cond1.y?jag3.y?pe.y: (pe.y-2.0) : pe.y;
	pe.z=cond1.z?jag3.z?pe.z: (pe.z-2.0) : pe.z;
	pe.w=cond1.w?jag3.w?pe.w: (pe.w-2.0) : pe.w;
	
	bvec4 cond2=(equal(pe,vec4(5.0)) || equal(pe,vec4(6.0)));
	pe.x=cond2.x?jag2.x?pe.x: (pe.x-2.0) : pe.x;
	pe.y=cond2.y?jag2.y?pe.y: (pe.y-2.0) : pe.y;
	pe.z=cond2.z?jag2.z?pe.z: (pe.z-2.0) : pe.z;
	pe.w=cond2.w?jag2.w?pe.w: (pe.w-2.0) : pe.w;
	
	
	bvec4 jag91 = ((id(h,i,e,h) || id(i4,i,f4,i4)) && greaterThan(f2 ,vec4(1.0)) && greaterThan(f1 ,vec4(1.0)));
	bvec4 jag92 = ((id(f,i,e,f) || id(i5,i,h5,i5)) && greaterThan(h2 ,vec4(1.0)) && greaterThan(h3 ,vec4(1.0)));
	bvec4 jag93 = ( rd(h,g,e,g));
	bvec4 jag94 = ( rd(f,c,e,c));
	bvec4 jag9  = (not(jag91 && jag93 || jag92 && jag94));
	
	bvec4 cond3=(equal(pe ,vec4(0.0)) || (not(nbrs) || jag1) && jag9);
	
	pe.x=cond3.x?pe.x:1.0;
	pe.y=cond3.y?pe.y:1.0;
	pe.z=cond3.z?pe.z:1.0;
	pe.w=cond3.w?pe.w:1.0;
	

	fragColor=vec4(remapTo01(pe, maximo));

}