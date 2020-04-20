#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];

in Vertex {
   vec2 texCoord;
   vec4 t1;
} vertexIn;

   out vec4 fragColor;

   uniform vec4 targetSize;
   uniform vec4 outputSize;

   const mat4x2 sym_vectors  = mat4x2(1,  1,   1, -1,   -1, -1,   -1,  1);

float remapFrom01(float v, float high)
{
   return (high*v + 0.5);
}

vec2 unpack_info(float i)
{
   vec2 info;
   info.x = round(modf(i/2.0f, i));
   info.y = i;

   return info;
}

void main() {
   float px, edr; // px = pixel to blend, edr = edge detection rule

   vec2 pos = fract(vertexIn.texCoord*sourceSize[0].xy)-vec2(0.5, 0.5); // pos = pixel position
   vec2 dir = sign(pos); // dir = pixel direction

   vec2 g1  = dir*vertexIn.t1.xy;
   vec2 g2  = dir*vertexIn.t1.zw;

   vec3 F   = texture2D(source[1], vertexIn.texCoord +g1).rgb;
   vec3 H   = texture2D(source[1], vertexIn.texCoord +g2).rgb;
   vec3 E   = texture2D(source[1], vertexIn.texCoord    ).rgb;

   vec4 icomp = round(clamp(dir*sym_vectors, vec4(0.0), vec4(1.0))); // choose info component
   float  info  = remapFrom01(dot(texture2D(source[0], vertexIn.texCoord), icomp), 255.0f); // retrieve 1st pass info
   vec2 flags = unpack_info(info); // retrieve 1st pass flags

   edr = flags.x;
   px  = flags.y;

   vec3 color = mix(E, mix(H, F, px), edr*0.5); // interpolate if there's edge

   fragColor = vec4(color, 1.0);
}