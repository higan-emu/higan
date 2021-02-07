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

#define contrast 0.95		//analogous to the contrast slider on the original Gameboy, higher values darken the image - [0, 1]
#define bg_smoothing 0.75	//higher values suppress changes in background color directly beneath the foreground to improve image clarity - [0, 1]
#define shadow_opacity 0.55	//how strongly shadows affect the background, higher values darken the shadows - [0, 1]
#define shadow_offset_x 1.0	//how far the shadow should be shifted to the right in pixels - [-infinity, infinity]
#define shadow_offset_y 1.0	//how far the shadow should be shifted to down in pixels - [-infinity, infinity]


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//fragment definitions/declarations                                                                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define bg_color texture(pixmap[0], vec2(0.25, 0.5))				//sample the background color from the palette
#define shadow_alpha (contrast * shadow_opacity)					//blending factor used when overlaying shadows on the background
#define shadow_offset vec2(shadow_offset_x * sourceSize[0].z, shadow_offset_y * sourceSize[0].w)	//offset for the shadow


uniform sampler2D source[];
uniform sampler2D frame[];
uniform sampler2D pixmap[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//fragment shader                                                                                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void main(void){
  //sample all the relevant textures	

    vec4 foreground = texture(source[2], texCoord);
    vec4 background = texture(pixmap[1], texCoord);
    vec4 shadows    = texture(source[0], texCoord - shadow_offset);
    vec4 background_color = bg_color;


  //foreground and background are blended with the background color

    foreground *= bg_color;
    background -= (background - 0.5) * bg_smoothing * ((foreground.a > 0.0)?1.0:0.0);	//suppress drastic background color changes under the foreground to improve clarity

    background.rgb = clamp(vec3( 				//allows for highlights, background = bg_color when the background color is 0.5 gray
	bg_color.r + mix(-1.0, 1.0, background.r), 
	bg_color.g + mix(-1.0, 1.0, background.g), 
	bg_color.b + mix(-1.0, 1.0, background.b) ),0.0,1.0);

  //shadows are alpha blended with the background

    vec4 out_color = (shadows * shadows.a * shadow_alpha) + (background * (1 - shadows.a * shadow_alpha));


  //foreground is alpha blended with the shadowed background

    out_color = (foreground * foreground.a * contrast) + (out_color * (1 - foreground.a * contrast));
	fragColor=out_color;

}