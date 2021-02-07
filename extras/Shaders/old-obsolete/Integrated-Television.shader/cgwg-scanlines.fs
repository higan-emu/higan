#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
uniform int phase;

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

// Macros.
#define FIX(c) max(abs(c), 1e-5);
#define PI 3.141592653589

#define ilfac vec2(1.0, floor(sourceSize[0].y / 200.0))
#define one (ilfac / sourceSize[0].xy)
#define mod_factor (texCoord.x * targetSize.x)

// Settings //

  // gamma of simulated CRT
#define CRTgamma 2.4
  // gamma of display monitor (typically 2.2 is correct)
#define monitorgamma 2.2
#define OVERSAMPLE
//#define LINEAR_PROCESSING

// END Settings //

#ifdef LINEAR_PROCESSING
#       define TEX2D(c) pow(texture(source[0], (c)), vec4(CRTgamma))
#else
#       define TEX2D(c) texture(source[0], (c))
#endif

// Calculate the influence of a scanline on the current pixel.
//
// 'distance' is the distance in texture coordinates from the current
// pixel to the scanline in question.
// 'color' is the colour of the scanline at the horizontal location of
// the current pixel.
vec4 scanlineWeights(float distance, vec4 color)
{
  // "wid" controls the width of the scanline beam, for each RGB channel
  // The "weights" lines basically specify the formula that gives
  // you the profile of the beam, i.e. the intensity as
  // a function of distance from the vertical center of the
  // scanline. In this case, it is gaussian if width=2, and
  // becomes nongaussian for larger widths. Ideally this should
  // be normalized so that the integral across the beam is
  // independent of its width. That is, for a narrower beam
  // "weights" should have a higher peak at the center of the
  // scanline than for a wider beam.
#ifdef USEGAUSSIAN
  vec4 wid = 0.3 + 0.1 * pow(color, vec4(3.0));
  vec4 weights = vec4(distance / wid);
  return 0.4 * exp(-weights * weights) / wid;
#else
  vec4 wid = 2.0 + 2.0 * pow(color, vec4(4.0));
  vec4 weights = vec4(distance / 0.3);
  return 1.4 * exp(-pow(weights * inversesqrt(0.5 * wid), wid)) / (0.6 + 0.2 * wid);
#endif
}

void main() {

vec2 xy = texCoord;

  // Of all the pixels that are mapped onto the texel we are
  // currently rendering, which pixel are we currently rendering?
  vec2 ilvec = vec2(0.0,ilfac.y > 1.5 ? mod(float(phase),2.0) : 0.0);
  vec2 ratio_scale = xy * sourceSize[0].xy - vec2(0.5);
  
#ifdef OVERSAMPLE
  float filter_ = fwidth(ratio_scale.y);
#endif
  vec2 uv_ratio = fract(ratio_scale);
  
    // Snap to the center of the underlying texel.
  xy = (floor(ratio_scale) + vec2(0.5)) / sourceSize[0].xy;
  
    // Calculate Lanczos scaling coefficients describing the effect
  // of various neighbour texels in a scanline on the current
  // pixel.
  vec4 coeffs = PI * vec4(1.0 + uv_ratio.x, uv_ratio.x, 1.0 - uv_ratio.x, 2.0 - uv_ratio.x);

  // Prevent division by zero.
  coeffs = FIX(coeffs);

  // Lanczos2 kernel.
  coeffs = 2.0 * sin(coeffs) * sin(coeffs / 2.0) / (coeffs * coeffs);

  // Normalize.
  coeffs /= dot(coeffs, vec4(1.0));
  
   // Calculate the effective colour of the current and next
  // scanlines at the horizontal location of the current pixel,
  // using the Lanczos coefficients above.
  vec4 col  = clamp(mat4(
			 TEX2D(xy + vec2(-one.x, 0.0)),
			 TEX2D(xy),
			 TEX2D(xy + vec2(one.x, 0.0)),
			 TEX2D(xy + vec2(2.0 * one.x, 0.0))) * coeffs,
		    0.0, 1.0);
  vec4 col2 = clamp(mat4(
			 TEX2D(xy + vec2(-one.x, one.y)),
			 TEX2D(xy + vec2(0.0, one.y)),
			 TEX2D(xy + one),
			 TEX2D(xy + vec2(2.0 * one.x, one.y))) * coeffs,
		    0.0, 1.0);

#ifndef LINEAR_PROCESSING
  col  = pow(col , vec4(CRTgamma));
  col2 = pow(col2, vec4(CRTgamma));
#endif

  // Calculate the influence of the current and next scanlines on
  // the current pixel.
  vec4 weights  = scanlineWeights(uv_ratio.y, col);
  vec4 weights2 = scanlineWeights(1.0 - uv_ratio.y, col2);
#ifdef OVERSAMPLE
  uv_ratio.y =uv_ratio.y+1.0/3.0*filter_;
  weights = (weights+scanlineWeights(uv_ratio.y, col))/3.0;
  weights2=(weights2+scanlineWeights(abs(1.0-uv_ratio.y), col2))/3.0;
  uv_ratio.y =uv_ratio.y-2.0/3.0*filter_;
  weights=weights+scanlineWeights(abs(uv_ratio.y), col)/3.0;
  weights2=weights2+scanlineWeights(abs(1.0-uv_ratio.y), col2)/3.0;
#endif
  vec3 mul_res  = (col * weights + col2 * weights2).rgb;
  
    // Convert the image gamma for display on our output device.
  mul_res = pow(mul_res, vec3(1.0 / monitorgamma));

  // Color the texel.
  fragColor = vec4(mul_res, 1.0);
}