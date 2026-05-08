#pragma once

#include "SDL_API.h"

namespace gan {

    struct FractalCursor {
        int selectedFractal = -1;
        vec2 selectionPos{};

        /** Finds which fractal is currently being hovered over in the case of multiple fractals being displayed */
        static fractal_id findHoveredFractal(vec2 nMousePos, const std::vector<std::unique_ptr<FractalPanel>>& panels) {
            for (size_t i = 0; i < panels.size(); ++i) {
                if (panels[i]->containedWithin(nMousePos)) {
                    return i;
                }
            }
            return -1;
        }

        /** Updates the selection pos on mouse motion */
        void onMouseMotion(const Window& window, const Mouse& mouse, std::vector<std::unique_ptr<FractalPanel>>& panels) {
            const vec2 n_pos = window.normalizeToWindow(mouse.getPos());

            selectedFractal = findHoveredFractal(n_pos, panels);
            if (selectedFractal == -1) return;

            if (mouse.isLeftClicked()) {
                panels[selectedFractal]->moveFractal(window, mouse);
            }

            realSelectionPos = panels[selectedFractal]->normalizeToFractalPos(window, n_pos);
        }

        void onMouseWheel(const Window& window, const Mouse& mouse, std::vector<std::unique_ptr<FractalPanel>>& panels) {
            vec2 nMousePos = window.normalizeToWindow(mouse.getPos());
            selectedFractal = findHoveredFractal(nMousePos, panels);

            if (selectedFractal != -1) {
                panels[selectedFractal]->zoom(window, mouse);
                realSelectionPos = panels[selectedFractal]->normalizeToFractalPos(window, window.normalizeToWindow(mouse.getPos()));
            }
        }

        void tick(const bool isCursorFrozen) {
            if (!isCursorFrozen) {
                selectionPos += (realSelectionPos - selectionPos) * 0.1f;
            }
        }

        [[nodiscard]] const vec2& getTruePos() const {
            return realSelectionPos;
        }

    private:
        vec2 realSelectionPos{}; // The actually direct read selection pos that isn't smoothed.
    };

}
