#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
uniform sampler2D pixmap[];

#define PHOSPHOR_SCALE 1.0
//set to 2 if you enable gaussian-vert.fs
#define ORG_ID 1

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
// using fract() on the coordinate is effectiveley the same as setting the LUT wrap mode to repeat
// so there is no need to do it here, 
// add fract() here if you can't set the wrap mode to repeat
vec2 LUTeffectiveCoord = texCoord.xy*targetSize.xy/vec2(4.0,2.0);// same as pixmapSize[0].zw

vec4 org = texture(source[ORG_ID], texCoord);
vec4 screen = texture(pixmap[0], LUTeffectiveCoord);
fragColor = (screen) * (org);
}

