#version 150

#in persistence

uniform sampler2D source[];

uniform sampler2D history[];
vec3 color;

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
vec3 color = texture(history[7], texCoord).rgb;
color = mix(texture(history[6], texCoord).rgb, color, persistence);
color = mix(texture(history[5], texCoord).rgb, color, persistence);
color = mix(texture(history[4], texCoord).rgb, color, persistence);
color = mix(texture(history[3], texCoord).rgb, color, persistence);
color = mix(texture(history[2], texCoord).rgb, color, persistence);
color = mix(texture(history[1], texCoord).rgb, color, persistence);
color = mix(texture(history[0], texCoord).rgb, color, persistence);
color = mix(texture(source[0], texCoord).rgb, color, persistence);
  fragColor = vec4(color,1.0);
}
