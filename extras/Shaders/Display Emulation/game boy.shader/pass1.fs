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

#in forceMonochromeGameBoy
#in persistence
#in baselineAlpha


uniform sampler2D source[];
uniform sampler2D history[];
uniform float baseline_alpha;

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

#ifdef  forceMonochromeGameBoy
#define GET_ALPHA(src,coord) (1.0-clamp((1.0 - texture(src, coord)[2])*(31.54-25.87+47.225),0.0,1.0))
#else
#define GET_ALPHA(src,coord) (1.0-clamp((1.0 - texture(src, coord)[2])*(0.2126+0.7152+0.0722),0.0,1.0))
#endif

void main(void) {	
	float  c0=GET_ALPHA(source[0],texCoord);
c0 = mix(GET_ALPHA(history[6], texCoord), c0, persistence);
c0 = mix(GET_ALPHA(history[5], texCoord), c0, persistence);
c0 = mix(GET_ALPHA(history[4], texCoord), c0, persistence);
c0 = mix(GET_ALPHA(history[3], texCoord), c0, persistence);
c0 = mix(GET_ALPHA(history[2], texCoord), c0, persistence);
c0 = mix(GET_ALPHA(history[1], texCoord), c0, persistence);
c0 = mix(GET_ALPHA(history[0], texCoord), c0, persistence);
c0 = mix(GET_ALPHA(source[0], texCoord), c0, persistence);

	fragColor=vec4(c0*(1.0-baselineAlpha)+baselineAlpha);
}
