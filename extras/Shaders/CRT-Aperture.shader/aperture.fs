#version 150

/*
    CRT Shader by EasyMode
    License: GPL
*/

#define SHARPNESS_IMAGE 1.0
#define SHARPNESS_EDGES 3.0
#define GLOW_WIDTH 0.5
#define GLOW_HEIGHT 0.5
#define GLOW_HALATION 0.1
#define GLOW_DIFFUSION 0.05
#define MASK_COLORS 2.0
#define MASK_STRENGTH 0.3
#define MASK_SIZE 1.0
#define SCANLINE_SIZE_MIN 0.5
#define SCANLINE_SIZE_MAX 1.5
#define GAMMA_INPUT 2.4
#define GAMMA_OUTPUT 2.4
#define BRIGHTNESS 1.5

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

in Vertex {
   vec2 texCoord;
};

out vec4 fragColor;

#define FIX(c) max(abs(c), 1e-5)
#define PI 3.141592653589
#define TEX2D(c) pow(texture(source[0], c).rgb, vec3(GAMMA_INPUT))
#define saturate(c) clamp(c, 0.0, 1.0)

mat3 get_color_matrix(sampler2D tex, vec2 co, vec2 dx)
{
    return mat3(TEX2D(co - dx), TEX2D(co), TEX2D(co + dx));
}

vec3 blur(mat3 m, float dist, float rad)
{
    vec3 x = vec3(dist - 1.0, dist, dist + 1.0) / rad;
    vec3 w = exp2(x * x * -1.0);

    return (m[0] * w.x + m[1] * w.y + m[2] * w.z) / (w.x + w.y + w.z);
}

vec3 filter_gaussian(sampler2D tex, vec2 co, vec2 tex_size)
{
    vec2 dx = vec2(1.0 / tex_size.x, 0.0);
    vec2 dy = vec2(0.0, 1.0 / tex_size.y);
    vec2 pix_co = co * tex_size;
    vec2 tex_co = (floor(pix_co) + 0.5) / tex_size;
    vec2 dist = (fract(pix_co) - 0.5) * -1.0;

    mat3 line0 = get_color_matrix(tex, tex_co - dy, dx);
    mat3 line1 = get_color_matrix(tex, tex_co, dx);
    mat3 line2 = get_color_matrix(tex, tex_co + dy, dx);
    mat3 column = mat3(blur(line0, dist.x, GLOW_WIDTH),
                               blur(line1, dist.x, GLOW_WIDTH),
                               blur(line2, dist.x, GLOW_WIDTH));

    return blur(column, dist.y, GLOW_HEIGHT);
}

vec3 filter_lanczos(sampler2D tex, vec2 co, vec2 tex_size, float sharp)
{
    tex_size.x *= sharp;

    vec2 dx = vec2(1.0 / tex_size.x, 0.0);
    vec2 pix_co = co * tex_size - vec2(0.5, 0.0);
    vec2 tex_co = (floor(pix_co) + vec2(0.5, 0.001)) / tex_size;
    vec2 dist = fract(pix_co);
    vec4 coef = PI * vec4(dist.x + 1.0, dist.x, dist.x - 1.0, dist.x - 2.0);

    coef = FIX(coef);
    coef = 2.0 * sin(coef) * sin(coef / 2.0) / (coef * coef);
    coef /= dot(coef, vec4(1.0));

    vec4 col1 = vec4(TEX2D(tex_co), 1.0);
    vec4 col2 = vec4(TEX2D(tex_co + dx), 1.0);

    return (mat4(col1, col1, col2, col2) * coef).rgb;
}

vec3 get_scanline_weight(float x, vec3 col)
{
    vec3 beam = mix(vec3(SCANLINE_SIZE_MIN), vec3(SCANLINE_SIZE_MAX), col);
    vec3 x_mul = 2.0 / beam;
    vec3 x_offset = x_mul * 0.5;

    return smoothstep(0.0, 1.0, 1.0 - abs(x * x_mul - x_offset)) * x_offset;
}

vec3 get_mask_weight(float x)
{
    float i = mod(floor(x * targetSize.x * sourceSize[0].x / (sourceSize[0].x * MASK_SIZE)), MASK_COLORS);

    if (i == 0.0) return mix(vec3(1.0, 0.0, 1.0), vec3(1.0, 0.0, 0.0), MASK_COLORS - 2.0);
    else if (i == 1.0) return vec3(0.0, 1.0, 0.0);
    else return vec3(0.0, 0.0, 1.0);
}

void main() {
    vec3 col_glow = filter_gaussian(source[0], texCoord, sourceSize[0].xy);
    vec3 col_soft = filter_lanczos(source[0], texCoord, sourceSize[0].xy, SHARPNESS_IMAGE);
    vec3 col_sharp = filter_lanczos(source[0], texCoord, sourceSize[0].xy, SHARPNESS_EDGES);
    vec3 col = sqrt(col_sharp * col_soft);

    col *= get_scanline_weight(fract(texCoord.y * sourceSize[0].y), col_soft);
    col_glow = saturate(col_glow - col);
    col += col_glow * col_glow * GLOW_HALATION;
    col = mix(col, col * get_mask_weight(texCoord.x) * MASK_COLORS, MASK_STRENGTH);
    col += col_glow * GLOW_DIFFUSION;
    col = pow(col * BRIGHTNESS, vec3(1.0 / GAMMA_OUTPUT));
fragColor = vec4(col, 1.0);
}