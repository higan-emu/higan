#version 150

#in filtertaps
#in lumacutoff
#in chromacutoff

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform int phase;

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

vec4 sample(vec2 coord) {
  const float min = 0.350*0.746;
  const float max = 1.962;
  const float black = 0.518;
  if (coord.x < 0.0 || coord.x > 1.0) return vec4(0.0);
  return (texture(source[0], coord) * vec4(max-min) + vec4(min-black)) / vec4(max-black);
}

vec4 sinc(vec4 x) {
  x = max(abs(x),vec4(0.0001));
  return sin(x)/x;
}

void main() {
  const int width = filtertaps;
  const float bw_y =   lumacutoff/12.0;
  const float bw_c = chromacutoff/12.0;

  int x = int(floor(texCoord.x*sourceSize[0].x));
  int y = int(floor(texCoord.y*sourceSize[0].y));
  int p = (x+y+phase) % 3;

  const vec4 one = vec4(1.0);
  const float PI = 3.14159265359;
  const vec4 PI_6 = vec4(PI/6.0);

  float norm_y = 0.0;
  float norm_c = 0.0;
  vec3 yiq = vec3(0.0);
  for (int i = -width/2; i <= width/2; i++) {
    // Hamming window
    const float alpha = 0.54;
    const float beta  = 0.46;
    vec4 window = vec4(alpha) - vec4(beta) * cos(vec4(2.0*PI/(4*width-1))*(vec4(4*(width/2+i))+vec4(0,1,2,3)));

    vec4 t = vec4(i*4)+vec4(-1.5,-0.5,+0.5,+1.5);
    vec4 lumafilt   = vec4(2.0*bw_y)*sinc(vec4(2.0*PI*bw_y)*t);
    vec4 chromafilt = vec4(2.0*bw_c)*sinc(vec4(2.0*PI*bw_c)*t);

    vec4 samp = sample(texCoord + vec2(i*sourceSize[0].z,0));
    vec4 filt = window*lumafilt;
    yiq.x += dot(samp, filt);
    norm_y += dot(one, filt);
    filt = window*chromafilt;
    yiq.y += dot(samp, filt*cos((vec4(0,1,2,3)+vec4(4*(i+p)))*PI_6));
    yiq.z += dot(samp, filt*sin((vec4(0,1,2,3)+vec4(4*(i+p)))*PI_6));
    norm_c += dot(one,filt);
  }

  yiq *= vec3(1.0/norm_y, 2.0/norm_c, 2.0/norm_c);

  fragColor = vec4(dot(yiq, vec3(1.0, 0.946882, 0.623557)),
		   dot(yiq, vec3(1.0,-0.274788,-0.635691)),
		   dot(yiq, vec3(1.0,-1.108545, 1.709007)),
		   0.0);
  //fragColor = vec4(yiq+vec3(0,0.5,0.5),0.0);
}
