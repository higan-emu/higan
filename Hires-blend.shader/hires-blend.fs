#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
vec4 ps = 1.0 / sourceSize[0];
vec4 l = texture2D(source[0], vec2(texCoord.x - ps.x, texCoord.y));
vec4 r = texture2D(source[0], texCoord);
if(sourceSize[0].x > 256) fragColor = mix(l, r, 0.5);

else

fragColor = texture2D(source[0], texCoord);

}