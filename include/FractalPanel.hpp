#pragma once
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
        virtual void draw(const Window&) = 0;

        /// Embeds a fractal into the panel
        virtual bool embed(size_t) = 0;

        /// Returns true if the position is contained within the fractal panel.
        [[nodiscard]] virtual bool containedWithin(vec2) const = 0;
        /// Converts a normalized position to a position contained within the fractal.
        [[nodiscard]] virtual vec2 normalizeToFractalPos(const Window&, vec2) const = 0;

        // Mouse functions: Only called when mouse is interacting with the fractal
        /// Determines what happens when the mouse moves
        virtual void onMouseMotion(const Window&, const Mouse&) = 0;
        /// Determines what happens when the mouse is clicked
        virtual void onMouseDown(const Window&, const Mouse&) = 0;
        /// Determines what happens when the mouse is released
        virtual void onMouseRelease(const Window&, const Mouse&) {};
        /// Determines what happens when the mouse wheel is scrolled
        virtual void onMouseWheel(const Window&, const Mouse&) {};
        /// Determines what happens when the mouse leaves the window
        virtual void onMouseLeave(const Window&, const Mouse&) {};
        // Global cursor update function
        /// Updates the fractal with the global cursor position.
        virtual void updateCursorPos(vec2) {}

        /// Sets the target draw region for a fractal (coords in complex plane)
        virtual void setDrawRegion(const vec4&) {}
    };

}