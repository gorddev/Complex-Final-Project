#pragma once


#include "../cpufractaldef.hpp"
#include "SDL_API.h"

/* Created by Gordie Novak on 4/13/26.
* Purpose:
* Contains important definitions that are used to create a CPU fractal */

namespace gan::cfr {

    /** A tagged union of parameter information a cpu function can use. */
    struct ArgumentData {
        enum ArgumentType {FLOAT, INT, VEC2, IVEC2} type;
        union { float x; int ix; };
        union { float y; int iy; };

        constexpr ArgumentData()
            : type(FLOAT), x(), y() {}
        explicit constexpr ArgumentData(float x)
            : type(FLOAT), x(x), y() {}
        explicit constexpr ArgumentData(float x, float y)
            : type(VEC2), x(x), y(y) {}
        explicit constexpr ArgumentData(int ix)
            : type(INT), ix(ix), iy() {}
        explicit constexpr ArgumentData(int ix, int iy)
            : type(IVEC2), ix(ix), iy(iy) {}
    };

    /** The makeup of a single parameter. Contains:
     * - const char* param name: name of the parameter that will be displayed
     * - float min, max: minimum and max values of a specific parameter.
     * - ArgumentData data: actual data that's stored in the argument
     */
    struct Argument {
        const char* param_name; ///< Name of the parameter that will be displayed
        float min, max;         ///< Minimum and maximum values of a specific parameter
        ArgumentData data;      ///< Internal data of the parameter.

        constexpr Argument() :
            param_name("NO_PARAM_NAME"), min(0.f), max(0.f), data(ArgumentData()) {}
        constexpr Argument(const char* name, float min, float max, ArgumentData data = ArgumentData{})
            : param_name(name), min(min), max(max), data(data) {}
    };
}
