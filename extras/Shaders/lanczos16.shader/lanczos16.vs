// This is a port of the original CG shader to the quark format
// the original shader can be found here :
// https://github.com/libretro/common-shaders/tree/master/handheld/gameboy

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

in vec4 position;
in vec2 texCoord;
 
out Vertex {
   vec2 texCoord;
   vec4 t1;
   vec4 t2;
   vec4 t3;
   vec4 t4;
} vertexOut;
 
uniform vec4 targetSize;
uniform vec4 sourceSize[];

void main() {
	gl_Position=position;
	vertexOut.texCoord=texCoord;
	vertexOut.t1=texCoord.xxyy + vec4(-sourceSize[0].z,                 0.0,    -sourceSize[0].w,                 0.0);
    vertexOut.t2=texCoord.xxyy + vec4( sourceSize[0].z, 2.0*sourceSize[0].z,    -sourceSize[0].w,                 0.0);
    vertexOut.t3=texCoord.xxyy + vec4(-sourceSize[0].z,                 0.0,     sourceSize[0].w, 2.0*sourceSize[0].w);
    vertexOut.t4=texCoord.xxyy + vec4( sourceSize[0].z, 2.0*sourceSize[0].z,     sourceSize[0].w, 2.0*sourceSize[0].w);
}
