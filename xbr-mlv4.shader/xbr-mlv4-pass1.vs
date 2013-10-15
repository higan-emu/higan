/*

   Hyllian's xBR MultiLevel4 Shader - Pass1
   
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
   vec4 t2;
   vec4 t3;
   vec4 t4;
   vec4 t5;
   vec4 t6;
   vec4 t7;
   vec4 t8;
   vec4 t9;
} vertexOut;
 
uniform vec4 targetSize;
uniform vec4 sourceSize[];

void main()
{
    vec2 _ps;

    _ps = vec2(1.00000000E+000/sourceSize[0].x, 1.00000000E+000/sourceSize[0].y);
    vertexOut.t1 = texCoord.xxyy + vec4(-2.00000000E+000*_ps.x, 2.00000000E+000*_ps.x, -_ps.y, _ps.y);
    vertexOut.t2 = texCoord.xxyy + vec4(-_ps.x, _ps.x, -2.00000000E+000*_ps.y, 2.00000000E+000*_ps.y);
    vertexOut.t3 = texCoord.xxyy + vec4(-3.00000000E+000*_ps.x, 3.00000000E+000*_ps.x, -_ps.y, _ps.y);
    vertexOut.t4 = texCoord.xxyy + vec4(-_ps.x, _ps.x, -3.00000000E+000*_ps.y, 3.00000000E+000*_ps.y);
    vertexOut.t5 = texCoord.xxyy + vec4(-_ps.x, _ps.x, -_ps.y, _ps.y);
    vertexOut.t6 = texCoord.xyyy + vec4(0.00000000E+000, -_ps.y, -2.00000000E+000*_ps.y, -3.00000000E+000*_ps.y);
    vertexOut.t7 = texCoord.xxxy + vec4(-3.00000000E+000*_ps.x, -2.00000000E+000*_ps.x, -_ps.x, 0.00000000E+000);
    vertexOut.t8 = texCoord.xyyy + vec4(0.00000000E+000, _ps.y, 2.00000000E+000*_ps.y, 3.00000000E+000*_ps.y);
    vertexOut.t9 = texCoord.xxxy + vec4(3.00000000E+000*_ps.x, 2.00000000E+000*_ps.x, _ps.x, 0.00000000E+000);
    gl_Position = position;

    vertexOut.texCoord = texCoord;
  
} 