#pragma once

#include "SDL_API.h"

namespace gan {

    struct FractalCursor {
        int selectedFractal = -1;
        vec2 selectionPos{};

        static fractal_id findHoveredFractal(vec2 nMousePos, std::vector<FractalPanel>& panels) {
            for (fractal_id i = 0; i < panels.size(); ++i) {
                if (panels[i].containedWithin(nMousePos)) {
                    return i;
                }
            }
            return fractal_id(-1);
        }

        void onMouseMotion(const Window& window, const Mouse& mouse, std::vector<FractalPanel>& panels) {
            const vec2 n_pos = window.normalizeToWindow(mouse.getPos());
            if (selectedFractal >= 0 && selectedFractal <= panels.size() && mouse.isLeftClicked()) {
                panels[selectedFractal].moveFractal(
                    vec2{
                        mouse.getDeltaPos().x*panels[selectedFractal].getScale()/window.getWidth() * static_cast<float>(window.getWidth())/window.getHeight(),
                        -mouse.getDeltaPos().y*panels[selectedFractal].getScale()/window.getHeight()
                    }
                );
                selectionPos = panels[selectedFractal].normalizeToFractalPos(n_pos, window);
            } else {
                selectedFractal = findHoveredFractal(n_pos, panels);
                if (selectedFractal != -1) {
                    if (mouse.isLeftClicked()) {
                        panels[selectedFractal].moveFractal(
                            vec2{
                                mouse.getDeltaPos().x*panels[selectedFractal].getScale()/window.getWidth() * static_cast<float>(window.getWidth())/window.getHeight(),
                                -mouse.getDeltaPos().y*panels[selectedFractal].getScale()/window.getHeight()
                            }
                        );
                    }
                    selectionPos = panels[selectedFractal].normalizeToFractalPos(n_pos, window);
                }
            }

        }

        void onMouseWheel(const Window& window, const Mouse& mouse, std::vector<FractalPanel>& panels) {
            vec2 nMousePos = window.normalizeToWindow(mouse.getPos());
            selectedFractal = findHoveredFractal(nMousePos, panels);

            if (selectedFractal != -1) {
                auto& panel = panels[selectedFractal];

                float oldScale = panel.getScale();
                float zoomFactor = std::exp(mouse.getScrollWheelY() * -0.1f);
                float newScale = oldScale * zoomFactor;
                panel.setScale(newScale);

                selectionPos = panel.normalizeToFractalPos(nMousePos, window);
            }
        }
    };

}