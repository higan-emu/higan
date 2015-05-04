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
#in shadowOffsetX
#in contrast
#in shadowOpacity
#in foregroundColor
#in backgroundColor

uniform sampler2D source[];
uniform sampler2D frame[];
uniform sampler2D pixmap[];
uniform vec4 sourceSize[2];
uniform vec4 targetSize;

in Vertex {
  vec2 texCoord;
  vec4 newSize;
};


out vec4 fragColor;

#define pi			3.14159265358
#define GAUSS(X) (0.5)*exp(-(X)*(X)*pi*0.25)

void main(void) {
	float offset	= fract(texCoord.x * sourceSize[0].x);
	vec4  c0=texture(source[0],texCoord.xy-vec2((offset-0.5)*sourceSize[0].z,0.0));
	
	float a=1.0-spacing*newSize.x*sourceSize[0].z;
	a=max(a,0.0);	
	a=(offset > sourceSize[0].x*newSize.z)?c0.a:a;

#ifdef blend
	c0.r=(offset > sourceSize[0].x*newSize.z)?c0.r:texture(source[0],texCoord.xy).r;
#endif

	a*=c0.r;
	
	vec2 shadowCoords=texCoord.xy;
	shadowCoords.x-=shadowOffsetX*sourceSize[0].z;
	offset=fract((shadowCoords.x * sourceSize[0].x)-0.5);
	float blur;	
	blur =texture(source[0],shadowCoords.xy-vec2((offset-0.0)*sourceSize[0].z,0.0)).g*GAUSS(offset-0.0);
	blur+=texture(source[0],shadowCoords.xy-vec2((offset-2.0)*sourceSize[0].z,0.0)).g*GAUSS(offset-2.0);
	blur+=texture(source[0],shadowCoords.xy-vec2((offset-1.0)*sourceSize[0].z,0.0)).g*GAUSS(offset-1.0);
	blur+=texture(source[0],shadowCoords.xy-vec2((offset+1.0)*sourceSize[0].z,0.0)).g*GAUSS(offset+1.0);

	
    vec3 outColor =  backgroundColor * (1.0 - blur * contrast * shadowOpacity);
    outColor = outColor + a * contrast *( foregroundColor - outColor);
	fragColor=vec4(outColor,1.0);
	// fragColor=vec4(1.0-a);

}
