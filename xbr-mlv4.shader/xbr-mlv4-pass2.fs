

/*

   Hyllian's xBR MultiLevel4 Shader - Pass2
   
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
uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
in Vertex {
   vec2 texCoord;
   vec4 t1;
   vec4 t2;
   vec4 t3;
   vec4 t4;
   vec4 t5;
   vec4 t6;
   vec4 t7;
};
out vec4 fragColor;

vec4 _ret_0;

bvec4 _TMP24;

bvec4 _TMP23;

bvec4 _TMP22;

bvec4 _TMP21;

vec4 _TMP20;

vec4 _TMP19;

vec4 _TMP18;

vec4 _TMP17;

vec4 _TMP16;

vec4 _TMP15;

vec4 _TMP14;

vec4 _TMP13;

vec4 _TMP12;

vec4 _TMP11;

vec4 _TMP10;

vec4 _TMP9;

vec4 _TMP8;

vec4 _TMP7;

vec4 _TMP6;

vec4 _TMP5;

vec4 _TMP4;

vec4 _TMP3;

vec4 _TMP2;

vec4 _TMP1;

vec4 _TMP0;


vec4 _r0102;

vec4 _r0112;

vec4 _r0122;

vec4 _r0132;

vec4 _r0142;

vec4 _r0152;

vec4 _TMP163;

vec4 _a0164;

vec4 _x0166;

vec4 _TMP169;

vec4 _a0170;

vec4 _x0172;

vec4 _TMP175;

vec4 _a0176;

vec4 _x0178;

vec4 _TMP181;

vec4 _a0182;

vec4 _x0184;

vec4 _TMP187;

vec4 _a0188;

vec4 _x0190;

vec4 _TMP193;

vec4 _a0196;

vec4 _TMP197;

vec4 _a0200;

vec4 _TMP203;

vec4 _a0206;

vec4 _TMP207;

vec4 _a0210;

vec4 _TMP213;

vec4 _a0216;

vec4 _TMP217;

vec4 _a0220;

vec4 _TMP223;

vec4 _a0226;

vec4 _TMP227;

vec4 _a0230;

vec4 _TMP233;

vec4 _a0236;

vec4 _TMP237;

vec4 _a0240;

vec4 _TMP243;

vec4 _a0246;

vec4 _TMP247;

vec4 _a0250;



void main()

{


    vec4 _PB;

    vec4 _PD;

    vec4 _PE;

    vec4 _PF;

    vec4 _PH;

    vec4 _pe;

    vec4 _f2;

    vec4 _h2;

    vec4 _f1;

    vec4 _h3;

    bvec4 _nbrs;

    bvec4 _jag1;

    bvec4 _jag2;

    bvec4 _jag3;

    bvec4 _jag91;

    bvec4 _jag92;

    bvec4 _jag93;

    bvec4 _jag94;

    bvec4 _jag9;



    _PB = texture(source[0], t2.yw);

    _PD = texture(source[0], t3.xw);

    _PE = texture(source[0], t3.yw);

    _PF = texture(source[0], t3.zw);

    _PH = texture(source[0], t4.yw);

    _TMP0 = texture(source[1], t1.xw);

    _TMP1 = texture(source[1], t1.yw);

    _TMP2 = texture(source[1], t1.zw);

    _TMP3 = texture(source[1], t2.xw);

    _TMP4 = texture(source[1], t2.yw);

    _TMP5 = texture(source[1], t2.zw);

    _TMP6 = texture(source[1], t3.xw);

    _TMP7 = texture(source[1], t3.yw);

    _TMP8 = texture(source[1], t3.zw);

    _TMP9 = texture(source[1], t4.xw);

    _TMP10 = texture(source[1], t4.yw);

    _TMP11 = texture(source[1], t4.zw);

    _TMP12 = texture(source[1], t5.xw);

    _TMP13 = texture(source[1], t5.yw);

    _TMP14 = texture(source[1], t5.zw);

    _TMP15 = texture(source[1], t6.xy);

    _TMP16 = texture(source[1], t6.xz);

    _TMP17 = texture(source[1], t6.xw);

    _TMP18 = texture(source[1], t7.xy);

    _TMP19 = texture(source[1], t7.xz);

    _TMP20 = texture(source[1], t7.xw);

    _r0102.x = dot(_TMP4.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0102.y = dot(_TMP6.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0102.z = dot(_TMP10.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0102.w = dot(_TMP8.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0112.x = dot(_TMP5.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0112.y = dot(_TMP3.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0112.z = dot(_TMP9.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0112.w = dot(_TMP11.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0122.x = dot(_TMP7.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0122.y = dot(_TMP7.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0122.z = dot(_TMP7.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0122.w = dot(_TMP7.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0132.x = dot(_TMP20.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0132.y = dot(_TMP2.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0132.z = dot(_TMP15.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0132.w = dot(_TMP12.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0142.x = dot(_TMP14.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0142.y = dot(_TMP18.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0142.z = dot(_TMP0.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0142.w = dot(_TMP17.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0152.x = dot(_TMP13.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0152.y = dot(_TMP19.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0152.z = dot(_TMP1.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0152.w = dot(_TMP16.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _a0164 = vec4( 2.55000000E+002, 2.55000000E+002, 2.55000000E+002, 2.55000000E+002)*_PE;

    _x0166 = _a0164 + 5.00000000E-001;

    _TMP163 = floor(_x0166);

    _a0170 = vec4( 2.55000000E+002, 2.55000000E+002, 2.55000000E+002, 2.55000000E+002)*_PF;

    _x0172 = _a0170 + 5.00000000E-001;

    _TMP169 = floor(_x0172);

    _a0176 = vec4( 2.55000000E+002, 2.55000000E+002, 2.55000000E+002, 2.55000000E+002)*_PH;

    _x0178 = _a0176 + 5.00000000E-001;

    _TMP175 = floor(_x0178);

    _a0182 = vec4( 2.55000000E+002, 2.55000000E+002, 2.55000000E+002, 2.55000000E+002)*_PB;

    _x0184 = _a0182 + 5.00000000E-001;

    _TMP181 = floor(_x0184);

    _a0188 = vec4( 2.55000000E+002, 2.55000000E+002, 2.55000000E+002, 2.55000000E+002)*_PD;

    _x0190 = _a0188 + 5.00000000E-001;

    _TMP187 = floor(_x0190);

    _f2 = vec4(_TMP169.z, _TMP181.w, _TMP187.x, _TMP175.y);

    _h2 = vec4(_TMP175.z, _TMP169.w, _TMP181.x, _TMP187.y);

    _f1 = vec4(_TMP169.y, _TMP181.z, _TMP187.w, _TMP175.x);

    _h3 = vec4(_TMP175.w, _TMP169.x, _TMP181.y, _TMP187.z);

    _nbrs = bvec4(_TMP163.y > 1.00000000E+000 || _TMP163.w > 1.00000000E+000 ? true : false, _TMP163.z > 1.00000000E+000 || _TMP163.x > 1.00000000E+000 ? true : false, _TMP163.w > 1.00000000E+000 || _TMP163.y > 1.00000000E+000 ? true : false, _TMP163.x > 1.00000000E+000 || _TMP163.z > 1.00000000E+000 ? true : false);

    _jag1 = bvec4(_f2.x > 1.00000000E+000 || _h2.x > 1.00000000E+000 ? true : false, _f2.y > 1.00000000E+000 || _h2.y > 1.00000000E+000 ? true : false, _f2.z > 1.00000000E+000 || _h2.z > 1.00000000E+000 ? true : false, _f2.w > 1.00000000E+000 || _h2.w > 1.00000000E+000 ? true : false);

    _jag2 = bvec4(_f2.x > 2.00000000E+000 || _h2.x > 2.00000000E+000 ? true : false, _f2.y > 2.00000000E+000 || _h2.y > 2.00000000E+000 ? true : false, _f2.z > 2.00000000E+000 || _h2.z > 2.00000000E+000 ? true : false, _f2.w > 2.00000000E+000 || _h2.w > 2.00000000E+000 ? true : false);

    _jag3 = bvec4(_f2.x > 4.00000000E+000 || _h2.x > 4.00000000E+000 ? true : false, _f2.y > 4.00000000E+000 || _h2.y > 4.00000000E+000 ? true : false, _f2.z > 4.00000000E+000 || _h2.z > 4.00000000E+000 ? true : false, _f2.w > 4.00000000E+000 || _h2.w > 4.00000000E+000 ? true : false);

    _pe = vec4(_TMP163.x == 7.00000000E+000 || _TMP163.x == 8.00000000E+000 ? _jag3.x ? _TMP163.x : (_TMP163 - 2.00000000E+000).x : _TMP163.x, _TMP163.y == 7.00000000E+000 || _TMP163.y == 8.00000000E+000 ? _jag3.y ? _TMP163.y : (_TMP163 - 2.00000000E+000).y : _TMP163.y, _TMP163.z == 7.00000000E+000 || _TMP163.z == 8.00000000E+000 ? _jag3.z ? _TMP163.z : (_TMP163 - 2.00000000E+000).z : _TMP163.z, _TMP163.w == 7.00000000E+000 || _TMP163.w == 8.00000000E+000 ? _jag3.w ? _TMP163.w : (_TMP163 - 2.00000000E+000).w : _TMP163.w);

    _pe = vec4(_pe.x == 5.00000000E+000 || _pe.x == 6.00000000E+000 ? _jag2.x ? _pe.x : (_pe - 2.00000000E+000).x : _pe.x, _pe.y == 5.00000000E+000 || _pe.y == 6.00000000E+000 ? _jag2.y ? _pe.y : (_pe - 2.00000000E+000).y : _pe.y, _pe.z == 5.00000000E+000 || _pe.z == 6.00000000E+000 ? _jag2.z ? _pe.z : (_pe - 2.00000000E+000).z : _pe.z, _pe.w == 5.00000000E+000 || _pe.w == 6.00000000E+000 ? _jag2.w ? _pe.w : (_pe - 2.00000000E+000).w : _pe.w);

    _a0196 = _r0122 - _r0102.zwxy;

    _TMP193 = abs(_a0196);

    _a0200 = _r0102.zwxy - _r0112.wxyz;

    _TMP197 = abs(_a0200);

    _TMP21 = bvec4(_TMP193.x > _TMP197.x, _TMP193.y > _TMP197.y, _TMP193.z > _TMP197.z, _TMP193.w > _TMP197.w);

    _a0206 = _r0152.yzwx - _r0132;

    _TMP203 = abs(_a0206);

    _a0210 = _r0132 - _r0112.wxyz;

    _TMP207 = abs(_a0210);

    _TMP22 = bvec4(_TMP203.x > _TMP207.x, _TMP203.y > _TMP207.y, _TMP203.z > _TMP207.z, _TMP203.w > _TMP207.w);

    _jag91 = bvec4((_TMP21.x || _TMP22.x) && _f2.x > 1.00000000E+000 && _f1.x > 1.00000000E+000, (_TMP21.y || _TMP22.y) && _f2.y > 1.00000000E+000 && _f1.y > 1.00000000E+000, (_TMP21.z || _TMP22.z) && _f2.z > 1.00000000E+000 && _f1.z > 1.00000000E+000, (_TMP21.w || _TMP22.w) && _f2.w > 1.00000000E+000 && _f1.w > 1.00000000E+000);

    _a0216 = _r0122 - _r0102.wxyz;

    _TMP213 = abs(_a0216);

    _a0220 = _r0102.wxyz - _r0112.wxyz;

    _TMP217 = abs(_a0220);

    _TMP23 = bvec4(_TMP213.x > _TMP217.x, _TMP213.y > _TMP217.y, _TMP213.z > _TMP217.z, _TMP213.w > _TMP217.w);

    _a0226 = _r0152 - _r0142;

    _TMP223 = abs(_a0226);

    _a0230 = _r0142 - _r0112.wxyz;

    _TMP227 = abs(_a0230);

    _TMP24 = bvec4(_TMP223.x > _TMP227.x, _TMP223.y > _TMP227.y, _TMP223.z > _TMP227.z, _TMP223.w > _TMP227.w);

    _jag92 = bvec4((_TMP23.x || _TMP24.x) && _h2.x > 1.00000000E+000 && _h3.x > 1.00000000E+000, (_TMP23.y || _TMP24.y) && _h2.y > 1.00000000E+000 && _h3.y > 1.00000000E+000, (_TMP23.z || _TMP24.z) && _h2.z > 1.00000000E+000 && _h3.z > 1.00000000E+000, (_TMP23.w || _TMP24.w) && _h2.w > 1.00000000E+000 && _h3.w > 1.00000000E+000);

    _a0236 = _r0122 - _r0112.zwxy;

    _TMP233 = abs(_a0236);

    _a0240 = _r0102.zwxy - _r0112.zwxy;

    _TMP237 = abs(_a0240);

    _jag93 = bvec4((_TMP233/(_TMP237 + 9.99999972E-010)).x > 2.00000000E+000, (_TMP233/(_TMP237 + 9.99999972E-010)).y > 2.00000000E+000, (_TMP233/(_TMP237 + 9.99999972E-010)).z > 2.00000000E+000, (_TMP233/(_TMP237 + 9.99999972E-010)).w > 2.00000000E+000);

    _a0246 = _r0122 - _r0112;

    _TMP243 = abs(_a0246);

    _a0250 = _r0102.wxyz - _r0112;

    _TMP247 = abs(_a0250);

    _jag94 = bvec4((_TMP243/(_TMP247 + 9.99999972E-010)).x > 2.00000000E+000, (_TMP243/(_TMP247 + 9.99999972E-010)).y > 2.00000000E+000, (_TMP243/(_TMP247 + 9.99999972E-010)).z > 2.00000000E+000, (_TMP243/(_TMP247 + 9.99999972E-010)).w > 2.00000000E+000);

    _jag9 = bvec4(!(_jag91.x && _jag93.x || _jag92.x && _jag94.x), !(_jag91.y && _jag93.y || _jag92.y && _jag94.y), !(_jag91.z && _jag93.z || _jag92.z && _jag94.z), !(_jag91.w && _jag93.w || _jag92.w && _jag94.w));

    _pe = vec4(_pe.x == 0.00000000E+000 || (!_nbrs.x || _jag1.x) && _jag9.x ? _pe.x : 1.00000000E+000, _pe.y == 0.00000000E+000 || (!_nbrs.y || _jag1.y) && _jag9.y ? _pe.y : 1.00000000E+000, _pe.z == 0.00000000E+000 || (!_nbrs.z || _jag1.z) && _jag9.z ? _pe.z : 1.00000000E+000, _pe.w == 0.00000000E+000 || (!_nbrs.w || _jag1.w) && _jag9.w ? _pe.w : 1.00000000E+000);

    _ret_0 = _pe/vec4( 2.55000000E+002, 2.55000000E+002, 2.55000000E+002, 2.55000000E+002);
	
    fragColor = _ret_0;
}
