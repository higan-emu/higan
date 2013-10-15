
/*

   Hyllian's xBR MultiLevel4 Shader - Pass4
   
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

in vec4 position;
in vec2 texCoord;
 
out Vertex {
   vec2 texCoord;
   vec4 t1;
   float scale_factor;
} vertexOut;
 
uniform vec4 targetSize;
uniform vec4 sourceSize[];

void main()
{
    vec2 _ps;

    _ps = vec2(1.00000000E+000/sourceSize[3].x, 1.00000000E+000/sourceSize[3].y);
    vertexOut.t1 = vec4(_ps.x, 0.00000000E+000, 0.00000000E+000, _ps.y);

    vertexOut.scale_factor = targetSize.x/sourceSize[3].x;
    
    gl_Position = position;    vertexOut.texCoord = texCoord;
  
} 