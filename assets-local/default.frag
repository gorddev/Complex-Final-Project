#version 410 core
precision mediump float;

#define MAX_COLORS 8

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

// returns the final palette
vec4 getPalette(int iterations, vec2 z) {
    return vec4(palette(smooth_iter(iterations, z) / float(uIterations)), 1.0);
}

// ............................. //

// gives the square of a complex number
vec2 complex_sqr(vec2 c1) {
    return vec2(
    c1.x * c1.x - c1.y*c1.y,
    2.0 * c1.x * c1.y
    );
}

vec2 complex_mul(vec2 a, vec2 b) {
    return vec2(
    a.x * b.x - a.y * b.y,
    a.x * b.y + a.y * b.x
    );
}

vec2 complex_pow(vec2 z, float power) {
    float r = pow(length(z), power);
    return r * vec2(cos(z.y*power), sin(z.y*power));
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