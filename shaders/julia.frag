#version 410 core
precision lowp float;

#define MAX_COLORS 8
#define MULTI_SAMPLE(func, output) \
    float sampleRange = 0.5; \
    vec2 pix0 = toFractalSpace(gl_FragCoord.xy); \
    vec2 pix1 = toFractalSpace(gl_FragCoord.xy + vec2(sampleRange)); \
    vec2 pix2 = toFractalSpace(gl_FragCoord.xy + vec2(-sampleRange)); \
    vec2 pix3 = toFractalSpace(gl_FragCoord.xy + vec2(-sampleRange, sampleRange)); \
    vec2 pix4 = toFractalSpace(gl_FragCoord.xy + vec2(sampleRange, -sampleRange)); \
    output = 0.2*(func(pix0) + func(pix1) + func(pix2) + func(pix3) + func(pix4))

// ~~~~~~~~~~~ Uniforms ~~~~~~~~~~~~//

in vec3 vPos;

uniform vec2 uResolution; // Resolution of the display
uniform vec2 uMousePos;   // Position of the mouse (in pixels)
uniform vec2 uCenterPos;   // Starting position of the scan (in pixels)
uniform vec4 uWindowDimensions; // Dimensions of the internal window.
uniform int uIterations;  // How many iteration we will go through
uniform float uScale;     // Current uScale of the scene.

uniform int uColorCount;
uniform vec3 uColors[MAX_COLORS];

out vec4 color;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //


// .......  Standard Functions ........ //
// .................................... //


// sets up the pallete for coloring
vec3 palette(float t) {
    if (uColorCount == 0) return vec3(0.0);
    if (uColorCount == 1) return uColors[0];

    float scaled = t * float(uColorCount - 1);
    int i = int(floor(scaled));
    float f = fract(scaled);

    i = clamp(i, 0, uColorCount - 2);

    return mix(uColors[i], uColors[i + 1], f);
}

// smooth decay and iteratinos
float smooth_iter(int i, vec2 z) {
    float log_zn = log(dot(z, z)) / 2.0;
    float nu = log(log_zn / log(2.0)) / log(2.0);
    return float(i) + 1.0 - nu;
}

// ............................. //

// gives the square of a complex number
vec2 complex_sqr(vec2 c1) {
    return vec2(
    c1.x * c1.x - c1.y*c1.y,
    2.0 * c1.x * c1.y
    );
}


vec2 normToNDC(vec2 norm) {
    return (norm - vec2(0.5))*2.0;
}

vec2 toFractalSpace(vec2 fragCoord) {
    // global normalized coords
    vec2 uv = fragCoord / uResolution;
    uv -= uWindowDimensions.xy;
    vec2 p = uv - vec2(0.5);
    p.x *= uResolution.x / uResolution.y;

    return uCenterPos + p * uScale;
}

// .................................... //
// .................................... //
// .................................... //
// .................................... //

vec2 sStartPos;

vec4 julia(vec2 pixel_coord) {
    vec2 z = pixel_coord;
    vec2 c = uMousePos;

    int i;
    for(i = 0; i < uIterations; i++) {
        float x2 = z.x * z.x;
        float y2 = z.y * z.y;
        z.y = 2.0 * z.x * z.y + c.y;
        z.x = x2 - y2 + c.x;
        if (dot(z, z) > 4.0) {
            break;
        }
    }
    if (i == uIterations) {
        return vec4(0.2, 0.2, 0.2, 1.0);
    } else {
        float t = smooth_iter(i, z) / float(uIterations);
        return vec4(palette(t), 1.0);
    }
}

vec3 draw_mouse_circle(vec2 pixel_coord, float radius) {

    float thickness = 0.005; // ring thickness

    float d = length(pixel_coord - uMousePos);

    float ring = smoothstep(radius, radius - thickness, d)
    - smoothstep(radius + thickness, radius, d);

    return vec3(1.0) * ring;
}

void main() {
    vec2 pix = toFractalSpace(gl_FragCoord.xy);
    color = julia(pix) + vec4(draw_mouse_circle(pix, 0.004),1.0);
}
