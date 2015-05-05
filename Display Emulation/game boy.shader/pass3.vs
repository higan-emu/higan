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
in vec4 position;
in vec2 texCoord;
 
out Vertex {
   vec2 texCoord;
   vec4 newSize;
} vertexOut;
 
uniform vec4 targetSize;
uniform vec4 sourceSize[];

void main() {
	gl_Position=position;
	
	float video_scale=floor(targetSize.y * sourceSize[1].w); 		
	vertexOut.newSize.xy=sourceSize[1].xy*video_scale;
    vertexOut.newSize.zw=sourceSize[1].zw/video_scale;
	vec2 coord=texCoord*targetSize.xy;
	coord-=floor((targetSize.xy-vertexOut.newSize.xy)/2.0);
	vertexOut.texCoord.x=coord.x*vertexOut.newSize.z;	
	vertexOut.texCoord.y=texCoord.y;
}
