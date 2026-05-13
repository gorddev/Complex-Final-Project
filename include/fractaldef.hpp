#pragma once

#include <cstdint>

namespace gan::fractal {
    inline constexpr uint32_t maxFractalViews = 4;
    inline constexpr float defaultScale = 3.0f;
    /// Maximum number of iterations allowed.
    #ifdef __EMSCRIPTEN__
    inline uint32_t maxIterations = 400;
    #else
    inline uint32_t maxIterations = 750;
    #endif

    inline int getPanelNumber() {
        static int panelNum = 0;
        return panelNum++;
    }
}