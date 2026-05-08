#pragma once
#include "fractaldef.hpp"
#include "SDL_API.h"

/* Created by Gordie Novak on 4/12/26.
 * Purpose: 
 * generic interface for GPUFractalPanels and CPUFractalPanels*/

namespace gan {

    /// Interface that all fractal panels must follow.
    struct FractalPanel {
        virtual ~FractalPanel() = default;

        // Pure virtual functions

        /// Changes the location of the window.
        virtual void reframe(const Window&, vec2 pos, vec2 size) = 0;

        /// ImGui Creation Functions
        virtual void imguiBegin(const Window&) = 0;
        virtual void imguiBody(const Window&) = 0;
        virtual void imguiEnd() = 0;

        /// Draws the fractal panel onto the screen
        virtual void draw(const Window&, bool cursorEnabled, vec2 cursorPos) = 0;

        /// Embeds a fractal into the panel
        virtual bool embed(fractal_id) = 0;

        /// Returns true if the position is contained within the fractal panel.
        [[nodiscard]] virtual bool containedWithin(vec2) const = 0;
        /// Converts a normalized position to a position contained within the fractal.
        [[nodiscard]] virtual vec2 normalizeToFractalPos(const Window&, vec2) const = 0;

        // Regular virtual functions

        /// Zooms in a given fractal panel
        virtual void zoom(const Window&, const Mouse&) {}
        /// Moves a fractal panel by a given amount
        virtual void moveFractal(const Window&, const Mouse&) {}
        /// Sets the target draw region for a fractal (coords in complex plane)
        virtual void setDrawRegion(const vec4&) {}
    };

}