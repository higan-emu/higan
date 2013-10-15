
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
uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
in Vertex {
   vec2 texCoord;
   vec4 t1;
   float scale_factor;
};
out vec4 fragColor;
vec4 _ret_0;

vec3 _TMP49;

float _TMP51;

float _TMP50;

vec3 _TMP46;

float _TMP48;

float _TMP47;

vec3 _TMP43;

float _TMP45;

float _TMP44;

float _TMP42;

float _TMP55;

float _TMP41;

float _TMP40;

float _TMP39;

float _TMP38;

float _TMP37;

float _TMP36;

float _TMP35;

vec4 _TMP34;

float _TMP33;

float _TMP32;

float _TMP31;

float _TMP53;

float _TMP54;

float _TMP30;

float _TMP29;

float _TMP28;

float _TMP27;

vec4 _TMP26;

float _TMP25;

float _TMP24;

float _TMP23;

float _TMP22;

float _TMP21;

float _TMP20;

float _TMP19;

vec4 _TMP18;

float _TMP17;

float _TMP16;

float _TMP15;

float _TMP14;

float _TMP13;

float _TMP12;

float _TMP11;

vec4 _TMP10;

float _TMP9;

float _TMP8;

float _TMP7;

float _TMP6;

float _TMP5;

float _TMP4;

float _TMP3;

vec4 _TMP2;

vec4 _TMP52;

vec2 _TMP1;

vec2 _TMP0;

vec2 _x0064;

vec2 _x0066;

vec2 _c0068;

vec2 _c0070;

vec2 _c0072;

vec2 _c0074;

vec4 _TMP79;

vec4 _x0082;

vec4 _TMP87;

vec4 _x0090;

vec4 _TMP95;

vec4 _x0098;

vec4 _TMP103;

vec4 _x0106;

float _x0110;

float _x0114;

float _TMP117;

float _x0120;

float _x0122;

float _x0126;

float _TMP129;

float _x0132;

float _x0134;

float _x0138;

float _TMP141;

float _x0144;

float _x0146;

float _x0150;

float _TMP153;

float _x0156;

float _x0158;

float _x0162;

float _TMP165;

float _x0168;

float _TMP169;

float _x0172;

vec2 _c0178;

float _x0180;

float _x0184;

float _TMP187;

float _x0190;

float _x0192;

float _x0196;

float _TMP199;

float _x0202;

float _x0204;

float _x0208;

float _TMP211;

float _x0214;

float _x0216;

float _x0220;

float _TMP223;

float _x0226;

float _x0228;

float _x0232;

float _TMP235;

float _x0238;

float _TMP239;

float _x0242;

vec2 _c0248;

float _x0250;

float _x0254;

float _TMP257;

float _x0260;

float _x0262;

float _x0266;

float _TMP269;

float _x0272;

float _x0274;

float _x0278;

float _TMP281;

float _x0284;

float _x0286;

float _x0290;

float _TMP293;

float _x0296;

float _x0298;

float _x0302;

float _TMP305;

float _x0308;

float _TMP309;

float _x0312;

vec2 _c0318;

float _x0320;

float _x0324;

float _TMP327;

float _x0330;

float _x0332;

float _x0336;

float _TMP339;

float _x0342;

float _x0344;

float _x0348;

float _TMP351;

float _x0354;

float _x0356;

float _x0360;

float _TMP363;

float _x0366;

float _x0368;

float _x0372;

float _TMP375;

float _x0378;

float _TMP379;

float _x0382;

vec2 _c0388;

float _a0390;

float _a0392;

float _a0394;

float _a0396;

vec4 _b0398;

float _x0402;

float _TMP403;

float _x0412;

float _TMP413;

float _x0422;

float _TMP423;

float _x0432;

float _TMP433;

vec3 _df0448;

vec3 _a0450;

vec3 _df0452;

vec3 _a0454;

vec3 _df0456;

vec3 _a0458;

vec3 _df0460;

vec3 _a0462;

vec3 _df0464;

vec3 _a0466;

vec3 _df0468;

vec3 _a0470;


void main()
{


    vec2 _fp;

    vec2 _pxcoord;

    vec4 _UL;

    vec4 _UR;

    vec4 _DL;

    vec4 _DR;

    vec3 _ax;

    vec3 _ay;

    vec2 _iq;

    vec3 _fp1;

    vec4 _inc;

    vec4 _level;

    vec3 _c11;

    vec3 _c21;

    vec3 _c3;

    vec3 _c4;



    _x0064 = texCoord.xy*sourceSize[3].xy;

    _TMP0 = fract(_x0064);

    _fp = _TMP0 - vec2( 5.00000000E-001, 5.00000000E-001);

    _x0066 = texCoord.xy*sourceSize[3].xy;

    _TMP1 = floor(_x0066);

    _pxcoord = _TMP1/sourceSize[3].xy;

    _c0068 = _pxcoord + 2.50000000E-001*t1.xy + 2.50000000E-001*t1.zw;

    _UL = texture(source[0], _c0068);

    _c0070 = _pxcoord + 7.50000000E-001*t1.xy + 2.50000000E-001*t1.zw;

    _UR = texture(source[0], _c0070);

    _c0072 = _pxcoord + 2.50000000E-001*t1.xy + 7.50000000E-001*t1.zw;

    _DL = texture(source[0], _c0072);

    _c0074 = _pxcoord + 7.50000000E-001*t1.xy + 7.50000000E-001*t1.zw;

    _DR = texture(source[0], _c0074);

    _TMP52 = vec4( -6.40000000E+001, -6.40000000E+001, -6.40000000E+001, -6.40000000E+001) + _UL*vec4( 1.28000000E+002, 1.28000000E+002, 1.28000000E+002, 1.28000000E+002);

    _x0082 = _TMP52 + 5.00000000E-001;

    _TMP79 = floor(_x0082);

    _TMP52 = vec4( -6.40000000E+001, -6.40000000E+001, -6.40000000E+001, -6.40000000E+001) + _UR*vec4( 1.28000000E+002, 1.28000000E+002, 1.28000000E+002, 1.28000000E+002);

    _x0090 = _TMP52 + 5.00000000E-001;

    _TMP87 = floor(_x0090);

    _TMP52 = vec4( -6.40000000E+001, -6.40000000E+001, -6.40000000E+001, -6.40000000E+001) + _DL*vec4( 1.28000000E+002, 1.28000000E+002, 1.28000000E+002, 1.28000000E+002);

    _x0098 = _TMP52 + 5.00000000E-001;

    _TMP95 = floor(_x0098);

    _TMP52 = vec4( -6.40000000E+001, -6.40000000E+001, -6.40000000E+001, -6.40000000E+001) + _DR*vec4( 1.28000000E+002, 1.28000000E+002, 1.28000000E+002, 1.28000000E+002);

    _x0106 = _TMP52 + 5.00000000E-001;

    _TMP103 = floor(_x0106);

    _TMP2 = texture(source[3], texCoord.xy);

    _x0110 = _TMP79.w/2.00000000E+000;

    if (_x0110 < 0.00000000E+000) { 
        _x0114 = -_x0110;

        _TMP54 = floor(_x0114);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0110);

    } 
    _TMP3 = _x0110 - _TMP53;

    _x0120 = _TMP3 + 5.00000000E-001;

    _TMP117 = floor(_x0120);

    _ay.z = _TMP117;

    _x0122 = _TMP53/2.00000000E+000;

    if (_x0122 < 0.00000000E+000) { 
        _x0126 = -_x0122;

        _TMP54 = floor(_x0126);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0122);

    } 
    _TMP4 = _x0122 - _TMP53;

    _x0132 = _TMP4 + 5.00000000E-001;

    _TMP129 = floor(_x0132);

    _ay.y = _TMP129;

    _x0134 = _TMP53/2.00000000E+000;

    if (_x0134 < 0.00000000E+000) { 
        _x0138 = -_x0134;

        _TMP54 = floor(_x0138);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0134);

    } 
    _TMP5 = _x0134 - _TMP53;

    _x0144 = _TMP5 + 5.00000000E-001;

    _TMP141 = floor(_x0144);

    _ay.x = _TMP141;

    _x0146 = _TMP53/2.00000000E+000;

    if (_x0146 < 0.00000000E+000) { 
        _x0150 = -_x0146;

        _TMP54 = floor(_x0150);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0146);

    } 
    _TMP6 = _x0146 - _TMP53;

    _x0156 = _TMP6 + 5.00000000E-001;

    _TMP153 = floor(_x0156);

    _ax.z = _TMP153;

    _x0158 = _TMP53/2.00000000E+000;

    if (_x0158 < 0.00000000E+000) { 
        _x0162 = -_x0158;

        _TMP54 = floor(_x0162);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0158);

    } 
    _TMP7 = _x0158 - _TMP53;

    _x0168 = _TMP7 + 5.00000000E-001;

    _TMP165 = floor(_x0168);

    _ax.y = _TMP165;

    _x0172 = _TMP53 + 5.00000000E-001;

    _TMP169 = floor(_x0172);

    _ax.x = _TMP169;

    _TMP8 = dot(_ax, vec3( 4.00000000E+000, 2.00000000E+000, 1.00000000E+000));

    _iq.x = _TMP8 - 2.00000000E+000;

    _TMP9 = dot(_ay, vec3( 4.00000000E+000, 2.00000000E+000, 1.00000000E+000));

    _iq.y = _TMP9 - 2.00000000E+000;

    _c0178 = texCoord.xy + _iq.x*t1.xy + _iq.y*t1.zw;

    _TMP10 = texture(source[3], _c0178);

    _x0180 = _TMP87.w/2.00000000E+000;

    if (_x0180 < 0.00000000E+000) { 
        _x0184 = -_x0180;

        _TMP54 = floor(_x0184);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0180);

    } 
    _TMP11 = _x0180 - _TMP53;

    _x0190 = _TMP11 + 5.00000000E-001;

    _TMP187 = floor(_x0190);

    _ay.z = _TMP187;

    _x0192 = _TMP53/2.00000000E+000;

    if (_x0192 < 0.00000000E+000) { 
        _x0196 = -_x0192;

        _TMP54 = floor(_x0196);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0192);

    } 
    _TMP12 = _x0192 - _TMP53;

    _x0202 = _TMP12 + 5.00000000E-001;

    _TMP199 = floor(_x0202);

    _ay.y = _TMP199;

    _x0204 = _TMP53/2.00000000E+000;

    if (_x0204 < 0.00000000E+000) { 
        _x0208 = -_x0204;

        _TMP54 = floor(_x0208);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0204);

    } 
    _TMP13 = _x0204 - _TMP53;

    _x0214 = _TMP13 + 5.00000000E-001;

    _TMP211 = floor(_x0214);

    _ay.x = _TMP211;

    _x0216 = _TMP53/2.00000000E+000;

    if (_x0216 < 0.00000000E+000) { 
        _x0220 = -_x0216;

        _TMP54 = floor(_x0220);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0216);

    } 
    _TMP14 = _x0216 - _TMP53;

    _x0226 = _TMP14 + 5.00000000E-001;

    _TMP223 = floor(_x0226);

    _ax.z = _TMP223;

    _x0228 = _TMP53/2.00000000E+000;

    if (_x0228 < 0.00000000E+000) { 
        _x0232 = -_x0228;

        _TMP54 = floor(_x0232);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0228);

    } 
    _TMP15 = _x0228 - _TMP53;

    _x0238 = _TMP15 + 5.00000000E-001;

    _TMP235 = floor(_x0238);

    _ax.y = _TMP235;

    _x0242 = _TMP53 + 5.00000000E-001;

    _TMP239 = floor(_x0242);

    _ax.x = _TMP239;

    _TMP16 = dot(_ax, vec3( 4.00000000E+000, 2.00000000E+000, 1.00000000E+000));

    _iq.x = _TMP16 - 2.00000000E+000;

    _TMP17 = dot(_ay, vec3( 4.00000000E+000, 2.00000000E+000, 1.00000000E+000));

    _iq.y = _TMP17 - 2.00000000E+000;

    _c0248 = texCoord.xy + _iq.x*t1.xy + _iq.y*t1.zw;

    _TMP18 = texture(source[3], _c0248);

    _x0250 = _TMP95.w/2.00000000E+000;

    if (_x0250 < 0.00000000E+000) { 
        _x0254 = -_x0250;

        _TMP54 = floor(_x0254);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0250);

    } 
    _TMP19 = _x0250 - _TMP53;

    _x0260 = _TMP19 + 5.00000000E-001;

    _TMP257 = floor(_x0260);

    _ay.z = _TMP257;

    _x0262 = _TMP53/2.00000000E+000;

    if (_x0262 < 0.00000000E+000) { 
        _x0266 = -_x0262;

        _TMP54 = floor(_x0266);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0262);

    } 
    _TMP20 = _x0262 - _TMP53;

    _x0272 = _TMP20 + 5.00000000E-001;

    _TMP269 = floor(_x0272);

    _ay.y = _TMP269;

    _x0274 = _TMP53/2.00000000E+000;

    if (_x0274 < 0.00000000E+000) { 
        _x0278 = -_x0274;

        _TMP54 = floor(_x0278);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0274);

    } 
    _TMP21 = _x0274 - _TMP53;

    _x0284 = _TMP21 + 5.00000000E-001;

    _TMP281 = floor(_x0284);

    _ay.x = _TMP281;

    _x0286 = _TMP53/2.00000000E+000;

    if (_x0286 < 0.00000000E+000) { 
        _x0290 = -_x0286;

        _TMP54 = floor(_x0290);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0286);

    } 
    _TMP22 = _x0286 - _TMP53;

    _x0296 = _TMP22 + 5.00000000E-001;

    _TMP293 = floor(_x0296);

    _ax.z = _TMP293;

    _x0298 = _TMP53/2.00000000E+000;

    if (_x0298 < 0.00000000E+000) { 
        _x0302 = -_x0298;

        _TMP54 = floor(_x0302);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0298);

    } 
    _TMP23 = _x0298 - _TMP53;

    _x0308 = _TMP23 + 5.00000000E-001;

    _TMP305 = floor(_x0308);

    _ax.y = _TMP305;

    _x0312 = _TMP53 + 5.00000000E-001;

    _TMP309 = floor(_x0312);

    _ax.x = _TMP309;

    _TMP24 = dot(_ax, vec3( 4.00000000E+000, 2.00000000E+000, 1.00000000E+000));

    _iq.x = _TMP24 - 2.00000000E+000;

    _TMP25 = dot(_ay, vec3( 4.00000000E+000, 2.00000000E+000, 1.00000000E+000));

    _iq.y = _TMP25 - 2.00000000E+000;

    _c0318 = texCoord.xy + _iq.x*t1.xy + _iq.y*t1.zw;

    _TMP26 = texture(source[3], _c0318);

    _x0320 = _TMP103.w/2.00000000E+000;

    if (_x0320 < 0.00000000E+000) { 
        _x0324 = -_x0320;

        _TMP54 = floor(_x0324);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0320);

    } 
    _TMP27 = _x0320 - _TMP53;

    _x0330 = _TMP27 + 5.00000000E-001;

    _TMP327 = floor(_x0330);

    _ay.z = _TMP327;

    _x0332 = _TMP53/2.00000000E+000;

    if (_x0332 < 0.00000000E+000) { 
        _x0336 = -_x0332;

        _TMP54 = floor(_x0336);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0332);

    } 
    _TMP28 = _x0332 - _TMP53;

    _x0342 = _TMP28 + 5.00000000E-001;

    _TMP339 = floor(_x0342);

    _ay.y = _TMP339;

    _x0344 = _TMP53/2.00000000E+000;

    if (_x0344 < 0.00000000E+000) { 
        _x0348 = -_x0344;

        _TMP54 = floor(_x0348);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0344);

    } 
    _TMP29 = _x0344 - _TMP53;

    _x0354 = _TMP29 + 5.00000000E-001;

    _TMP351 = floor(_x0354);

    _ay.x = _TMP351;

    _x0356 = _TMP53/2.00000000E+000;

    if (_x0356 < 0.00000000E+000) { 
        _x0360 = -_x0356;

        _TMP54 = floor(_x0360);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0356);

    } 
    _TMP30 = _x0356 - _TMP53;

    _x0366 = _TMP30 + 5.00000000E-001;

    _TMP363 = floor(_x0366);

    _ax.z = _TMP363;

    _x0368 = _TMP53/2.00000000E+000;

    if (_x0368 < 0.00000000E+000) { 
        _x0372 = -_x0368;

        _TMP54 = floor(_x0372);

        _TMP53 = -_TMP54;

    } else {

        _TMP53 = floor(_x0368);

    } 
    _TMP31 = _x0368 - _TMP53;

    _x0378 = _TMP31 + 5.00000000E-001;

    _TMP375 = floor(_x0378);

    _ax.y = _TMP375;

    _x0382 = _TMP53 + 5.00000000E-001;

    _TMP379 = floor(_x0382);

    _ax.x = _TMP379;

    _TMP32 = dot(_ax, vec3( 4.00000000E+000, 2.00000000E+000, 1.00000000E+000));

    _iq.x = _TMP32 - 2.00000000E+000;

    _TMP33 = dot(_ay, vec3( 4.00000000E+000, 2.00000000E+000, 1.00000000E+000));

    _iq.y = _TMP33 - 2.00000000E+000;

    _c0388 = texCoord.xy + _iq.x*t1.xy + _iq.y*t1.zw;

    _TMP34 = texture(source[3], _c0388);

    _fp1 = vec3(_fp.x, _fp.y, -1.00000000E+000);

    _a0390 = _TMP79.x/_TMP79.y;

    _TMP35 = abs(_a0390);

    _a0392 = _TMP87.x/_TMP87.y;

    _TMP36 = abs(_a0392);

    _a0394 = _TMP95.x/_TMP95.y;

    _TMP37 = abs(_a0394);

    _a0396 = _TMP103.x/_TMP103.y;

    _TMP38 = abs(_a0396);

    _inc = vec4(_TMP35, _TMP36, _TMP37, _TMP38);

    _b0398 = 1.00000000E+000/_inc;

    _level = max(_inc, _b0398);

    _TMP39 = dot(_fp1, _TMP79.xyz);

    _x0402 = (_TMP39*scale_factor)/(8.00000000E+000*_level.x) + 5.00000000E-001;

    _TMP55 = min(1.00000000E+000, _x0402);

    _TMP403 = max(0.00000000E+000, _TMP55);

    _TMP40 = dot(_fp1, _TMP87.xyz);

    _x0412 = (_TMP40*scale_factor)/(8.00000000E+000*_level.y) + 5.00000000E-001;

    _TMP55 = min(1.00000000E+000, _x0412);

    _TMP413 = max(0.00000000E+000, _TMP55);

    _TMP41 = dot(_fp1, _TMP95.xyz);

    _x0422 = (_TMP41*scale_factor)/(8.00000000E+000*_level.z) + 5.00000000E-001;

    _TMP55 = min(1.00000000E+000, _x0422);

    _TMP423 = max(0.00000000E+000, _TMP55);

    _TMP42 = dot(_fp1, _TMP103.xyz);

    _x0432 = (_TMP42*scale_factor)/(8.00000000E+000*_level.w) + 5.00000000E-001;

    _TMP55 = min(1.00000000E+000, _x0432);

    _TMP433 = max(0.00000000E+000, _TMP55);

    _c11 = _TMP2.xyz + _TMP403*(_TMP10.xyz - _TMP2.xyz);

    _c21 = _TMP2.xyz + _TMP413*(_TMP18.xyz - _TMP2.xyz);

    _c3 = _TMP2.xyz + _TMP423*(_TMP26.xyz - _TMP2.xyz);

    _c4 = _TMP2.xyz + _TMP433*(_TMP34.xyz - _TMP2.xyz);

    _a0450 = _c21 - _TMP2.xyz;

    _df0448 = abs(_a0450);

    _TMP44 = _df0448.x + _df0448.y + _df0448.z;

    _a0454 = _c11 - _TMP2.xyz;

    _df0452 = abs(_a0454);

    _TMP45 = _df0452.x + _df0452.y + _df0452.z;

    if (_TMP44 > _TMP45) { 
        _TMP43 = _c21;

    } else {

        _TMP43 = _c11;

    } 
    _a0458 = _c3 - _TMP2.xyz;

    _df0456 = abs(_a0458);

    _TMP47 = _df0456.x + _df0456.y + _df0456.z;

    _a0462 = _TMP43 - _TMP2.xyz;

    _df0460 = abs(_a0462);

    _TMP48 = _df0460.x + _df0460.y + _df0460.z;

    if (_TMP47 > _TMP48) { 
        _TMP46 = _c3;

    } else {

        _TMP46 = _TMP43;

    } 
    _a0466 = _c4 - _TMP2.xyz;

    _df0464 = abs(_a0466);

    _TMP50 = _df0464.x + _df0464.y + _df0464.z;

    _a0470 = _TMP46 - _TMP2.xyz;

    _df0468 = abs(_a0470);

    _TMP51 = _df0468.x + _df0468.y + _df0468.z;

    if (_TMP50 > _TMP51) { 
        _TMP49 = _c4;

    } else {

        _TMP49 = _TMP46;

    } 
    _ret_0 = vec4(_TMP49.x, _TMP49.y, _TMP49.z, 1.00000000E+000);

    fragColor = _ret_0;
}
