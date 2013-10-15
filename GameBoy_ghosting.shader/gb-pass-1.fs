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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//config                                                                                                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define blending_mode 0				//0 - only the space between dots is blending, 1 - all texels are blended
#define adjacent_texel_alpha_blending 0.1755	//the amount of alpha swapped between neighboring texels

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//fragment definitions/declarations                                                                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define blending_modifier(color) clamp(float(color.a == 0) + blending_mode,0.0,1.0)

uniform sampler2D source[];
uniform sampler2D frame[];
uniform sampler2D pixmap[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
in Vertex {
   vec2 texCoord;
   vec2 tex_coord_1;
   vec2 tex_coord_2;
   vec2 tex_coord_3;
   vec2 tex_coord_4;
   vec2 lower_bound;
   vec2 upper_bound;
};

out vec4 fragColor;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//fragment functions                                                                                                                      //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//a simple blur technique that softens harsh color transitions
//specialized to only blur alpha values and limited to only blurring texels lying in the spaces between two or more texels

float simple_blur(vec4 COLOR)
{
  //clamp the blur coords to the input texture size so it doesn't attempt to sample off the texture (it'll retrieve float4(0.0) and darken the edges otherwise)

    vec2 coord_1 = clamp(tex_coord_1, lower_bound, upper_bound);
    vec2 coord_2 = clamp(tex_coord_2, lower_bound, upper_bound);
    vec2 coord_3 = clamp(tex_coord_3, lower_bound, upper_bound);
    vec2 coord_4 = clamp(tex_coord_4, lower_bound, upper_bound);


  //sample adjacent texels based on the coordinates above

    vec4 adjacent_texel_1 = texture(source[0], coord_1);
    vec4 adjacent_texel_2 = texture(source[0], coord_2);
    vec4 adjacent_texel_3 = texture(source[0], coord_3);
    vec4 adjacent_texel_4 = texture(source[0], coord_4);


  //sum the alpha differences between neighboring texels, apply modifiers, then subtract the result from the current fragment alpha value

    COLOR.a -= ( (COLOR.a - adjacent_texel_1.a) + 
		 (COLOR.a - adjacent_texel_2.a) + 
		 (COLOR.a - adjacent_texel_3.a) + 
		 (COLOR.a - adjacent_texel_4.a) ) * adjacent_texel_alpha_blending * blending_modifier(COLOR);


  //return new alpha value
    return COLOR.a;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//fragment shader                                                                                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main(void) {

  //sample the input textures

    vec4 out_color = texture(source[0], texCoord);


  //apply the blur effect

    out_color.a = simple_blur(out_color);


  //return

    fragColor=out_color;
	
}