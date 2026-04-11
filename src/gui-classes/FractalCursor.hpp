#pragma once

#include "SDL_API.h"

namespace gan {

    struct FractalCursor {
        int selectedFractal = -1;
        vec2 selectionPos{};

        /** Finds which fractal is currently being hovered over in the case of multiple fractals being displayed */
        static fractal_id findHoveredFractal(vec2 nMousePos, std::vector<FractalPanel>& panels) {
            for (fractal_id i = 0; i < panels.size(); ++i) {
                if (panels[i].containedWithin(nMousePos)) {
                    return i;
                }
            }
            return fractal_id(-1);
        }

        /** Updates the selection pos on mouse motion */
        void onMouseMotion(const Window& window, const Mouse& mouse, std::vector<FractalPanel>& panels) {
            const vec2 n_pos = window.normalizeToWindow(mouse.getPos());

            /* If the currently selected fractal is valid */
            if (selectedFractal >= 0 && selectedFractal <= panels.size() && mouse.isLeftClicked()) {
                    const float aspect_ratio = (window.getWidth())/window.getHeight();
                    panels[selectedFractal].moveFractal(
                        vec2{
                            mouse.getDeltaPos().x*panels[selectedFractal].getScale()/(window.getWidth()) * aspect_ratio,
                            -mouse.getDeltaPos().y*panels[selectedFractal].getScale()/(window.getHeight())
                        }
                    );
                selectionPos = panels[selectedFractal].normalizeToFractalPos(n_pos, window);
            }
            /* If we currently do not have a valid selected fractal */
            else {
                selectedFractal = findHoveredFractal(n_pos, panels);
                if (selectedFractal != -1) {
                    if (mouse.isLeftClicked()) {
                        panels[selectedFractal].moveFractal(
                            vec2{
                                mouse.getDeltaPos().x*panels[selectedFractal].getScale()/(window.getWidth())*static_cast<float>(window.getWidth())/window.getHeight(),
                                -mouse.getDeltaPos().y*panels[selectedFractal].getScale()/(window.getHeight())
                            }
                        );
                    }
                    realSelectionPos = panels[selectedFractal].normalizeToFractalPos(n_pos, window);
                }
            }

        }

        void onMouseWheel(const Window& window, const Mouse& mouse, std::vector<FractalPanel>& panels) {
            vec2 nMousePos = window.normalizeToWindow(mouse.getPos());
            selectedFractal = findHoveredFractal(nMousePos, panels);

            #ifndef __EMSCRIPTEN__
            constexpr float zoomAmount = -0.1f;
            #else
            constexpr float zoomAmount = -0.3f;
            #endif

            if (selectedFractal != -1) {
                auto& panel = panels[selectedFractal];

                const float oldScale = panel.getScale();
                const float zoomFactor = std::exp(mouse.getScrollWheelY() * zoomAmount);
                const float newScale = oldScale * zoomFactor;

                const vec2 mpos = mouse.getPos(); // pixel coords
                const vec2 winSize = vec2{window.getWidth(), window.getHeight()};

                vec2 ndc = vec2{mpos.x / winSize.x, mpos.y/winSize.y} * 2.0f - vec2{1.0f, 1.f};

                vec2 windowPos{panel.getWindowPos().x, -1.f*panel.getWindowPos().y};
                vec2 adjPos = (ndc - windowPos*2.f);
                vec2 before = panel.getCenterPos() + adjPos* oldScale;

                panel.setScale(newScale);

                vec2 after = panel.getCenterPos() + adjPos * newScale;
                vec2 composite = before - after;
                composite.y *= -1.f;

                composite.x *= window.getWidth()/window.getHeight();

                panel.setCenterPos(panel.getCenterPos() + composite/2.f);

                realSelectionPos = panels[selectedFractal].normalizeToFractalPos(window.normalizeToWindow(mouse.getPos()), window);
            }
        }

        void tick(bool isCursorFrozen) {
            if (!isCursorFrozen) {
                selectionPos += (realSelectionPos - selectionPos) * 0.1f;
            }
        }

        const vec2& getTruePos() const {
            return realSelectionPos;
        }

    private:
        vec2 realSelectionPos{}; // The actually direct read selection pos that isn't smoothed.
    };

}