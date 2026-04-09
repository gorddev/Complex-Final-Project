#pragma once
#include <array>
#include <string>

#include "FractalInfo.hpp"

/* Created by Gordie Novak on 4/7/26.
 * Purpose: 
 * Contains a bunch of constants relevant to fractals */


namespace gan {
    using fractal_id = int;
}

namespace gan::fractal {
    inline constexpr uint32_t maxFractalViews = 4;
    inline constexpr float defaultScale = 2.f;
    inline uint32_t maxIterations = 1000;
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~ Fractal Creation ~~~~~~~~~~~~~~~~

    inline constexpr int totalFractalNum = 7;
    inline constexpr FractalInfo fractalInfo[totalFractalNum] = {
        // Mandelbrot Set
        {"Mandelbrot", "mandelbrot.frag"},
        // Julia Set
        {"Julia Set", "julia.frag"},
        // Lambda Fractal
        {"Lambda Exp[z]", "lambda.frag", {
            {"Lambda (x + yi)", "uLambda", VEC2, {-1.f, 1.f}, UniformData(0.803f, 0.189f)}}},
        // multibrot (integer)
        {"Multibrot {Integer}", "multibrot_int.frag", {
            {"Multibrot Power {Integer}", "uMultiPower", INT, {1, 100}, UniformData(2)}}},
        // multibrot (float)
        {"Multibrot {Float}", "multibrot_float.frag", {
                {"Multibrot Power {Float}", "uMultiPower", FLOAT, {1, 100}, UniformData(2.f)}}},
        // Burning Ship Fractal
        {"Burning Ship", "burn_ship.frag"},
        // Popcorn Fractal
        {"Popcorn", "popcorn.frag", {}, "popcorn.vert"}
    };

    inline constexpr const char* fractalNames[totalFractalNum] = {
        fractalInfo[0].name,
        fractalInfo[1].name,
        fractalInfo[2].name,
        fractalInfo[3].name,
        fractalInfo[4].name,
        fractalInfo[5].name,
        fractalInfo[6].name,
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~ View Locations ~~~~~~~~~~~~~~~~~
    enum class ViewPosition {
        FULLSCREEN,
        LEFT,
        RIGHT,
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT,
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~ Coloring ~~~~~~~~~~~~~~~~~~~~
    inline constexpr uint32_t max_colors = 8;
    inline constexpr uint32_t min_colors = 2;
    inline constexpr uint32_t default_num_colors = 6;
    inline constexpr std::array default_colors = {
        vec3{0.3, 0.5, 0.7},
        vec3{0.5, 0.3, 0.7},
        vec3{0.7, 0.5, 0.3},
        vec3{0.3, 0.3, 0.7},
        vec3{0.3, 0.7, 0.3},
        vec3{0.7, 0.3, 0.3},
        vec3{0.5, 0.5, 0.5},
        vec3{0.8, 0.6, 0.1}
    };
}


