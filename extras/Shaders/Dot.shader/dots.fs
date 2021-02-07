#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

      in vec2 c00;
      in vec2 c10;
      in vec2 c20;
      in vec2 c01;
      in vec2 c11;
      in vec2 c21;
      in vec2 c02;
      in vec2 c12;
      in vec2 c22;
      in vec2 pixel_no;

      const float gamma = 2.4;
      const float shine = 0.01;
      const float blend = 0.65;

      float dist(vec2 texCoord, vec2 source)
      {
         vec2 delta = texCoord - source;
         return sqrt(dot(delta, delta));
      }

      float color_bloom(vec3 color)
      {
         const vec3 gray_coeff = vec3(0.30, 0.59, 0.11);
         float bright = dot(color, gray_coeff);
         return mix(1.0 + shine, 1.0 - shine, bright);
      }

      vec3 lookup(float offset_x, float offset_y, vec2 texCoord)
      {
         vec2 offset = vec2(offset_x, offset_y);
         vec3 color = texture(source[0], texCoord).rgb;
         float delta = dist(fract(pixel_no), offset + vec2(0.5));
         return color * exp(-gamma * delta * color_bloom(color));
      }

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {

    vec3 mid_color = lookup(0.0, 0.0, c11);
         vec3 color = vec3(0.0);

         color += lookup(-1.0, -1.0, c00);
         color += lookup( 0.0, -1.0, c10);
         color += lookup( 1.0, -1.0, c20);
         color += lookup(-1.0,  0.0, c01);
         color += mid_color;
         color += lookup( 1.0,  0.0, c21);
         color += lookup(-1.0,  1.0, c02);
         color += lookup( 0.0,  1.0, c12);
         color += lookup( 1.0,  1.0, c22);
         vec3 out_color = mix(1.2 * mid_color, color, blend);

         fragColor = vec4(color, 1.0);
}