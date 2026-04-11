#pragma once
#include <array>
#include <string>

#include "fractals/FractalInfo.hpp"

/* Created by Gordie Novak on 4/7/26.
 * Purpose: 
 * Contains a bunch of constants relevant to fractals */


namespace gan {
    using fractal_id = int;
}

namespace gan::fractal {
    inline constexpr uint32_t maxFractalViews = 4;
    inline constexpr float defaultScale = 2.f;
    /// Maximum number of iterations allowed.
    inline uint32_t maxIterations = 1000;
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~ Fractal Creation ~~~~~~~~~~~~~~~~

    inline constexpr int totalFractalNum = 6;

    /** @brief List of all fractals and how to compile them.
     * @details To create a fractal, you must follow the constructor for a @code FractalInfo@endcode object:
     *
     * @code FractalInfo@endcode :
     * @param Name Name of the fractal
     * @param FragShader Relative path of the fragment shader from the assets folder
     * @param UniformList List of all additional uniforms that bind to the shader. Defaults to empty. Create a uniform with:
     * @code {Name, uniformHandleInShader, type, {min_val, max_val}, UniformData(defaults)}@endcode. Note that the UniformData
     * constructor is type-sensitive, so it matters if you put in floats vs. ints.
     * @param VertexShader Relative path to the vertex shader. Defaults to @code std.vert@endcode.
     */
    inline constexpr FractalInfo fractalInfo[totalFractalNum] = {
        // Mandelbrot Set
        {"Mandelbrot", "mandelbrot.frag"},

        // Julia Set
        {"Julia Set", "julia.frag"},

        // Lambda Fractal
        {"Lambda Exp[z]", "lambda.frag", {
            {"Lambda (x + yi)", "uLambda", VEC2, {-1.f, 1.f}, UniformData(0.803f, 0.189f), 0.025f}}},

        // multibrot (integer)
        {"Multibrot {Integer}", "multibrot_int.frag", {
            {"Multibrot Power {Integer}", "uMultiPower", INT, {1, 100}, UniformData(2)}}},

        // multibrot (float)
        {"Multibrot {Float}", "multibrot_float.frag", {
                {"Multibrot Power {Float}", "uMultiPower", FLOAT, {1, 100}, UniformData(2.f)}}},

        // Burning Ship Fractal
        {"Burning Ship", "burn_ship.frag"},
    };

    /// Links names of all fractals for listing in the compile menu.
    inline constexpr const char* fractalNames[totalFractalNum] = {
        fractalInfo[0].name,
        fractalInfo[1].name,
        fractalInfo[2].name,
        fractalInfo[3].name,
        fractalInfo[4].name,
        fractalInfo[5].name
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~ View Locations ~~~~~~~~~~~~~~~~~
    /// Different locations fractal panels can be placed in the window
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
    inline constexpr uint32_t defaultNumColors = 6;
    /// Default color values for newly compiled fractals.
    inline constexpr std::array<vec3, max_colors> default_colors = {
        vec3{0.3, 0.5, 0.7},
        vec3{0.5, 0.3, 0.7},
        vec3{0.7, 0.5, 0.3},
        vec3{0.3, 0.3, 0.7},
        vec3{0.3, 0.7, 0.3},
        vec3{0.7, 0.3, 0.3},
        vec3{0.5, 0.5, 0.5},
        vec3{0.8, 0.6, 0.1},
    };
}


