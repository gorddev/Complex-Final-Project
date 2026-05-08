#pragma once
#include <array>
#include "GPUFractalInfo.hpp"
#include "core/types/vec.hpp"

/* Created by Gordie Novak on 4/7/26.
 * Purpose: 
 * Contains a bunch of constants relevant to fractals */

namespace gan::gpu_fractal {
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~ Fractal Creation ~~~~~~~~~~~~~~~~

    inline constexpr int totalFractalNum = 9;

    /** @brief List of all fractals and how to compile them.
     * @details To create a fractal, you must follow the constructor for a @code FractalInfo@endcode object:
     *
     * @code FractalInfo@endcode :
     * @param Name Name of the fractal
     * @param FragShader Relative path of the fragment shader from the assets folder
     * @param UniformList{} List of all additional uniforms that bind to the shader. Defaults to empty. Create a uniform with:
     * @code {Name, uniformHandleInShader, type, {min_val, max_val}, UniformData(defaults)}@endcode. Note that the UniformData
     * constructor is type-sensitive, so it matters if you put in floats vs. ints.
     * @param VertexShader{} Relative path to the vertex shader. Defaults to @code std.vert@endcode.
     */
    inline constexpr GPUFractalInfo fractalInfo[totalFractalNum] = {
        // Mandelbrot Set
        {"Mandelbrot", "gpu_shaders/mandelbrot.frag"},

        // Julia Set
        {"Julia Set", "gpu_shaders/julia.frag"},

        /* Lambda */
        {"Lambda Exp[z]", "gpu_shaders/lambda.frag",
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
            {
                    {"Lambda (x + yi)", "uLambda", VEC2, {0.1f, 1.f}, UniformData(0.803f, 0.0f), 0.025f},
                    {"Escape Velocity²", "uEscapeVelocity", FLOAT, {30.f, 1000.f}, UniformData(100.f)}
            },
        },

        /* Multibrot (integer) */
        {"Multibrot", "gpu_shaders/multibrot_int.frag",
            "While the Mandelbrot repeatedly applies the formula:\n"
            "    z² + c\n"
            "Multibrots change that exponent to values other than 2.\n"
            "Use the slider to adjust the power.",
            {{"Multibrot Power {Integer}", "uMultiPower", INT, {1, 100}, UniformData(2)}}},

        /* Multibrot (float) */
        {"Multibrot {Float}", "gpu_shaders/multibrot_float.frag",
            // Description
            "The Mandelbrot repeatedly applies the formula:\n"
            "    z² + c\n"
            "While Multibrots typically use other integer powers\n"
            "other than 2, it is possible to take non-integer powers.\n"
            "Because of floating-point precision errors, the shaders\n"
            "deform in strange ways, but it's still quite interesting!",
            // Uniforms
            {{"Multibrot Power {Float}", "uMultiPower", FLOAT, {1, 100}, UniformData(2.f)}}},

        /* Burning Ship Fractal */
        {"Burning Ship", "gpu_shaders/burn_ship.frag"},

        /* Burning Ship Fractal Inverse */
        {"Burning Ship (Reverse)", "gpu_shaders/burn_ship_inv.frag",
            "Like the julia set to the mandelbrot set,\n"
            "this fractal is connected when the starting\n"
            "value is within the Burning Ship fractal." },

        /* Cursorbrot */
        {"Cursorbrot ‹Experimental›", "gpu_shaders/cursorbrot.frag",
            // Description
            "Experimental variation on the mandelbrot set. \n"
            "Instead of (z^2 + c), where z starts at 0,\n"
            "z's initial value is tied to cursor position."},

        /* Nova Fractal */
        {"Nova Fractal", "gpu_shaders/nova.frag",
            // Description
            "I dunno found this one online"}
    };

    /// Links names of all fractals for listing in the compile menu.
    static consteval auto getFractalNames() {
        std::array<const char*, totalFractalNum> ret{};
        for (int i = 0; i < totalFractalNum; i++)
            ret[i] = fractalInfo[i].name;
        return ret;
    }
    inline constexpr std::array fractalNames = getFractalNames();

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
        vec3{0.0, 0.0, 0.0},
        vec3{0.5, 0.3, 0.7},
        vec3{0.7, 0.5, 0.3},
        vec3{0.3, 0.3, 0.7},
        vec3{0.3, 0.7, 0.3},
        vec3{0.7, 0.3, 0.3},
        vec3{0.5, 0.5, 0.5},
        vec3{0.8, 0.6, 0.1},
    };
}


