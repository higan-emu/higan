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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//vertex shader                                                                                                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#version 150 

in vec4 position;
in vec2 texCoord;
 
out Vertex {
   vec2 texCoord;
   vec2 tex_coord_1;
   vec2 tex_coord_2;
   vec2 tex_coord_3;
   vec2 tex_coord_4;
   vec2 lower_bound;
   vec2 upper_bound;
} vertexOut;
 
uniform vec4 targetSize;
uniform vec4 sourceSize[];

void main() {
    gl_Position = position;
    vertexOut.texCoord = texCoord;
  //define the neighboring texel coordinates for use in blur
  
	vertexOut.tex_coord_1=texCoord + vec2(0.0,sourceSize[0].w );	//down
	vertexOut.tex_coord_2=texCoord + vec2(0.0, -sourceSize[0].w);	//up
	vertexOut.tex_coord_3=texCoord + vec2(sourceSize[0].z, 0.0);  //right
	vertexOut.tex_coord_4=texCoord + vec2(-sourceSize[0].z, 0.0);	//left
	vertexOut.lower_bound=vec2(0.0);				//lower texture bounds
	vertexOut.upper_bound=sourceSize[0].zw * (targetSize.xy - 2.0);		//upper texture bounds
}
