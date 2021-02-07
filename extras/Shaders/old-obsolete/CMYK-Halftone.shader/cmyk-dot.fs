#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
uniform int phase;

in vec2 st2;
in vec2 nearest;
in vec3 texcolor;
in vec3 white;
in vec3 black;
in vec4 cmyk;
in vec2 Kst;
in vec2 Kuv;
in vec2 Cst;
in vec2 Cuv;
in vec2 Mst;
in vec2 Muv;
in vec2 Yst;
in vec2 Yuv;
in vec3 rgbscreen;
in float value;
in vec3 newcolor;

#define frequency  253

in Vertex {
  vec2 texCoord;
};

float aastep(float threshold, float value) {
  float afwidth = 0.7 * length(vec2(dFdx(value), dFdy(value)));
  return smoothstep(threshold-afwidth, threshold+afwidth, value);
}

out vec4 fragColor;
 
void main() {
    // Sample the colors
	vec3 texcolor = texture(source[0], texCoord).rgb;
	
	// Perform a rough RGB-to-CMYK conversion
    vec4 cmyk;
    cmyk.xyz = 1.0 - texcolor;
    cmyk.w = min(cmyk.x, min(cmyk.y, cmyk.z)); // Create K
    cmyk.xyz -= cmyk.w; // Subtract K equivalent from CMY
	
    vec3 white = vec3(1.0, 1.0, 1.0);
    vec3 black = vec3(0.0, 0.0, 0.0);

    // Distance to nearest point in a grid of
    // (frequency x frequency) points over the unit square
    vec2 Kst = frequency * mat2(0.707, -0.707, 0.707, 0.707) * texCoord;
    vec2 Kuv = 2.0 * fract(Kst) - 1.0;
    float k = aastep(0.0, sqrt(cmyk.w) - length(Kuv));
    vec2 Cst = frequency * mat2(0.966, -0.259, 0.259, 0.966) * texCoord;
    vec2 Cuv = 2.0 * fract(Cst) - 1.0;
    float c = aastep(0.0, sqrt(cmyk.x) - length(Cuv));
    vec2 Mst = frequency * mat2(0.966, 0.259, -0.259, 0.966) * texCoord;
    vec2 Muv = 2.0 * fract(Mst) - 1.0;
    float m = aastep(0.0, sqrt(cmyk.y) - length(Muv));
    vec2 Yst = frequency * texCoord; // 0 deg
    vec2 Yuv = 2.0 * fract(Yst) - 1.0;
    float y = aastep(0.0, sqrt(cmyk.z) - length(Yuv));
 
    vec3 rgbscreen = 1.0 - vec3(c, m, y);
    rgbscreen = mix(rgbscreen, black, k + 0.4);

    fragColor = (vec4(rgbscreen * 2.0, 1.0) + vec4(texture(source[0], texCoord).rgba)) / 2.0;
}