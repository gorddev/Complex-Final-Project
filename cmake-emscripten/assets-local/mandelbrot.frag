#version 410 core
precision mediump float;

#define MAX_COLORS 8

// ~~~~~~~~~~~ Uniforms ~~~~~~~~~~~~//

in vec3 vPos;

uniform vec2 uResolution; // Resolution of the display
uniform vec2 uMousePos;   // Position of the mouse (in pixels)
uniform vec2 uStartPos;   // Starting position of the scan (in pixels)
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

// turns a pixel position into a normalized position
vec2 renormalize(vec2 pos) {
    return (pos - 0.5* uResolution.xy)/(length(uResolution.xy));
}

// turns a pixel position into a scaled normalized position.
vec2 rescale(vec2 position) {
    return uScale*(position - 0.5* uResolution.xy)/(length(uResolution.xy));
}

// .................................... //
// .................................... //
// .................................... //
// .................................... //


vec4 mandelbrot(vec2 frag) {
    vec2 z = vec2(0.0);
    vec2 c = frag;

    int i;
    for(i = 0; i < uIterations; i++) {
        z = complex_sqr(z) + c;
        if (length(z) >= 2.0) {
            break;
        }
    }

    if (length(z) < 2.0) {
        return vec4(0.0);
    } else {
        float t = smooth_iter(i, z) / float(uIterations);
        return vec4(palette(t), 1.0);
    }
}


void main() {
    vec2 sStartpos = renormalize(uStartPos);

    vec2 pix = rescale(gl_FragCoord.xy) - sStartpos;
    color = mandelbrot(pix);
}