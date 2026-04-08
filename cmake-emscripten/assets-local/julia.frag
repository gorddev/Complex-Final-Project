#version 410 core
precision mediump float;

in vec3 vPos;

uniform vec2 uResolution; // Resolution of the display
uniform vec2 uMousePos;   // Position of the mouse (in pixels)
uniform vec2 uStartPos;   // Starting position of the scan (in pixels)
uniform int uIterations;  // How many iteration we will go through
uniform float uScale;      // Current uScale of the scene.

out vec4 color;

float iter_mod = 0.0;     // Color modulator for uIterations
vec2 sMousepos;           // Recalculated mouse position based on the current resolution
vec2 sStartpos;




vec2 complex_sqr(vec2 c1) {
    return vec2(
        c1.x * c1.x - c1.y*c1.y,
        2.0 * c1.x * c1.y
    );
}

vec4 mandelbrot(vec2 frag) {
    vec2 z = vec2(0.0);
    vec2 c = frag;

    int i;
    for(i = 0; i < uIterations; i++) {
        z = complex_sqr(z) + c;
        if (length(z) >= 2) {
            break;
        }
    }

    if (length(z) < 2) {
        return vec4(1.0);
    } else {
        return vec4(float(i) / float(uIterations));
    }
}


vec4 julia(vec2 pixel_coord) {
    vec2 z = pixel_coord;
    vec2 c = sMousepos;

    int i;
    for(i = 0; i < uIterations; i++) {
        z = complex_sqr(z) + c;
        if (dot(z, z) > 4.0) {
            break;
        }
    }
    if (i == uIterations) {
        return vec4(0.2, 0.2, 0.2, 1.0);
    } else {
        float t = float(i) / 30.0;
        return vec4(t, t, t, 1.0);
    }
}

vec2 renormalize(vec2 pos) {
    return (pos - 0.5* uResolution.xy)/(length(uResolution.xy));
}

vec2 rescale(vec2 position) {
    return uScale*(position - 0.5* uResolution.xy)/(length(uResolution.xy));
}

void main() {
    sStartpos = renormalize(uStartPos);
    sMousepos = rescale(uMousePos) - sStartpos;

    vec2 pix = rescale(gl_FragCoord.xy) - sStartpos;
    iter_mod = 30.0/float(uIterations);
    color = julia(rescale(gl_FragCoord.xy) *3.0);

    vec4 mandy = mandelbrot(pix);
    color.y = color.y + mandy.y/2.0;


}
