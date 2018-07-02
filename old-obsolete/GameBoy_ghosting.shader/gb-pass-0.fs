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

#define baseline_alpha 0.10	//the alpha value of dots in their "off" state, does not affect the border region of the screen - [0, 1]
#define response_time 0.333	//simulate response time, higher values result in longer color transition periods - [0, 1]
#define HIGAN_FORCE_MONOCHROME_GB //uncomment only when running gb games in higan since the shader expects black and white input.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//fragment definitions                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define foreground_color texture(pixmap[0], vec2(0.75, 0.5)).rgb					//hardcoded to look up the foreground color from the right half of the palette image
// #define rgb_to_alpha(rgb) ( ((rgb.r + rgb.g + rgb.b) / 3.0) + (is_on_dot * baseline_alpha) )		//averages rgb values (allows it to work with color games), modified for contrast and base alpha
#define rgb_to_alpha(rgb) ( ((0.2126*rgb.r + 0.7152*rgb.g + 0.0722*rgb.b) ) + (is_on_dot * baseline_alpha) )	//modified formula


#ifdef HIGAN_FORCE_MONOCHROME_GB
#define TEX(src,coord) vec4(vec3(clamp(dot(texture(src, coord).rgb,vec3(21,-16.73,30)),0.0,1.0)),1.0)
#else
#define TEX(src,coord) texture(src, coord)
#endif

#define curr_rgb  abs(1 - TEX(source[0], texCoord).rgb)
#define prev0_rgb abs(1 - TEX(history[0], texCoord).rgb)
#define prev1_rgb abs(1 - TEX(history[1], texCoord).rgb)
#define prev2_rgb abs(1 - TEX(history[2], texCoord).rgb)
#define prev3_rgb abs(1 - TEX(history[3], texCoord).rgb)
#define prev4_rgb abs(1 - TEX(history[4], texCoord).rgb)
#define prev5_rgb abs(1 - TEX(history[5], texCoord).rgb)
#define prev6_rgb abs(1 - TEX(history[6], texCoord).rgb)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//fragment shader                                                                                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uniform sampler2D source[];
uniform sampler2D history[];
uniform sampler2D pixmap[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
in Vertex {
  vec2 texCoord;
  vec2 one_texel;
};

out vec4 fragColor;

void main(void) {
  //determine if the corrent texel lies on a dot or in the space between dots

    float is_on_dot = float( mod(texCoord.x, sourceSize[0].z) > one_texel.x)* 		//returns 1 if fragment lies on a dot, 0 otherwise
			       float(mod(texCoord.y, sourceSize[0].w) > one_texel.y )*
				   float(texCoord.x>=0.0)*
				   float(texCoord.x<=1.0)*
				   float(texCoord.y>=0.0)*
				   float(texCoord.y<=1.0);


  //sample color from the current and previous historys, apply response time modifier
  //response time effect implmented through an exponential dropoff algorithm

    vec3 input_rgb = curr_rgb;
    input_rgb += (prev0_rgb - input_rgb) * response_time;
    input_rgb += (prev1_rgb - input_rgb) * pow(response_time, 2.0);
    input_rgb += (prev2_rgb - input_rgb) * pow(response_time, 3.0);
    input_rgb += (prev3_rgb - input_rgb) * pow(response_time, 4.0);
    input_rgb += (prev4_rgb - input_rgb) * pow(response_time, 5.0);
    input_rgb += (prev5_rgb - input_rgb) * pow(response_time, 6.0);
    input_rgb += (prev6_rgb - input_rgb) * pow(response_time, 7.0);


  //apply foreground color and assign alpha value

  vec4 out_color = vec4(foreground_color, rgb_to_alpha(input_rgb));	//apply the foreground color to all texels (the color will be modified by alpha later) and assign alpha based on rgb input
    

  //overlay the matrix

    out_color.a *= is_on_dot;		//if the fragment is not on a dot, set its alpha value to 0


  //return fragment color

    fragColor= out_color;
}
