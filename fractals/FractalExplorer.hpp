#pragma once
#include <vector>
#include "FractalPanel.hpp"
#include "gui-classes/FractalCompilerGUI.hpp"
#include "gui-classes/FractalCursor.hpp"

namespace gan {


    /** Contains all FractalViews and already compiled fractals.
     *  Generates new fractals and handles the distribution of input to all fractals */
    class FractalExplorer {
    private:
        std::vector<FractalPanel> panels;               ///< All currently active views into fractals.
        FractalCompilerGUI compilerGUI;
        FractalCursor cursor;

        void instantiateFractalPanel(fractal_id, bool newPanel);            ///< Creates a new fractalView from the compile window.
    public:

        /** Displays all fractals and all relevant GUI interfaces */
        void display(const Window& window, vec2 mousePos);
        /** Moves all internal windows around. */
        void reorganize(const Window& window);
        /** Moves a fractal on mouse_motion */
        void onMouseMotion(const Window& window, const Mouse& mouse);
        /** Scales a fractal on mouse wheel */
        void onMouseWheel(const Window& window, const Mouse& mouse);
    };

}
