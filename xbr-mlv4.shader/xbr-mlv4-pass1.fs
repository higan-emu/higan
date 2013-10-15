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
vec4 _ret_0;
bvec4 _TMP86;
bvec4 _TMP85;
bvec4 _TMP84;
bvec4 _TMP83;
bvec4 _TMP82;
bvec4 _TMP81;
bvec4 _TMP80;
bvec4 _TMP79;
bvec4 _TMP78;
bvec4 _TMP77;
bvec4 _TMP76;
bvec4 _TMP75;
bvec4 _TMP74;
bvec4 _TMP73;
bvec4 _TMP72;
bvec4 _TMP71;
bvec4 _TMP70;
bvec4 _TMP69;
vec3 _TMP68;
vec3 _TMP67;
vec3 _TMP66;
vec3 _TMP65;
vec3 _TMP64;
vec3 _TMP63;
vec3 _TMP62;
vec3 _TMP61;
vec3 _TMP60;
vec3 _TMP59;
vec3 _TMP58;
vec3 _TMP57;
vec3 _TMP56;
vec3 _TMP55;
vec3 _TMP54;
vec3 _TMP53;
vec3 _TMP52;
vec3 _TMP51;
vec3 _TMP50;
vec3 _TMP49;
vec3 _TMP48;
vec3 _TMP47;
vec3 _TMP46;
vec3 _TMP45;
vec3 _TMP44;
vec3 _TMP43;
vec3 _TMP42;
vec3 _TMP41;
vec3 _TMP40;
vec3 _TMP39;
vec3 _TMP38;
vec3 _TMP37;
vec3 _TMP36;
vec3 _TMP35;
vec3 _TMP34;
vec3 _TMP33;
vec4 _TMP32;
vec4 _TMP31;
vec4 _TMP30;
vec4 _TMP29;
vec4 _TMP28;
vec4 _TMP27;
vec4 _TMP26;
vec4 _TMP25;
vec4 _TMP24;
vec4 _TMP23;
vec4 _TMP22;
vec4 _TMP21;
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

vec3 _C0207[4];
vec4 _r0217;
vec3 _C0227[4];
vec4 _r0237;
vec3 _C0247[4];
vec4 _r0257;
vec3 _C0267[4];
vec4 _r0277;
vec3 _C0287[4];
vec4 _r0297;
vec3 _C0307[4];
vec4 _r0317;
vec3 _C0327[4];
vec4 _r0337;
vec3 _C0347[4];
vec4 _r0357;
vec3 _C0367[4];
vec4 _r0377;
vec4 _TMP388;
vec4 _a0391;
vec4 _TMP394;
vec4 _a0397;
vec4 _TMP400;
vec4 _a0403;
vec4 _TMP406;
vec4 _a0409;
vec4 _TMP412;
vec4 _a0415;
vec4 _TMP418;
vec4 _a0421;
vec4 _TMP424;
vec4 _a0427;
vec4 _TMP430;
vec4 _a0433;
vec4 _TMP436;
vec4 _a0439;
vec4 _TMP442;
vec4 _a0445;
vec4 _TMP448;
vec4 _a0451;
vec4 _TMP454;
vec4 _a0457;
vec4 _TMP460;
vec4 _a0463;
vec4 _TMP464;
vec4 _a0467;
vec4 _TMP468;
vec4 _a0471;
vec4 _TMP472;
vec4 _a0475;
vec4 _TMP476;
vec4 _a0479;
vec4 _TMP482;
vec4 _a0485;
vec4 _TMP486;
vec4 _a0489;
vec4 _TMP490;
vec4 _a0493;
vec4 _TMP494;
vec4 _a0497;
vec4 _TMP498;
vec4 _a0501;
vec4 _TMP504;
vec4 _a0507;
vec4 _TMP510;
vec4 _a0513;
vec4 _TMP514;
vec4 _a0517;
vec4 _TMP520;
vec4 _a0523;
vec4 _TMP526;
vec4 _a0529;
vec4 _TMP530;
vec4 _a0533;
vec4 _TMP536;
vec4 _a0539;
vec4 _TMP542;
vec4 _a0545;
vec4 _TMP546;
vec4 _a0549;
vec4 _TMP550;
vec4 _a0553;
vec4 _TMP554;
vec4 _a0557;
vec4 _TMP558;
vec4 _a0561;
vec4 _TMP562;
vec4 _a0565;
vec4 _TMP566;
vec4 _a0569;
vec4 _TMP570;
vec4 _a0573;
vec4 _TMP574;
vec4 _a0577;
vec4 _TMP578;
vec4 _a0581;
vec4 _TMP582;
vec4 _a0585;
vec4 _TMP586;
vec4 _a0589;
vec4 _TMP590;
vec4 _a0593;
void main()
{

    bvec4 _edr0;
    bvec4 _edr;
    bvec4 _edr_left;
    bvec4 _edr_up;
    bvec4 _edr3_left;
    bvec4 _edr3_up;
    bvec4 _edr4_left;
    bvec4 _edr4_up;
    bvec4 _interp_restriction_lv1;
    bvec4 _interp_restriction_lv2_left;
    bvec4 _interp_restriction_lv2_up;
    bvec4 _interp_restriction_lv3_left;
    bvec4 _interp_restriction_lv3_up;
    bvec4 _interp_restriction_lv4_left;
    bvec4 _interp_restriction_lv4_up;
    vec4 _wd1;
    vec4 _wd2;
    vec4 _info;

    _TMP0 = texture(source[0], texCoord);
    _TMP1 = texture(source[0], t1.xz);
    _TMP2 = texture(source[0], t2.xz);
    _TMP3 = texture(source[0], t1.yz);
    _TMP4 = texture(source[0], t2.yz);
    _TMP5 = texture(source[0], t1.xw);
    _TMP6 = texture(source[0], t2.xw);
    _TMP7 = texture(source[0], t1.yw);
    _TMP8 = texture(source[0], t2.yw);
    _TMP9 = texture(source[0], t3.xz);
    _TMP10 = texture(source[0], t4.xz);
    _TMP11 = texture(source[0], t3.yz);
    _TMP12 = texture(source[0], t4.yz);
    _TMP13 = texture(source[0], t3.xw);
    _TMP14 = texture(source[0], t4.xw);
    _TMP15 = texture(source[0], t3.yw);
    _TMP16 = texture(source[0], t4.yw);
    _TMP17 = texture(source[0], t5.xz);
    _TMP18 = texture(source[0], t5.yz);
    _TMP19 = texture(source[0], t5.xw);
    _TMP20 = texture(source[0], t5.yw);
    _TMP21 = texture(source[0], t6.xy);
    _TMP22 = texture(source[0], t7.zw);
    _TMP23 = texture(source[0], t6.xz);
    _TMP24 = texture(source[0], t7.yw);
    _TMP25 = texture(source[0], t6.xw);
    _TMP26 = texture(source[0], t7.xw);
    _TMP27 = texture(source[0], t8.xy);
    _TMP28 = texture(source[0], t9.zw);
    _TMP29 = texture(source[0], t8.xz);
    _TMP30 = texture(source[0], t9.yw);
    _TMP31 = texture(source[0], t8.xw);
    _TMP32 = texture(source[0], t9.xw);
    _C0207[0] = _TMP21.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP21.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP21.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0207[1] = _TMP22.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP22.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP22.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0207[2] = _TMP27.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP27.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP27.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0207[3] = _TMP28.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP28.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP28.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _TMP33 = abs(_C0207[0]);
    _TMP34 = abs(_C0207[1]);
    _TMP35 = abs(_C0207[2]);
    _TMP36 = abs(_C0207[3]);
    _r0217.x = dot(_TMP33, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0217.y = dot(_TMP34, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0217.z = dot(_TMP35, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0217.w = dot(_TMP36, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _C0227[0] = _TMP18.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP18.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP18.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0227[1] = _TMP17.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP17.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP17.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0227[2] = _TMP19.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP19.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP19.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0227[3] = _TMP20.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP20.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP20.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _TMP37 = abs(_C0227[0]);
    _TMP38 = abs(_C0227[1]);
    _TMP39 = abs(_C0227[2]);
    _TMP40 = abs(_C0227[3]);
    _r0237.x = dot(_TMP37, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0237.y = dot(_TMP38, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0237.z = dot(_TMP39, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0237.w = dot(_TMP40, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _C0247[0] = _TMP0.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP0.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP0.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0247[1] = _TMP0.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP0.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP0.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0247[2] = _TMP0.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP0.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP0.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0247[3] = _TMP0.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP0.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP0.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _TMP41 = abs(_C0247[0]);
    _TMP42 = abs(_C0247[1]);
    _TMP43 = abs(_C0247[2]);
    _TMP44 = abs(_C0247[3]);
    _r0257.x = dot(_TMP41, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0257.y = dot(_TMP42, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0257.z = dot(_TMP43, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0257.w = dot(_TMP44, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _C0267[0] = _TMP7.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP7.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP7.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0267[1] = _TMP4.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP4.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP4.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0267[2] = _TMP1.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP1.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP1.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0267[3] = _TMP6.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP6.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP6.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _TMP45 = abs(_C0267[0]);
    _TMP46 = abs(_C0267[1]);
    _TMP47 = abs(_C0267[2]);
    _TMP48 = abs(_C0267[3]);
    _r0277.x = dot(_TMP45, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0277.y = dot(_TMP46, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0277.z = dot(_TMP47, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0277.w = dot(_TMP48, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _C0287[0] = _TMP8.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP8.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP8.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0287[1] = _TMP3.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP3.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP3.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0287[2] = _TMP2.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP2.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP2.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0287[3] = _TMP5.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP5.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP5.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _TMP49 = abs(_C0287[0]);
    _TMP50 = abs(_C0287[1]);
    _TMP51 = abs(_C0287[2]);
    _TMP52 = abs(_C0287[3]);
    _r0297.x = dot(_TMP49, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0297.y = dot(_TMP50, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0297.z = dot(_TMP51, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0297.w = dot(_TMP52, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _C0307[0] = _TMP29.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP29.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP29.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0307[1] = _TMP30.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP30.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP30.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0307[2] = _TMP23.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP23.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP23.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0307[3] = _TMP24.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP24.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP24.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _TMP53 = abs(_C0307[0]);
    _TMP54 = abs(_C0307[1]);
    _TMP55 = abs(_C0307[2]);
    _TMP56 = abs(_C0307[3]);
    _r0317.x = dot(_TMP53, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0317.y = dot(_TMP54, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0317.z = dot(_TMP55, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0317.w = dot(_TMP56, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _C0327[0] = _TMP15.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP15.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP15.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0327[1] = _TMP12.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP12.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP12.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0327[2] = _TMP9.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP9.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP9.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0327[3] = _TMP14.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP14.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP14.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _TMP57 = abs(_C0327[0]);
    _TMP58 = abs(_C0327[1]);
    _TMP59 = abs(_C0327[2]);
    _TMP60 = abs(_C0327[3]);
    _r0337.x = dot(_TMP57, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0337.y = dot(_TMP58, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0337.z = dot(_TMP59, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0337.w = dot(_TMP60, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _C0347[0] = _TMP16.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP16.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP16.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0347[1] = _TMP11.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP11.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP11.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0347[2] = _TMP10.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP10.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP10.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0347[3] = _TMP13.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP13.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP13.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _TMP61 = abs(_C0347[0]);
    _TMP62 = abs(_C0347[1]);
    _TMP63 = abs(_C0347[2]);
    _TMP64 = abs(_C0347[3]);
    _r0357.x = dot(_TMP61, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0357.y = dot(_TMP62, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0357.z = dot(_TMP63, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0357.w = dot(_TMP64, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _C0367[0] = _TMP31.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP31.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP31.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0367[1] = _TMP32.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP32.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP32.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0367[2] = _TMP25.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP25.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP25.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _C0367[3] = _TMP26.x*vec3( 2.98999995E-001, -1.69000000E-001, 4.99000013E-001) + _TMP26.y*vec3( 5.87000012E-001, -3.31000000E-001, -4.18000013E-001) + _TMP26.z*vec3( 1.14000000E-001, 4.99000013E-001, -8.12999979E-002);
    _TMP65 = abs(_C0367[0]);
    _TMP66 = abs(_C0367[1]);
    _TMP67 = abs(_C0367[2]);
    _TMP68 = abs(_C0367[3]);
    _r0377.x = dot(_TMP65, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0377.y = dot(_TMP66, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0377.z = dot(_TMP67, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _r0377.w = dot(_TMP68, vec3( 4.00000000E+000, 1.00000000E+000, 2.00000000E+000));
    _interp_restriction_lv1 = bvec4(_r0257.x != _r0217.w && _r0257.x != _r0217.z, _r0257.y != _r0217.x && _r0257.y != _r0217.w, _r0257.z != _r0217.y && _r0257.z != _r0217.x, _r0257.w != _r0217.z && _r0257.w != _r0217.y);
    _a0391 = _r0257 - _r0217.yzwx;
    _TMP388 = abs(_a0391);
    _TMP69 = bvec4(_TMP388.x < 2.00000000E+000, _TMP388.y < 2.00000000E+000, _TMP388.z < 2.00000000E+000, _TMP388.w < 2.00000000E+000);
    _a0397 = _r0217.zwxy - _r0237.zwxy;
    _TMP394 = abs(_a0397);
    _TMP70 = bvec4(_TMP394.x < 2.00000000E+000, _TMP394.y < 2.00000000E+000, _TMP394.z < 2.00000000E+000, _TMP394.w < 2.00000000E+000);
    _interp_restriction_lv2_left = bvec4(_r0257.x != _r0237.z && _r0217.y != _r0237.z && (_TMP69.x || _TMP70.x), _r0257.y != _r0237.w && _r0217.z != _r0237.w && (_TMP69.y || _TMP70.y), _r0257.z != _r0237.x && _r0217.w != _r0237.x && (_TMP69.z || _TMP70.z), _r0257.w != _r0237.y && _r0217.x != _r0237.y && (_TMP69.w || _TMP70.w));
    _a0403 = _r0257 - _r0217;
    _TMP400 = abs(_a0403);
    _TMP71 = bvec4(_TMP400.x < 2.00000000E+000, _TMP400.y < 2.00000000E+000, _TMP400.z < 2.00000000E+000, _TMP400.w < 2.00000000E+000);
    _a0409 = _r0217.wxyz - _r0237;
    _TMP406 = abs(_a0409);
    _TMP72 = bvec4(_TMP406.x < 2.00000000E+000, _TMP406.y < 2.00000000E+000, _TMP406.z < 2.00000000E+000, _TMP406.w < 2.00000000E+000);
    _interp_restriction_lv2_up = bvec4(_r0257.x != _r0237.x && _r0217.x != _r0237.x && (_TMP71.x || _TMP72.x), _r0257.y != _r0237.y && _r0217.y != _r0237.y && (_TMP71.y || _TMP72.y), _r0257.z != _r0237.z && _r0217.z != _r0237.z && (_TMP71.z || _TMP72.z), _r0257.w != _r0237.w && _r0217.w != _r0237.w && (_TMP71.w || _TMP72.w));
    _a0415 = _r0217.yzwx - _r0317.wxyz;
    _TMP412 = abs(_a0415);
    _TMP73 = bvec4(_TMP412.x < 2.00000000E+000, _TMP412.y < 2.00000000E+000, _TMP412.z < 2.00000000E+000, _TMP412.w < 2.00000000E+000);
    _a0421 = _r0237.zwxy - _r0297.wxyz;
    _TMP418 = abs(_a0421);
    _TMP74 = bvec4(_TMP418.x < 2.00000000E+000, _TMP418.y < 2.00000000E+000, _TMP418.z < 2.00000000E+000, _TMP418.w < 2.00000000E+000);
    _interp_restriction_lv3_left = bvec4(_r0257.x != _r0297.w && _r0317.w != _r0297.w && (_TMP73.x || _TMP74.x), _r0257.y != _r0297.x && _r0317.x != _r0297.x && (_TMP73.y || _TMP74.y), _r0257.z != _r0297.y && _r0317.y != _r0297.y && (_TMP73.z || _TMP74.z), _r0257.w != _r0297.z && _r0317.z != _r0297.z && (_TMP73.w || _TMP74.w));
    _a0427 = _r0217 - _r0317.zwxy;
    _TMP424 = abs(_a0427);
    _TMP75 = bvec4(_TMP424.x < 2.00000000E+000, _TMP424.y < 2.00000000E+000, _TMP424.z < 2.00000000E+000, _TMP424.w < 2.00000000E+000);
    _a0433 = _r0237 - _r0277.yzwx;
    _TMP430 = abs(_a0433);
    _TMP76 = bvec4(_TMP430.x < 2.00000000E+000, _TMP430.y < 2.00000000E+000, _TMP430.z < 2.00000000E+000, _TMP430.w < 2.00000000E+000);
    _interp_restriction_lv3_up = bvec4(_r0257.x != _r0277.y && _r0317.z != _r0277.y && (_TMP75.x || _TMP76.x), _r0257.y != _r0277.z && _r0317.w != _r0277.z && (_TMP75.y || _TMP76.y), _r0257.z != _r0277.w && _r0317.x != _r0277.w && (_TMP75.z || _TMP76.z), _r0257.w != _r0277.x && _r0317.y != _r0277.x && (_TMP75.w || _TMP76.w));
    _a0439 = _r0317.wxyz - _r0377.wxyz;
    _TMP436 = abs(_a0439);
    _TMP77 = bvec4(_TMP436.x < 2.00000000E+000, _TMP436.y < 2.00000000E+000, _TMP436.z < 2.00000000E+000, _TMP436.w < 2.00000000E+000);
    _a0445 = _r0297.wxyz - _r0357.wxyz;
    _TMP442 = abs(_a0445);
    _TMP78 = bvec4(_TMP442.x < 2.00000000E+000, _TMP442.y < 2.00000000E+000, _TMP442.z < 2.00000000E+000, _TMP442.w < 2.00000000E+000);
    _interp_restriction_lv4_left = bvec4(_r0257.x != _r0357.w && _r0377.w != _r0357.w && (_TMP77.x || _TMP78.x), _r0257.y != _r0357.x && _r0377.x != _r0357.x && (_TMP77.y || _TMP78.y), _r0257.z != _r0357.y && _r0377.y != _r0357.y && (_TMP77.z || _TMP78.z), _r0257.w != _r0357.z && _r0377.z != _r0357.z && (_TMP77.w || _TMP78.w));
    _a0451 = _r0317.zwxy - _r0377.zwxy;
    _TMP448 = abs(_a0451);
    _TMP79 = bvec4(_TMP448.x < 2.00000000E+000, _TMP448.y < 2.00000000E+000, _TMP448.z < 2.00000000E+000, _TMP448.w < 2.00000000E+000);
    _a0457 = _r0277.yzwx - _r0337.yzwx;
    _TMP454 = abs(_a0457);
    _TMP80 = bvec4(_TMP454.x < 2.00000000E+000, _TMP454.y < 2.00000000E+000, _TMP454.z < 2.00000000E+000, _TMP454.w < 2.00000000E+000);
    _interp_restriction_lv4_up = bvec4(_r0257.x != _r0337.y && _r0377.z != _r0337.y && (_TMP79.x || _TMP80.x), _r0257.y != _r0337.z && _r0377.w != _r0337.z && (_TMP79.y || _TMP80.y), _r0257.z != _r0337.w && _r0377.x != _r0337.w && (_TMP79.z || _TMP80.z), _r0257.w != _r0337.x && _r0377.y != _r0337.x && (_TMP79.w || _TMP80.w));
    _a0463 = _r0257 - _r0237;
    _TMP460 = abs(_a0463);
    _a0467 = _r0257 - _r0237.zwxy;
    _TMP464 = abs(_a0467);
    _a0471 = _r0237.wxyz - _r0317;
    _TMP468 = abs(_a0471);
    _a0475 = _r0237.wxyz - _r0317.yzwx;
    _TMP472 = abs(_a0475);
    _a0479 = _r0217.zwxy - _r0217.wxyz;
    _TMP476 = abs(_a0479);
    _wd1 = _TMP460 + _TMP464 + _TMP468 + _TMP472 + 4.00000000E+000*_TMP476;
    _a0485 = _r0217.zwxy - _r0217.yzwx;
    _TMP482 = abs(_a0485);
    _a0489 = _r0217.zwxy - _r0297;
    _TMP486 = abs(_a0489);
    _a0493 = _r0217.wxyz - _r0277;
    _TMP490 = abs(_a0493);
    _a0497 = _r0217.wxyz - _r0217;
    _TMP494 = abs(_a0497);
    _a0501 = _r0257 - _r0237.wxyz;
    _TMP498 = abs(_a0501);
    _wd2 = _TMP482 + _TMP486 + _TMP490 + _TMP494 + 4.00000000E+000*_TMP498;
    _edr0 = bvec4(_wd1.x <= _wd2.x && _interp_restriction_lv1.x, _wd1.y <= _wd2.y && _interp_restriction_lv1.y, _wd1.z <= _wd2.z && _interp_restriction_lv1.z, _wd1.w <= _wd2.w && _interp_restriction_lv1.w);
    _a0507 = _r0217.wxyz - _r0217;
    _TMP504 = abs(_a0507);
    _TMP81 = bvec4(_TMP504.x < 2.00000000E+000, _TMP504.y < 2.00000000E+000, _TMP504.z < 2.00000000E+000, _TMP504.w < 2.00000000E+000);
    _a0513 = _r0217.wxyz - _r0217;
    _TMP510 = abs(_a0513);
    _a0517 = _r0217.wxyz - _r0237;
    _TMP514 = abs(_a0517);
    _TMP82 = bvec4(_TMP510.x > _TMP514.x, _TMP510.y > _TMP514.y, _TMP510.z > _TMP514.z, _TMP510.w > _TMP514.w);
    _a0523 = _r0217.zwxy - _r0217.yzwx;
    _TMP520 = abs(_a0523);
    _TMP83 = bvec4(_TMP520.x < 2.00000000E+000, _TMP520.y < 2.00000000E+000, _TMP520.z < 2.00000000E+000, _TMP520.w < 2.00000000E+000);
    _a0529 = _r0217.zwxy - _r0217.yzwx;
    _TMP526 = abs(_a0529);
    _a0533 = _r0217.zwxy - _r0237.zwxy;
    _TMP530 = abs(_a0533);
    _TMP84 = bvec4(_TMP526.x > _TMP530.x, _TMP526.y > _TMP530.y, _TMP526.z > _TMP530.z, _TMP526.w > _TMP530.w);
    _a0539 = _r0257 - _r0237.zwxy;
    _TMP536 = abs(_a0539);
    _TMP85 = bvec4(_TMP536.x < 2.00000000E+000, _TMP536.y < 2.00000000E+000, _TMP536.z < 2.00000000E+000, _TMP536.w < 2.00000000E+000);
    _a0545 = _r0257 - _r0237;
    _TMP542 = abs(_a0545);
    _TMP86 = bvec4(_TMP542.x < 2.00000000E+000, _TMP542.y < 2.00000000E+000, _TMP542.z < 2.00000000E+000, _TMP542.w < 2.00000000E+000);
    _edr = bvec4(_wd1.x < _wd2.x && _interp_restriction_lv1.x && (!_TMP81.x && !_TMP82.x || !_TMP83.x && !_TMP84.x || _TMP85.x || _TMP86.x), _wd1.y < _wd2.y && _interp_restriction_lv1.y && (!_TMP81.y && !_TMP82.y || !_TMP83.y && !_TMP84.y || _TMP85.y || _TMP86.y), _wd1.z < _wd2.z && _interp_restriction_lv1.z && (!_TMP81.z && !_TMP82.z || !_TMP83.z && !_TMP84.z || _TMP85.z || _TMP86.z), _wd1.w < _wd2.w && _interp_restriction_lv1.w && (!_TMP81.w && !_TMP82.w || !_TMP83.w && !_TMP84.w || _TMP85.w || _TMP86.w));
    _a0549 = _r0217.wxyz - _r0237.zwxy;
    _TMP546 = abs(_a0549);
    _a0553 = _r0217.zwxy - _r0237;
    _TMP550 = abs(_a0553);
    _edr_left = bvec4((2.00000000E+000*_TMP546).x <= _TMP550.x && _interp_restriction_lv2_left.x && _edr.x, (2.00000000E+000*_TMP546).y <= _TMP550.y && _interp_restriction_lv2_left.y && _edr.y, (2.00000000E+000*_TMP546).z <= _TMP550.z && _interp_restriction_lv2_left.z && _edr.z, (2.00000000E+000*_TMP546).w <= _TMP550.w && _interp_restriction_lv2_left.w && _edr.w);
    _a0557 = _r0217.wxyz - _r0237.zwxy;
    _TMP554 = abs(_a0557);
    _a0561 = _r0217.zwxy - _r0237;
    _TMP558 = abs(_a0561);
    _edr_up = bvec4(_TMP554.x >= (2.00000000E+000*_TMP558).x && _interp_restriction_lv2_up.x && _edr.x, _TMP554.y >= (2.00000000E+000*_TMP558).y && _interp_restriction_lv2_up.y && _edr.y, _TMP554.z >= (2.00000000E+000*_TMP558).z && _interp_restriction_lv2_up.z && _edr.z, _TMP554.w >= (2.00000000E+000*_TMP558).w && _interp_restriction_lv2_up.w && _edr.w);
    _a0565 = _r0217.wxyz - _r0297.wxyz;
    _TMP562 = abs(_a0565);
    _a0569 = _r0217.zwxy - _r0277.yzwx;
    _TMP566 = abs(_a0569);
    _edr3_left = bvec4((4.00000000E+000*_TMP562).x <= _TMP566.x && _interp_restriction_lv3_left.x && _edr_left.x, (4.00000000E+000*_TMP562).y <= _TMP566.y && _interp_restriction_lv3_left.y && _edr_left.y, (4.00000000E+000*_TMP562).z <= _TMP566.z && _interp_restriction_lv3_left.z && _edr_left.z, (4.00000000E+000*_TMP562).w <= _TMP566.w && _interp_restriction_lv3_left.w && _edr_left.w);
    _a0573 = _r0217.wxyz - _r0297.wxyz;
    _TMP570 = abs(_a0573);
    _a0577 = _r0217.zwxy - _r0277.yzwx;
    _TMP574 = abs(_a0577);
    _edr3_up = bvec4(_TMP570.x >= (4.00000000E+000*_TMP574).x && _interp_restriction_lv3_up.x && _edr_up.x, _TMP570.y >= (4.00000000E+000*_TMP574).y && _interp_restriction_lv3_up.y && _edr_up.y, _TMP570.z >= (4.00000000E+000*_TMP574).z && _interp_restriction_lv3_up.z && _edr_up.z, _TMP570.w >= (4.00000000E+000*_TMP574).w && _interp_restriction_lv3_up.w && _edr_up.w);
    _a0581 = _r0217.wxyz - _r0357.wxyz;
    _TMP578 = abs(_a0581);
    _a0585 = _r0217.zwxy - _r0337.yzwx;
    _TMP582 = abs(_a0585);
    _edr4_left = bvec4((4.00000000E+000*_TMP578).x <= _TMP582.x && _interp_restriction_lv4_left.x && _edr3_left.x, (4.00000000E+000*_TMP578).y <= _TMP582.y && _interp_restriction_lv4_left.y && _edr3_left.y, (4.00000000E+000*_TMP578).z <= _TMP582.z && _interp_restriction_lv4_left.z && _edr3_left.z, (4.00000000E+000*_TMP578).w <= _TMP582.w && _interp_restriction_lv4_left.w && _edr3_left.w);
    _a0589 = _r0217.wxyz - _r0357.wxyz;
    _TMP586 = abs(_a0589);
    _a0593 = _r0217.zwxy - _r0337.yzwx;
    _TMP590 = abs(_a0593);
    _edr4_up = bvec4(_TMP586.x >= (4.00000000E+000*_TMP590).x && _interp_restriction_lv4_up.x && _edr3_up.x, _TMP586.y >= (4.00000000E+000*_TMP590).y && _interp_restriction_lv4_up.y && _edr3_up.y, _TMP586.z >= (4.00000000E+000*_TMP590).z && _interp_restriction_lv4_up.z && _edr3_up.z, _TMP586.w >= (4.00000000E+000*_TMP590).w && _interp_restriction_lv4_up.w && _edr3_up.w);
    _info = vec4(_edr4_left.x && !_edr4_up.x ? 8.00000000E+000 : _edr4_up.x && !_edr4_left.x ? 7.00000000E+000 : _edr3_left.x && !_edr3_up.x ? 6.00000000E+000 : _edr3_up.x && !_edr3_left.x ? 5.00000000E+000 : _edr_left.x && !_edr_up.x ? 4.00000000E+000 : _edr_up.x && !_edr_left.x ? 3.00000000E+000 : _edr.x ? 2.00000000E+000 : _edr0.x ? 1.00000000E+000 : 0.00000000E+000, _edr4_left.y && !_edr4_up.y ? 8.00000000E+000 : _edr4_up.y && !_edr4_left.y ? 7.00000000E+000 : _edr3_left.y && !_edr3_up.y ? 6.00000000E+000 : _edr3_up.y && !_edr3_left.y ? 5.00000000E+000 : _edr_left.y && !_edr_up.y ? 4.00000000E+000 : _edr_up.y && !_edr_left.y ? 3.00000000E+000 : _edr.y ? 2.00000000E+000 : _edr0.y ? 1.00000000E+000 : 0.00000000E+000, _edr4_left.z && !_edr4_up.z ? 8.00000000E+000 : _edr4_up.z && !_edr4_left.z ? 7.00000000E+000 : _edr3_left.z && !_edr3_up.z ? 6.00000000E+000 : _edr3_up.z && !_edr3_left.z ? 5.00000000E+000 : _edr_left.z && !_edr_up.z ? 4.00000000E+000 : _edr_up.z && !_edr_left.z ? 3.00000000E+000 : _edr.z ? 2.00000000E+000 : _edr0.z ? 1.00000000E+000 : 0.00000000E+000, _edr4_left.w && !_edr4_up.w ? 8.00000000E+000 : _edr4_up.w && !_edr4_left.w ? 7.00000000E+000 : _edr3_left.w && !_edr3_up.w ? 6.00000000E+000 : _edr3_up.w && !_edr3_left.w ? 5.00000000E+000 : _edr_left.w && !_edr_up.w ? 4.00000000E+000 : _edr_up.w && !_edr_left.w ? 3.00000000E+000 : _edr.w ? 2.00000000E+000 : _edr0.w ? 1.00000000E+000 : 0.00000000E+000);
    _ret_0 = _info/vec4( 2.55000000E+002, 2.55000000E+002, 2.55000000E+002, 2.55000000E+002);
    fragColor = _ret_0;
}
