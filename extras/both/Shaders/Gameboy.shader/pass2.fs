// This is a port of the original CG shader to the quark format
// the original shader can be found here :
// https://github.com/libretro/common-shaders/tree/master/handheld/gameboy

///////////////////////////////////////////////////////////////////////////
//                                                                       //
// Gameboy Classic Shader v0.2.2                                         //
//                                                                       //
// Copyright (C) 2013 Harlequin : unknown92835@gmail.com                 //
//                                                                       //
// This program is free software: you can redistribute it and/or modify  //
// it under the terms of the GNU General Public License as published by  //
// the Free Software Foundation, either version 3 of the License, or     //
// (at your option) any later version.                                   //
//                                                                       //
// This program is distributed in the hope that it will be useful,       //
// but WITHOUT ANY WARRANTY; without even the implied warranty of        //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         //
// GNU General Public License for more details.                          //
//                                                                       //
// You should have received a copy of the GNU General Public License     //
// along with this program.  If not, see <http://www.gnu.org/licenses/>. //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#version 150

#in blend
#in spacing
#in shadowOffsetY

uniform sampler2D source[];
uniform sampler2D frame[];
uniform sampler2D pixmap[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
in Vertex {
  vec2 texCoord;
  vec4 newSize;
};

out vec4 fragColor;
#define pi			3.14159265358
#define GAUSS(X) (0.5)*exp(-(X)*(X)*pi*0.25)
void main(void) {
	float offset	= fract(texCoord.y * sourceSize[0].y);
	float  c0=texture(source[0],texCoord.xy-vec2(0.0,(offset-0.5)*sourceSize[0].w)).r;

	float a=1.0-spacing*newSize.y*sourceSize[0].w;
	a=max(a,0.0);	
	a=(offset > sourceSize[0].y*newSize.w)?1.0:a;
#ifdef blend
	c0=(offset > sourceSize[0].y*newSize.w)?c0:texture(source[0],texCoord.xy).r;
#endif
	
	vec2 shadowCoords=texCoord.xy;
	shadowCoords.y-=shadowOffsetY*sourceSize[0].w;
	offset=fract((shadowCoords.y * sourceSize[0].y)-0.5);
	float blur;
	blur =texture(source[0],shadowCoords.xy-vec2(0.0,(offset-0.0)*sourceSize[0].w)).g*GAUSS(offset-0.0);
	blur+=texture(source[0],shadowCoords.xy-vec2(0.0,(offset-1.0)*sourceSize[0].w)).g*GAUSS(offset-1.0);	
	blur+=texture(source[0],shadowCoords.xy-vec2(0.0,(offset-2.0)*sourceSize[0].w)).g*GAUSS(offset-2.0);
	blur+=texture(source[0],shadowCoords.xy-vec2(0.0,(offset+1.0)*sourceSize[0].w)).g*GAUSS(offset+1.0);	

	fragColor=vec4(c0,blur,a,a);

}
