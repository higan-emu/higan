/*    caligari's scanlines

    Copyright (C) 2011 caligari

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation; either version 2 of the License, or (at your option)
    any later version.

    (caligari gave their consent to have this shader distributed under the GPL
    in this message:

        http://board.byuu.org/viewtopic.php?p=36219#p36219

        "As I said to Hyllian by PM, I'm fine with the GPL (not really a big
        deal...)"
*/

#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
in float modulo;
in vec2 output_coords;


in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

        // 0.5 = the spot stays inside the original pixel
        // 1.0 = the spot bleeds up to the center of next pixel
        #define SPOT_WIDTH  0.9
        #define SPOT_HEIGHT 0.65

        // Used to counteract the desaturation effect of weighting.
        #define COLOR_BOOST 2.45

        // Different way to handle RGB phosphors.
        // #define RGB_BAR
         #define RGB_TRIAD
        // #define MG_BAR

        // Constants used with gamma correction.
        #define InputGamma 2.4
        #define OutputGamma 3.2

        // Uncomment one of these to choose a gamma correction method.
        // If none are uncommented, no gamma correction is done.
         #define REAL_GAMMA
        // #define FAKE_GAMMA
        // #define FAKER_GAMMA

        #ifdef REAL_GAMMA
        #define GAMMA_IN(color)     pow(color, vec4(InputGamma))
        #define GAMMA_OUT(color)    pow(color, vec4(1.0 / OutputGamma))

        #elif defined FAKE_GAMMA
        /*
         * Approximations:
         * for 1<g<2 : x^g ~ ax + bx^2
         *             where   a=6/(g+1)-2  and b=1-a
         * for 2<g<3 : x^g ~ ax^2 + bx^3
         *             where   a=12/(g+1)-3 and b=1-a
         * for 1<g<2 : x^(1/g) ~ (sqrt(a^2+4bx)-a)
         *             where   a=6/(g+1)-2  and b=1-a
         * for 2<g<3 : x^(1/g) ~ (a sqrt(x) + b sqrt(sqrt(x)))
         *             where   a = 6 - 15g / 2(g+1)  and b = 1-a
         */
        vec4 A_IN  = vec4(12.0/(InputGamma+1.0)-3.0);
        vec4 B_IN  = vec4(1.0) - A_IN;
        vec4 A_OUT = vec4(6.0 - 15.0 * OutputGamma / 2.0 / (OutputGamma+1.0));
        vec4 B_OUT = vec4(1.0) - A_OUT;
        #define GAMMA_IN(color)  ((A_IN + B_IN * color) * color * color)
        #define GAMMA_OUT(color) (A_OUT * sqrt(color) + B_OUT * sqrt(sqrt(color)))

        #elif defined FAKER_GAMMA
        vec4 A_IN = vec4( 6.0/( InputGamma/OutputGamma + 1.0 ) - 2.0 );
        vec4 B_IN = vec4(1.0) - A_IN;
        #define GAMMA_IN(color)     ( (A_IN + B_IN * color) * color )
        #define GAMMA_OUT(color)    color

        #else // No gamma correction
        #define GAMMA_IN(color) color
        #define GAMMA_OUT(color) color
        #endif

        #define TEX2D(coords)   GAMMA_IN( texture(source[0], coords) )

        // Macro for weights computing
        #define WEIGHT(w) \
                if(w>1.0) w=1.0; \
                w = 1.0 - w * w; \
                w = w * w;

        vec2 onex = vec2( 1.0/sourceSize[0].x, 0.0 );
        vec2 oney = vec2( 0.0, 1.0/sourceSize[0].y );

        void main(void)
        {
                vec2 coords = ( texCoord.xy * sourceSize[0].xy );
                vec2 pixel_center = floor( coords ) + vec2(0.5);
                vec2 texture_coords = pixel_center / sourceSize[0].xy;

                vec4 color = TEX2D( texture_coords );

                float dx = coords.x - pixel_center.x;

                float h_weight_00 = dx / SPOT_WIDTH;
                WEIGHT(h_weight_00);

                color *= vec4( h_weight_00  );

                // get closest horizontal neighbour to blend
                vec2 coords01;
                if (dx>0.0) {
                        coords01 = onex;
                        dx = 1.0 - dx;
                } else {
                        coords01 = -onex;
                        dx = 1.0 + dx;
                }
                vec4 colorNB = TEX2D( texture_coords + coords01 );

                float h_weight_01 = dx / SPOT_WIDTH;
                WEIGHT( h_weight_01 );

                color = color + colorNB * vec4( h_weight_01 );

                //////////////////////////////////////////////////////
                // Vertical Blending
                float dy = coords.y - pixel_center.y;
                float v_weight_00 = dy / SPOT_HEIGHT;
                WEIGHT(v_weight_00);
                color *= vec4( v_weight_00 );

                // get closest vertical neighbour to blend
                vec2 coords10;
                if (dy>0.0) {
                        coords10 = oney;
                        dy = 1.0 - dy;
                } else {
                        coords10 = -oney;
                        dy = 1.0 + dy;
                }
                colorNB = TEX2D( texture_coords + coords10 );

                float v_weight_10 = dy / SPOT_HEIGHT;
                WEIGHT( v_weight_10 );

                color = color + colorNB * vec4( v_weight_10 * h_weight_00 );

                colorNB = TEX2D(  texture_coords + coords01 + coords10 );

                color = color + colorNB * vec4( v_weight_10 * h_weight_01 );

                color *= vec4( COLOR_BOOST );

        #ifdef RGB_BAR
                vec2 output_coords = floor( texCoord.xy * targetSize.xy);

                float modulo = mod(output_coords.x,3.0);
                if ( modulo == 0.0 )
                    color = color * vec4(1.4,0.5,0.5,0.0);
                else if ( modulo == 1.0 )
                    color = color * vec4(0.5,1.4,0.5,0.0);
                else
                    color = color * vec4(0.5,0.5,1.4,0.0);
        #endif

        #ifdef RGB_TRIAD
                vec2 output_coords = floor(texCoord.xy * targetSize.xy / sourceSize[0].xy * sourceSize[0].xy);

                float modulo = mod(output_coords.x,2.0);

                if ( modulo < 1.0 )
                    modulo = mod(output_coords.y,6.0);
                else
                    modulo = mod(output_coords.y + 3.0, 6.0);

                if ( modulo < 2.0 )
                    color = color * vec4(1.0,0.0,0.0,0.0);
                else if ( modulo < 4.0 )
                    color = color * vec4(0.0,1.0,0.0,0.0);
                else
                    color = color * vec4(0.0,0.0,1.0,0.0);
        #endif

        #ifdef MG_BAR
                vec2 output_coords = floor(texCoord.xy * targetSize.xy);

                float modulo = mod(output_coords.x,2.0);
                if ( modulo == 0.0 )
                    color = color * vec4(1.0,0.1,1.0,0.0);
                else
                    color = color * vec4(0.1,1.0,0.1,0.0);
        #endif

                fragColor = clamp(GAMMA_OUT(color), 0.0, 1.0);
        }