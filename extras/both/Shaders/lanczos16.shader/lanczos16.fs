// This is a port of the original CG shader to the quark format
// the original shader can be found here :
// https://github.com/libretro/common-shaders/tree/master/lanczos


/*
 
   Copyright (C) 2007 guest(r) - guest.r@gmail.com
 
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

#define		floatpi	1.5707963267948966192313216916398
#define		pi		3.1415926535897932384626433832795

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
in Vertex {
   vec2 texCoord;
   vec4 t1;
   vec4 t2;
   vec4 t3;
   vec4 t4;
};
#define SI(X,f1,f2) ((X)==0)?((f1)*(f2)):(sin((f1)*(X))*sin((f2)*(X))/((X)*(X)))
out vec4 fragColor;
vec4 l(vec4 x) {
	// vec4 res;
	// res =all(equal(x,vec4(0.0, 0.0, 0.0, 0.0)))?vec4(pi*floatpi):sin(x*floatpi)* sin(x*pi)/(x*x);
	return vec4(SI(x.x,pi,floatpi),SI(x.y,pi,floatpi),SI(x.z,pi,floatpi),SI(x.w,pi,floatpi));
}
void main(void) {
	mat4x3 pix;
	vec2 frac = fract(texCoord*sourceSize[0].xy);
    vec2 offset = fract(texCoord * sourceSize[0].xy) - 0.5;
	offset /= sourceSize[0].xy;
    // calculating texel weights
     
      vec4 abcd, pqrs;
     
      abcd = l(vec4(1+frac.x, frac.x, 1-frac.x, 2-frac.x));
      pqrs = l(vec4(1+frac.y, frac.y, 1-frac.y, 2-frac.y));
     
    // reading the texels
     
      vec3 c00 = texture(source[0], t1.xz-offset).xyz;
      vec3 c10 = texture(source[0], t1.yz-offset).xyz;
      vec3 c20 = texture(source[0], t2.xz-offset).xyz;
      vec3 c30 = texture(source[0], t2.yz-offset).xyz;
      vec3 c01 = texture(source[0], t1.xw-offset).xyz;
      vec3 c11 = texture(source[0], texCoord-offset).xyz;
      vec3 c21 = texture(source[0], t2.xw-offset).xyz;
      vec3 c31 = texture(source[0], t2.yw-offset).xyz;
      vec3 c02 = texture(source[0], t3.xz-offset).xyz;
      vec3 c12 = texture(source[0], t3.yz-offset).xyz;
      vec3 c22 = texture(source[0], t4.xz-offset).xyz;
      vec3 c32 = texture(source[0], t4.yz-offset).xyz;
      vec3 c03 = texture(source[0], t3.xw-offset).xyz;
      vec3 c13 = texture(source[0], t3.yw-offset).xyz;
      vec3 c23 = texture(source[0], t4.xw-offset).xyz;
      vec3 c33 = texture(source[0], t4.yw-offset).xyz;
     
      pix[0] = abcd* transpose(mat4x3(-c00, c10, c20, -c30));
      pix[1] = abcd* transpose(mat4x3( c01, c11, c21,  c31));
      pix[2] = abcd* transpose(mat4x3( c02, c12, c22,  c32));
      pix[3] = abcd* transpose(mat4x3(-c03, c13, c23, -c33));
     
    // final sum and weight normalization
    fragColor=vec4((pqrs*transpose(pix))/vec3((dot(abcd, vec4(1.0))* dot(pqrs, vec4(1.0)))-2.0*(abcd.x+abcd.w)*(pqrs.x+pqrs.w)),1.0);
     

	
}






























































