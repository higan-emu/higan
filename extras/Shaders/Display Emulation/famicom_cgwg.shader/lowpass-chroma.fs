#version 150

#in postfiltertaps
#in postfilterchromacutoff

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform int phase;

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

vec3 sample(vec2 shift) {
  vec2 coord = texCoord + sourceSize[0].zw*shift;
  if (coord.x < 0.0 || coord.x > 1.0) return vec3(0.0);
  return texture(source[0], texCoord + sourceSize[0].zw*shift).xyz-vec3(0,0.5,0.5);
}

float sinc(float x) {
  x = max(abs(x),0.0001);
  return sin(x)/x;
}

void main() {
  const int width = postfiltertaps;
  const float bw_c = postfilterchromacutoff/3.0;

  const vec4 one = vec4(1.0);
  const float PI = 3.14159265359;
  const vec4 PI_6 = vec4(PI/6.0);

  float norm_c = 0.0;
  vec3 yiq = vec3(0.0);
  yiq.x = sample(vec2(0,0)).x;
  for (int i = -width/2; i <= width/2; i++) {
    // Hamming window
    const float alpha = 0.54;
    const float beta  = 0.46;
    float window = alpha - beta * cos(2.0*PI/(width-1)*(width/2+i));

    float chromafilt = 2.0*bw_c*sinc(2.0*PI*bw_c*i);

    vec3 samp = sample(vec2(i,0));
    float filt = window*chromafilt;
    yiq.yz += samp.yz*vec2(filt);
    norm_c += filt;
  }

  yiq *= vec3(1.0, 1.0/norm_c, 1.0/norm_c);

  fragColor = vec4(dot(yiq, vec3(1.0, 0.946882, 0.623557)),
		   dot(yiq, vec3(1.0,-0.274788,-0.635691)),
		   dot(yiq, vec3(1.0,-1.108545, 1.709007)),
		   0.0);
}
