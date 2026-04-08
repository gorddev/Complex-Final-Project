#pragma once
#include <array>
#include <string>

/* Created by Gordie Novak on 4/7/26.
 * Purpose: 
 * Contains a bunch of constants relevant to fractals */


namespace gan {
    using fractal_id = int;
}

namespace gan::fractal {
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~ Fractal Creation ~~~~~~~~~~~~~~~~
    inline constexpr uint32_t maxFractalViews = 4;

    /// Contains all the information to create a fractal.
    struct FractalInfo {
        const std::string name;
        const std::string fragShader;
        const std::string vertShader = "std.vert";
    };

    inline constexpr int totalFractalNum = 4;
    inline constexpr const char* fractalNames[] = {
        "Mandelbrot",
        "Julia Set",
        "Burning Ship",
        "Popcorn",
    };

    inline const FractalInfo fractalInfo[] = {
        {"Mandelbrot", "mandelbrot.frag"},
        {"Julia Set", "julia.frag"},
        {"Burning Ship", "burn_ship.frag"},
        {"Popcorn", "popcorn.frag", "popcorn.vert"}
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
    inline constexpr uint32_t default_num_colors = 4;
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


