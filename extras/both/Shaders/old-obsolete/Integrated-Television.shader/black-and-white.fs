#version 150

uniform sampler2D source[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

vec3 desaturate(vec3 col)
{
   return vec3(dot(col, vec3(0.2126, 0.7152, 0.0722)));
}

void main() {
   fragColor = vec4(desaturate(texture(source[0], texCoord).rgb), 1.0);
}