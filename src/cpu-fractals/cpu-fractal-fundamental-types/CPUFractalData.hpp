#pragma once
#include "CPUFractalArgument.hpp"
#include <vector>
#include <span>

/// Denotes the namespace containing cpu fractal related procedures.
namespace gan::cfr {

    /** Core of a CPU fractal. Contains essential rendering information:
     * - x, y: Top left x and y of the fractal
     * - step: How far each pixel is drawn
     * - iterations: number of iterations performed in the algorithm
     * - arguments: number of fractal-specific arguments applied to the fractal (like powers or other custom params)
     * - numArguments: the number of arguments in the arguments array. */
    struct FractalData {
        cfloat x{}, y{};  ///< Starting x and y position of the fractal
        cfloat step{};  ///< How far to move per iteration in x & y direction.
        u32 iterations{};     ///< Number of iterations the fractal will run for.

        /** Additional parameter that may be passed to functions */
        Argument arguments[maxArguments]{};
        u32 numArguments{};   ///< Number of arguments provided to fractal functions.

        constexpr FractalData() = default;
    };

}
