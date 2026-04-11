#pragma once
#include <memory>
#include "../fractals/Fractal.hpp"

/* Created by Gordie Novak on 4/7/26.
 * Purpose: 
 * Allows for easy selection of colors of a fractal without
 * bloating up the FractalPanel class. ImGui Wrapper */

namespace gan {

    struct ColorSelectorGUI {
        unsigned int currentColor{}; ///< Current color selected.

        ColorSelectorGUI() = default;

        /// Displays the color selector.
        void display(std::unique_ptr<Fractal>& fractal);

    };
}
