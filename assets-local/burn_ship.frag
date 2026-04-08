#version 410 core
precision mediump float;

in vec3 vPos;

uniform vec2 uResolution;
uniform vec2 uMousePos;
uniform vec2 uStartPos;
uniform int uIterations;
uniform float uScale;

out vec4 color;

float iter_mod = 0.0;
vec2 sMousepos;
vec2 sStartpos;


// Burning Ship iteration (key difference)
vec2 burning_ship_step(vec2 z) {
    z = vec2(abs(z.x), abs(z.y));
    return vec2(
    z.x * z.x - z.y * z.y,
    2.0 * z.x * z.y
    );
}


// 🔥 Burning Ship Julia-style (mouse controls c)
vec4 burning_ship(vec2 pixel_coord) {
    vec2 z = pixel_coord;
    vec2 c = sMousepos;

    int i;
    for(i = 0; i < uIterations; i++) {
        z = burning_ship_step(z) + c;

        if (dot(z, z) > 4.0) {
            break;
        }
    }

    if (i == uIterations) {
        return vec4(0.1, 0.1, 0.1, 1.0);
    } else {
        float t = float(i) / float(uIterations);
        return vec4(t, t * 0.6, t * 0.2, 1.0); // warmer palette
    }
}


// (Optional) Burning Ship Mandelbrot-style
vec4 burning_ship_mandelbrot(vec2 frag) {
    vec2 z = vec2(0.0);
    vec2 c = frag;

    int i;
    for(i = 0; i < uIterations; i++) {
        z = burning_ship_step(z) + c;

        if (dot(z, z) > 4.0) {
            break;
        }
    }

    if (i == uIterations) {
        return vec4(1.0);
    } else {
        return vec4(float(i) / float(uIterations));
    }
}


vec2 renormalize(vec2 pos) {
    return (pos - 0.5 * uResolution.xy) / length(uResolution.xy);
}

vec2 rescale(vec2 position) {
    return uScale * (position - 0.5 * uResolution.xy) / length(uResolution.xy);
}


void main() {
    sStartpos = renormalize(uStartPos);
    sMousepos = rescale(uMousePos) - sStartpos;

    vec2 pix = rescale(gl_FragCoord.xy) - sStartpos;

    iter_mod = 30.0 / float(uIterations);

    // 🔥 Swap this line to switch fractals:
    color = burning_ship(pix);

    // Optional hybrid like your Julia + Mandelbrot combo
    vec4 mandy = burning_ship_mandelbrot(pix);
    color.y += mandy.y * 0.5;
}