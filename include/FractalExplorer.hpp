#pragma once
#include <vector>
#include "gpu-fractals/GPUFractalPanel.hpp"
#include "gui/FractalCompilerGUI.hpp"
#include "gui/FractalCursor.hpp"
#include <fractaldef.hpp>

namespace gan {

    /** Contains all FractcalViews and already compiled fractals.
     *  Generates new fractals and handles the distribution of input to all fractals */
    class FractalExplorer {
    public:

        /** Displays all fractals and all relevant GUI interfaces */
        void display(const Window& window, vec2 mousePos);

        void toggleCursorFreeze();

        /** Moves all internal windows around. */
        void reorganize(const Window& window) const;

        void onMouseDown(const Window& window, const Mouse& mouse);
        void onMouseRelease(const Window& window, const Mouse& mouse);
        void onMouseMotion(const Window& window, const Mouse& mouse);
        void onMouseWheel(const Window& window, const Mouse& mouse);


    private:
        std::vector<std::unique_ptr<FractalPanel>> panels; ///< All currently active views into fractals.
        FractalCompilerGUI compilerGUI;
        FractalCursor cursor;

        void instantiateFractalPanel(size_t, bool createNewPanel);            ///< Creates a new fractalView from the compile window.
    };

}
