#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];

in Vertex {
  vec2 texCoord;
  vec2 one;
  float mod_factor;
};

out vec4 fragColor;

        uniform vec4 targetSize;
        uniform vec2 outputSize;

        // Enable screen curvature.
        #define CURVATURE

        // Controls the intensity of the barrel distortion used to emulate the
        // curvature of a CRT. 0.0 is perfectly flat, 1.0 is annoyingly
        // distorted, higher values are increasingly ridiculous.
        #define distortion 0.08

        // Simulate a CRT gamma of 2.4.
        #define inputGamma  2.4

        // Compensate for the standard sRGB gamma of 2.2.
        #define outputGamma 2.2

        // Macros.
        #define TEX2D(c) pow(texture2D(source[0], (c)), vec4(inputGamma))
        #define PI 3.141592653589

        // Apply radial distortion to the given coordinate.
        vec2 radialDistortion(vec2 coord)
        {
                vec2 cc = coord - 0.5;
                float dist = dot(cc, cc) * distortion;
                return (coord + cc * (1.0 + dist) * dist);
        }

        // Calculate the influence of a scanline on the current pixel.
        //
        // 'distance' is the distance in texture coordinates from the current
        // pixel to the scanline in question.
        // 'color' is the colour of the scanline at the horizontal location of
        // the current pixel.
        vec4 scanlineWeights(float distance, vec4 color)
        {
                // The "width" of the scanline beam is set as 2*(1 + x^4) for
                // each RGB channel.
                vec4 wid = 2.0 + 2.0 * pow(color, vec4(4.0));

                // The "weights" lines basically specify the formula that gives
                // you the profile of the beam, i.e. the intensity as
                // a function of distance from the vertical center of the
                // scanline. In this case, it is gaussian if width=2, and
                // becomes nongaussian for larger widths. Ideally this should
                // be normalized so that the integral across the beam is
                // independent of its width. That is, for a narrower beam
                // "weights" should have a higher peak at the center of the
                // scanline than for a wider beam.
                vec4 weights = vec4(distance / 0.3);
                return 1.4 * exp(-pow(weights * inversesqrt(0.5 * wid), wid)) / (0.6 + 0.2 * wid);
        }

void main() {
                 // Here's a helpful diagram to keep in mind while trying to
                // understand the code:
                //
                //  |      |      |      |      |
                // -------------------------------
                //  |      |      |      |      |
                //  |  01  |  11  |  21  |  31  | <-- current scanline
                //  |      | @    |      |      |
                // -------------------------------
                //  |      |      |      |      |
                //  |  02  |  12  |  22  |  32  | <-- next scanline
                //  |      |      |      |      |
                // -------------------------------
                //  |      |      |      |      |
                //
                // Each character-cell represents a pixel on the output
                // surface, "@" represents the current pixel (always somewhere
                // in the bottom half of the current scan-line, or the top-half
                // of the next scanline). The grid of lines represents the
                // edges of the texels of the underlying texture.

                // Texture coordinates of the texel containing the active pixel.
        #ifdef CURVATURE
                vec2 xy = radialDistortion(texCoord);
        #else
                vec2 xy = texCoord;
        #endif

                // Of all the pixels that are mapped onto the texel we are
                // currently rendering, which pixel are we currently rendering?
                vec2 ratio_scale = xy * sourceSize[0].xy - vec2(0.5);
                vec2 uv_ratio = fract(ratio_scale);

                // Snap to the center of the underlying texel.
                xy.y = (floor(ratio_scale.y) + 0.5) / sourceSize[0].y;

                // Calculate the effective colour of the current and next
                // scanlines at the horizontal location of the current pixel.
                vec4 col  = TEX2D(xy);
                vec4 col2 = TEX2D(xy + vec2(0.0, one.y));

                // Calculate the influence of the current and next scanlines on
                // the current pixel.
                vec4 weights  = scanlineWeights(uv_ratio.y, col);
                vec4 weights2 = scanlineWeights(1.0 - uv_ratio.y, col2);
                vec3 mul_res  = (col * weights + col2 * weights2).rgb;

                // dot-mask emulation:
                // Output pixels are alternately tinted green and magenta.
                vec3 dotMaskWeights = mix(
                        vec3(1.0, 0.7, 1.0),
                        vec3(0.7, 1.0, 0.7),
                        floor(mod(mod_factor, 2.0))
                    );

                mul_res *= dotMaskWeights;

                fragColor = vec4(pow(mul_res, vec3(1.0 / outputGamma)), 1.0);
}
