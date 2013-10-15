
/*

   Hyllian's xBR MultiLevel4 Shader - Pass3
   
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
   vec4 t8;
   vec4 t9;
};
out vec4 fragColor;

vec4 _TMP45;

vec2 _TMP43;

vec2 _TMP42;

bool _TMP41;

vec3 _TMP33;

vec3 _TMP34;

vec3 _TMP35;

vec3 _TMP36;

vec3 _TMP37;

vec3 _TMP38;

vec3 _TMP39;

vec3 _TMP40;

float _TMP44;

float _TMP18;

vec4 _TMP17;

float _TMP16;

vec4 _TMP15;

float _TMP14;

vec4 _TMP13;

vec4 _TMP10;

vec4 _TMP9;

vec4 _TMP8;

vec4 _TMP7;

vec4 _TMP6;

vec4 _TMP5;

vec2 _TMP0;


vec2 _x0056;

vec2 _val0058;

float _x0060;

float _TMP61;

float _x0068;

float _TMP69;

float _x0076;

float _TMP77;

float _x0084;

float _TMP85;

vec2 _c0094;

vec2 _c0096;

vec2 _c0098;

vec2 _c0100;

vec2 _c0102;

vec4 _r0116;

vec4 _TMP119;

vec4 _TMP125;

vec4 _x0128;

float _TMP135;

float _a0136;

float _x0138;

vec2 _c0140;

float _TMP145;

float _a0146;

float _x0148;

vec2 _c0150;

float _TMP155;

float _a0156;

float _x0158;

float _TMP159;

float _a0162;

float _TMP163;

float _a0166;

float _x0168;

float _TMP169;

float _x0176;

float _TMP177;

float _TMP183;

float _a0186;

float _TMP187;

float _a0190;

float _x0192;

float _TMP193;

float _x0200;

float _TMP201;

float _TMP207;

float _a0210;

float _TMP211;

float _a0214;

float _x0216;

float _TMP217;

float _x0224;

float _TMP225;

float _TMP231;

float _a0234;

float _TMP235;

float _a0238;

float _x0240;

float _TMP241;

float _x0248;

float _TMP249;

float _TMP255;

float _a0258;

float _TMP259;

float _a0262;

float _x0264;

float _TMP265;

vec4 _x0274;

vec4 _TMP275;
void main()
{



    float _px;

    vec2 _pos;

    vec2 _dir;

    vec2 _g1;

    vec2 _g2;

    float _e;

    float _f;

    float _h;

    float _i;

    float _f4;

    float _h5;

    vec4 _lparam;

    vec2 _addr;



    _x0056 = texCoord.xy*sourceSize[2].xy;

    _TMP0 = fract(_x0056);

    _pos = _TMP0 - vec2( 5.00000000E-001, 5.00000000E-001);

    _val0058 = vec2(float((_pos.x > 0.00000000E+000)), float((_pos.y > 0.00000000E+000)));

    _dir = _val0058 - vec2(float((_pos.x < 0.00000000E+000)), float((_pos.y < 0.00000000E+000)));

    _x0060 = -_dir.y*_dir.x;

    _TMP44 = min(1.00000000E+000, _x0060);

    _TMP61 = max(0.00000000E+000, _TMP44);

    _x0068 = _dir.y*_dir.x;

    _TMP44 = min(1.00000000E+000, _x0068);

    _TMP69 = max(0.00000000E+000, _TMP44);

    _g1 = _dir*(_TMP61*t1.zw + _TMP69*t1.xy);

    _x0076 = _dir.y*_dir.x;

    _TMP44 = min(1.00000000E+000, _x0076);

    _TMP77 = max(0.00000000E+000, _TMP44);

    _x0084 = -_dir.y*_dir.x;

    _TMP44 = min(1.00000000E+000, _x0084);

    _TMP85 = max(0.00000000E+000, _TMP44);

    _g2 = _dir*(_TMP77*t1.zw + _TMP85*t1.xy);

    _TMP5 = texture(source[2], texCoord.xy);

    _c0094 = texCoord.xy + _g1;

    _TMP6 = texture(source[2], _c0094);

    _c0096 = texCoord.xy + _g2;

    _TMP7 = texture(source[2], _c0096);

    _c0098 = texCoord.xy + _g1 + _g2;

    _TMP8 = texture(source[2], _c0098);

    _c0100 = texCoord.xy + 2.00000000E+000*_g1;

    _TMP9 = texture(source[2], _c0100);

    _c0102 = texCoord.xy + 2.00000000E+000*_g2;

    _TMP10 = texture(source[2], _c0102);

    _e = dot(_TMP5.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _f = dot(_TMP6.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _h = dot(_TMP7.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _i = dot(_TMP8.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _f4 = dot(_TMP9.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _h5 = dot(_TMP10.xyz, vec3( 1.43519993E+001, 2.81760006E+001, 5.47200012E+000));

    _r0116 = _dir.x*vec4( 1.00000000E+000, 1.00000000E+000, -1.00000000E+000, -1.00000000E+000);

    _r0116 = _r0116 + _dir.y*vec4( 1.00000000E+000, -1.00000000E+000, -1.00000000E+000, 1.00000000E+000);

    _TMP45 = min(vec4( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000, 1.00000000E+000), _r0116);

    _TMP119 = max(vec4( 0.00000000E+000, 0.00000000E+000, 0.00000000E+000, 0.00000000E+000), _TMP45);

    _x0128 = _TMP119 + 5.00000000E-001;

    _TMP125 = floor(_x0128);

    _TMP13 = texture(source[0], texCoord.xy);

    _TMP14 = dot(_TMP13, _TMP125);

    _a0136 = 2.55000000E+002*_TMP14;

    _x0138 = _a0136 + 5.00000000E-001;

    _TMP135 = floor(_x0138);

    _c0140 = texCoord.xy + _g1;

    _TMP15 = texture(source[0], _c0140);

    _TMP16 = dot(_TMP15, _TMP125);

    _a0146 = 2.55000000E+002*_TMP16;

    _x0148 = _a0146 + 5.00000000E-001;

    _TMP145 = floor(_x0148);

    _c0150 = texCoord.xy + _g2;

    _TMP17 = texture(source[0], _c0150);

    _TMP18 = dot(_TMP17, _TMP125);

    _a0156 = 2.55000000E+002*_TMP18;

    _x0158 = _a0156 + 5.00000000E-001;

    _TMP155 = floor(_x0158);

    if (_TMP145 == 8.00000000E+000) { 
        _lparam.xyz = vec3( 4.00000000E+000, 1.60000000E+001, 6.00000000E+000);

        _a0162 = _f - _f4;

        _TMP159 = abs(_a0162);

        _a0166 = _f - _i;

        _TMP163 = abs(_a0166);

        _px = float((_TMP159 <= _TMP163));

        _x0168 = 1.00000000E+000 - _px;

        _TMP44 = min(1.00000000E+000, _x0168);

        _TMP169 = max(0.00000000E+000, _TMP44);

        _addr.x = 2.00000000E+000*_px + _TMP169;

        _x0176 = 1.00000000E+000 - _px;

        _TMP44 = min(1.00000000E+000, _x0176);

        _TMP177 = max(0.00000000E+000, _TMP44);

        _addr.y = _TMP177;

    } else {

        if (_TMP155 == 7.00000000E+000) { 
            _lparam.xyz = vec3( 1.60000000E+001, 4.00000000E+000, 6.00000000E+000);

            _a0186 = _h - _h5;

            _TMP183 = abs(_a0186);

            _a0190 = _h - _i;

            _TMP187 = abs(_a0190);

            _px = float((_TMP183 <= _TMP187));

            _x0192 = 1.00000000E+000 - _px;

            _TMP44 = min(1.00000000E+000, _x0192);

            _TMP193 = max(0.00000000E+000, _TMP44);

            _addr.x = _TMP193;

            _x0200 = 1.00000000E+000 - _px;

            _TMP44 = min(1.00000000E+000, _x0200);

            _TMP201 = max(0.00000000E+000, _TMP44);

            _addr.y = 2.00000000E+000*_px + _TMP201;

        } else {

            if (_TMP145 == 6.00000000E+000) { 
                _lparam.xyz = vec3( 4.00000000E+000, 1.20000000E+001, 6.00000000E+000);

                _a0210 = _f - _f4;

                _TMP207 = abs(_a0210);

                _a0214 = _f - _i;

                _TMP211 = abs(_a0214);

                _px = float((_TMP207 <= _TMP211));

                _x0216 = 1.00000000E+000 - _px;

                _TMP44 = min(1.00000000E+000, _x0216);

                _TMP217 = max(0.00000000E+000, _TMP44);

                _addr.x = 2.00000000E+000*_px + _TMP217;

                _x0224 = 1.00000000E+000 - _px;

                _TMP44 = min(1.00000000E+000, _x0224);

                _TMP225 = max(0.00000000E+000, _TMP44);

                _addr.y = _TMP225;

            } else {

                if (_TMP155 == 5.00000000E+000) { 
                    _lparam.xyz = vec3( 1.20000000E+001, 4.00000000E+000, 6.00000000E+000);

                    _a0234 = _h - _h5;

                    _TMP231 = abs(_a0234);

                    _a0238 = _h - _i;

                    _TMP235 = abs(_a0238);

                    _px = float((_TMP231 <= _TMP235));

                    _x0240 = 1.00000000E+000 - _px;

                    _TMP44 = min(1.00000000E+000, _x0240);

                    _TMP241 = max(0.00000000E+000, _TMP44);

                    _addr.x = _TMP241;

                    _x0248 = 1.00000000E+000 - _px;

                    _TMP44 = min(1.00000000E+000, _x0248);

                    _TMP249 = max(0.00000000E+000, _TMP44);

                    _addr.y = 2.00000000E+000*_px + _TMP249;

                } else {

                    _a0258 = _e - _f;

                    _TMP255 = abs(_a0258);

                    _a0262 = _e - _h;

                    _TMP259 = abs(_a0262);

                    _px = float((_TMP255 <= _TMP259));

                    _addr.x = _px;

                    _x0264 = 1.00000000E+000 - _px;

                    _TMP44 = min(1.00000000E+000, _x0264);

                    _TMP265 = max(0.00000000E+000, _TMP44);

                    _addr.y = _TMP265;

                    if (_TMP135 == 8.00000000E+000) { 
                        _TMP33 = vec3( 4.00000000E+000, 1.60000000E+001, 2.00000000E+000);

                    } else {

                        if (_TMP135 == 7.00000000E+000) { 
                            _TMP34 = vec3( 1.60000000E+001, 4.00000000E+000, 2.00000000E+000);

                        } else {

                            if (_TMP135 == 6.00000000E+000) { 
                                _TMP35 = vec3( 4.00000000E+000, 1.20000000E+001, 2.00000000E+000);

                            } else {

                                if (_TMP135 == 5.00000000E+000) { 
                                    _TMP36 = vec3( 1.20000000E+001, 4.00000000E+000, 2.00000000E+000);

                                } else {

                                    if (_TMP135 == 4.00000000E+000) { 
                                        _TMP37 = vec3( 4.00000000E+000, 8.00000000E+000, 2.00000000E+000);

                                    } else {

                                        if (_TMP135 == 3.00000000E+000) { 
                                            _TMP38 = vec3( 8.00000000E+000, 4.00000000E+000, 2.00000000E+000);

                                        } else {

                                            if (_TMP135 == 2.00000000E+000) { 
                                                _TMP39 = vec3( 4.00000000E+000, 4.00000000E+000, 2.00000000E+000);

                                            } else {

                                                if (_TMP135 == 1.00000000E+000) { 
                                                    _TMP40 = vec3( 4.00000000E+000, 4.00000000E+000, 3.00000000E+000);

                                                } else {

                                                    _TMP40 = vec3( 4.00000000E+000, 4.00000000E+000, 4.00000000E+000);

                                                } 
                                                _TMP39 = _TMP40;

                                            } 
                                            _TMP38 = _TMP39;

                                        } 
                                        _TMP37 = _TMP38;

                                    } 
                                    _TMP36 = _TMP37;

                                } 
                                _TMP35 = _TMP36;

                            } 
                            _TMP34 = _TMP35;

                        } 
                        _TMP33 = _TMP34;

                    } 
                    _lparam.xyz = _TMP33;

                } 
            } 
        } 
    } 
    if (_dir.x*_dir.y < 0.00000000E+000) { 
        _TMP41 = true;

    } else {

        _TMP41 = false;

    } 
    _addr = _addr*_dir.yx;

    if (_TMP41) { 
        _TMP42 = _addr.yx;

    } else {

        _TMP42 = _addr;

    } 
    _lparam.xy = _lparam.xy*_dir.yx;

    if (_TMP41) { 
        _TMP43 = _lparam.yx;

    } else {

        _TMP43 = _lparam.xy;

    } 
    _lparam.xy = _TMP43;

    _addr.x = _TMP42.x + 2.00000000E+000;

    _addr.y = _TMP42.y + 2.00000000E+000;

    _lparam.w = _addr.x*8.00000000E+000 + _addr.y;

    _x0274 = (_lparam - vec4( -6.40000000E+001, -6.40000000E+001, -6.40000000E+001, -6.40000000E+001))/vec4( 1.28000000E+002, 1.28000000E+002, 1.28000000E+002, 1.28000000E+002);

    _TMP45 = min(vec4( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000, 1.00000000E+000), _x0274);

    _TMP275 = max(vec4( 0.00000000E+000, 0.00000000E+000, 0.00000000E+000, 0.00000000E+000), _TMP45);

    fragColor = _TMP275;
}
