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
    inline constexpr float defaultScale = 2.5f;
    /// Maximum number of iterations allowed.
    inline uint32_t maxIterations = 1000;
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~ Fractal Creation ~~~~~~~~~~~~~~~~

    inline constexpr int totalFractalNum = 8;

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

        /* Lambda */
        {"Lambda Exp[z]", "lambda.frag",
            // Description
            "The lambda (λ) fractal repeatedly applies the formula:\n"
            "    λe^z\n"
            "Depending on the starting value of λ, the fractal\n"
            "changes dramatically. Past certain values, the fractal\n"
            "'explodes' in strange and beautiful ways.\n"
            "Use the double-slider to adjust the value of lambda.\n"
            "\n------- Suggestion --------\n"
            "Try setting y = 0, and slowly incrementing x until you reach\n"
            "the value 1/e for x (approx 0.36). Then, zoom in on one of the\n"
            "small tips. Then, push x over that 1/e boundary.\n"
            "Once you cross that boundary. The fractal will 'explode.'\n"
            "This is a good starting point for messing around with λ.",
            // Uniforms
            {{"Lambda (x + yi)", "uLambda", VEC2, {0.f, 1.f}, UniformData(0.803f, 0.189f), 0.025f}}},

        /* Multibrot (integer) */
        {"Multibrot", "multibrot_int.frag",
            "While the Mandelbrot repeatedly applies the formula:\n"
            "    z² + c\n"
            "Multibrots change that exponent to values other than 2.\n"
            "Use the slider to adjust the power.",
            {{"Multibrot Power {Integer}", "uMultiPower", INT, {1, 100}, UniformData(2)}}},

        /* Multibrot (float) */
        {"Multibrot {Float}", "multibrot_float.frag",
            // Description
            "The Mandelbrot repeatedly applies the formula:\n"
            "    z² + c\n"
            "While Multibrots typically use other integer powers\n"
            "other than 2, it is possible to take non-integer powers.\n"
            "Because of floating-point precision errors, the shaders\n"
            "deform in strange ways, but it's still quite interesting!",
            // Uniforms
            {{"Multibrot Power {Float}", "uMultiPower", FLOAT, {1, 100}, UniformData(2.f)}}},

        // Burning Ship Fractal
        {"Burning Ship", "burn_ship.frag"},

        // Burning Ship Fractal Inverse
        {"Burning Ship (Reverse)", "burn_ship_inv.frag",
            "Like the julia set to the mandelbrot set,\n"
            "this fractal is connected when the starting\n"
            "value is within the Burning Ship fractal." },

        // Cursorbrot
        {"Cursorbrot ‹Experimental›", "cursorbrot.frag",
            "Experimental variation on the mandelbrot set. \n"
            "Instead of (z^2 + c), where z starts at 0,\n"
            "z's initial value is tied to cursor position."}
    };

    /// Links names of all fractals for listing in the compile menu.
    inline constexpr const char* fractalNames[totalFractalNum] = {
        fractalInfo[0].name,
        fractalInfo[1].name,
        fractalInfo[2].name,
        fractalInfo[3].name,
        fractalInfo[4].name,
        fractalInfo[5].name,
        fractalInfo[6].name,
        fractalInfo[7].name
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


