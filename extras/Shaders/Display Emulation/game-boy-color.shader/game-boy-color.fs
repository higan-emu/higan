#version 150

#in coloremulation

uniform sampler2D source[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
  vec4 rgba = texture(source[0], texCoord);
  #ifndef coloremulation
  fragColor = rgba;
  #else
  float r = rgba[0];
  float g = rgba[1];
  float b = rgba[2];
  fragColor = vec4(
    min(r * 806 + g * 124 + b *  62, 960.0) / 1023.0,
    min(          g * 744 + b * 248, 960.0) / 1023.0,
    min(r * 186 + g * 124 + b * 682, 960.0) / 1023.0,
    0.0
  );
  #endif
}