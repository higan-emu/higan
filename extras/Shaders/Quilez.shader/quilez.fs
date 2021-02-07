#version 150

/*
	Fragment shader based on "Improved texture interpolation" by Iñigo Quílez
	Original description: http://www.iquilezles.org/www/articles/texture/texture.htm

*/

uniform sampler2D source[];
uniform vec4 sourceSize[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
	vec2 p = texCoord.xy;

	p = p * sourceSize[0].xy + vec2(0.5, 0.5);

	vec2 i = floor(p);
	vec2 f = p - i;
	f = f * f * f * (f * (f * 6.0 - vec2(15.0, 15.0)) + vec2(10.0, 10.0));
	p = i + f;

	p = (p - vec2(0.5, 0.5)) * sourceSize[0].zw;

	// final sum and weight normalization
   fragColor = vec4(texture(source[0], p));
}
