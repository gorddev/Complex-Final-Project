#pragma once

// Contains the definition for what makes a valid draw function for a CPU Fractal.
// It must start at a start line, and modify

#include <SDL_API.h>

namespace gan::cfr {
    struct FractalData;

    // ----------------------------------------------------

    /** Draw Instruction sent to a fractal function */
    struct DrawCommand {
        u8* const pixels;       ///< Pixels the thread should draw to. command.pixels[0] is the first pixel it should draw to.
        const u32 start_line;   ///< Line the function should start on.
        const u32 end_line;     ///< Number of lines the function should draw to.
        const u32 pitch;        ///< How long each row is.
    };

    // ------------------------------------------------------
    using DrawMethod = void (*)(const FractalData&, DrawCommand);
    // ------------------------------------------------------
}