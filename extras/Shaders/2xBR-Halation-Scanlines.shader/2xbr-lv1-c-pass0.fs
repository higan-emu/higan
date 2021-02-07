#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];

in Vertex {
   vec2 texCoord;
   vec4 t1;
   vec4 t2;
   vec4 t3;
   vec4 t4;
   vec4 t5;
   vec4 t6;
   vec4 t7;
} vertexIn;

   out vec4 fragColor;

   uniform vec4 targetSize;
   uniform vec4 outputSize;

   const float threshold = 15.0;

   const float y_weight = 48.0;
   const float u_weight = 7.0;
   const float v_weight = 6.0;

   const mat3 yuv          = mat3(0.299, 0.587, 0.114, -0.169, -0.331, 0.499, 0.499, -0.418, -0.0813);
   const mat3 yuv_weighted = mat3(y_weight * yuv[0], u_weight * yuv[1], v_weight * yuv[2]);
   const vec4 bin          = vec4(1.0f, 2.0f, 4.0f, 8.0f);
   const vec4 maximo       = vec4(255.0f, 255.0f, 255.0f, 255.0f);


bvec4 _and_(bvec4 A, bvec4 B) {
   return bvec4(A.x && B.x, A.y && B.y, A.z && B.z, A.w && B.w);
}

bvec4 _or_(bvec4 A, bvec4 B) {
   return bvec4(A.x || B.x, A.y || B.y, A.z || B.z, A.w || B.w);
}

vec4 df(vec4 A, vec4 B) {
   return vec4(abs(A - B));
}

bvec4 close(vec4 A, vec4 B) {
   return (lessThan(df(A, B), vec4(threshold)));
}

vec4 weighted_distance(vec4 a, vec4 b, vec4 c, vec4 d, vec4 e, vec4 f, vec4 g, vec4 h) {
   return (df(a, b) + df(a, c) + df(d, e) + df(d, f) + 4.0 * df(g, h));
}

vec4 remapTo01(vec4 v, vec4 high) {
   return (v/high);
}

void main() {
   bvec4 edr, px; // px = pixel, edr = edge detection rule
   bvec4 interp_restriction_lv1;

   vec3 A1 = texture2D(source[0], vertexIn.t1.xw).rgb;
   vec3 B1 = texture2D(source[0], vertexIn.t1.yw).rgb;
   vec3 C1 = texture2D(source[0], vertexIn.t1.zw).rgb;

   vec3 A  = texture2D(source[0], vertexIn.t2.xw).rgb;
   vec3 B  = texture2D(source[0], vertexIn.t2.yw).rgb;
   vec3 C  = texture2D(source[0], vertexIn.t2.zw).rgb;

   vec3 D  = texture2D(source[0], vertexIn.t3.xw).rgb;
   vec3 E  = texture2D(source[0], vertexIn.t3.yw).rgb;
   vec3 F  = texture2D(source[0], vertexIn.t3.zw).rgb;

   vec3 G  = texture2D(source[0], vertexIn.t4.xw).rgb;
   vec3 H  = texture2D(source[0], vertexIn.t4.yw).rgb;
   vec3 I  = texture2D(source[0], vertexIn.t4.zw).rgb;

   vec3 G5 = texture2D(source[0], vertexIn.t5.xw).rgb;
   vec3 H5 = texture2D(source[0], vertexIn.t5.yw).rgb;
   vec3 I5 = texture2D(source[0], vertexIn.t5.zw).rgb;

   vec3 A0 = texture2D(source[0], vertexIn.t6.xy).rgb;
   vec3 D0 = texture2D(source[0], vertexIn.t6.xz).rgb;
   vec3 G0 = texture2D(source[0], vertexIn.t6.xw).rgb;

   vec3 C4 = texture2D(source[0], vertexIn.t7.xy).rgb;
   vec3 F4 = texture2D(source[0], vertexIn.t7.xz).rgb;
   vec3 I4 = texture2D(source[0], vertexIn.t7.xw).rgb;

   vec4 bdhf = yuv_weighted[0]*mat4x3( B,  D,  H,  F);
   vec4 cagi = yuv_weighted[0]*mat4x3( C,  A,  G,  I);
   vec4 e    = yuv_weighted[0]*mat4x3( E,  E,  E,  E);
   vec4 i4   = yuv_weighted[0]*mat4x3(I4, C4, A0, G0);
   vec4 i5   = yuv_weighted[0]*mat4x3(I5, C1, A1, G5);
   vec4 h5x  = yuv_weighted[0]*mat4x3(H5, F4, B1, D0);

   vec4 b = bdhf.xzzx;
   vec4 d = bdhf.yyww;
   vec4 h = bdhf.zxxz;
   vec4 f = bdhf.wwyy;
   vec4 c = cagi.xwzy;
   vec4 a = cagi.yzwx;
   vec4 g = cagi.zyxw;
   vec4 i = cagi.wxyz;

   vec4 f4 = h5x.yyww;
   vec4 h5 = h5x.xzzx;

   bvec4 r1 = _and_( notEqual(  e,  f ), notEqual( e,  h ) );
   bvec4 r2 = _and_( not(close( f,  b)), not(close(f,  c)) );
   bvec4 r3 = _and_( not(close( h,  d)), not(close(h,  g)) );
   bvec4 r4 = _and_( not(close( f, f4)), not(close(f, i4)) );
   bvec4 r5 = _and_( not(close( h, h5)), not(close(h, i5)) );
   bvec4 r6 = _and_( close(e, i),  _or_(r4, r5) );
   bvec4 r7 =  _or_( close(e, g), close( e,  c) );

   interp_restriction_lv1 = _and_( r1, _or_( _or_( _or_(r2, r3), r6 ), r7 ) );

   edr = _and_( lessThan(weighted_distance(e, c, g, i, h5, f4, h, f), weighted_distance(h, d, i5, f, i4, b, e, i)), interp_restriction_lv1 );

   px  = lessThanEqual(df(e, f), df(e, h));

   vec4 info = bin*mat4x4(
                          edr.x, px.x, 0.0, 0.0,
                          edr.y, px.y, 0.0, 0.0,
                          edr.z, px.z, 0.0, 0.0,
                          edr.w, px.w, 0.0, 0.0
                          );

   fragColor = vec4(remapTo01(info, maximo));
}