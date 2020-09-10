#version 150

/*

   Hyllian's xBR MultiLevel4 Shader - Pass1
   
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

in vec4 position;
in vec2 texCoord;

out Vertex {
   vec2 vTexCoord;
   vec2 t1;
};

uniform vec4 outputSize;
uniform vec4 sourceSize[];

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


void main() {
   gl_Position = position;
   vTexCoord = texCoord * 1.0004;

	float2 ps = float2(1.0/texture_size.x, 1.0/texture_size.y);
	float dx = ps.x;
	float dy = ps.y;

	//      A3 B3 C3
	//      A1 B1 C1
	//A2 A0  A  B  C C4 C6
	//D2 D0  D  E  F F4 F6
	//G2 G0  G  H  I I4 I6
	//      G5 H5 I5
	//      G7 H7 I7
	
	t1 = float2(dx, dy); // F H
}