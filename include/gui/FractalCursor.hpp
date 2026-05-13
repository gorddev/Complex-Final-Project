// FractalCursor.hpp
#pragma once

#include "SDL_API.h"
#include "../FractalPanel.hpp"
#include <vector>

namespace gan {

/**
 * @brief Handles fractal selection and cursor interaction across multiple panels.
 *
 * Tracks the currently hovered fractal panel, forwards mouse interaction
 * events to the active panel, and maintains both a smoothed and real
 * fractal-space cursor position.
 */
class FractalCursor {
public:
    /**
     * @brief Constructs a default fractal cursor.
     */
    FractalCursor() = default;

    /**
     * @brief Handles mouse button press events.
     * Updates the selected fractal and forwards the mouse-down event
     * to the active fractal panel.
     * @param window Application window.
     * @param mouse Mouse input state.
     * @param panels Collection of fractal panels.
     * @param cursorFrozen
     */
    void onMouseDown(
        const Window& window,
        const Mouse& mouse,
        const std::vector<std::unique_ptr<FractalPanel>>& panels
    );

    /** @brief Handles mouse movement events.
     * Updates the hovered fractal, forwards the movement event,
     * and recalculates the current fractal-space selection position.
     * @param window Application window.
     * @param mouse Mouse input state.
     * @param panels Collection of fractal panels.
     */
    void onMouseMotion(
        const Window& window,
        const Mouse& mouse,
        const std::vector<std::unique_ptr<FractalPanel>>& panels
    );

    /** @brief Handles mouse wheel zoom events.
     * Updates the selected fractal panel, applies zooming,
     * and refreshes the fractal-space cursor position.
     * @param window Application window.
     * @param mouse Mouse input state.
     * @param panels Collection of fractal panels. */
    void onMouseWheel(
        const Window& window,
        const Mouse& mouse,
        const std::vector<std::unique_ptr<FractalPanel>>& panels
    );

    void onMouseRelease(
        const Window& window,
        const Mouse& mouse,
        const std::vector<std::unique_ptr<FractalPanel>>& panels);

    /** @brief Updates the smoothed cursor position.
     * Applies interpolation toward the real cursor position unless
     * cursor smoothing is frozen.
     * @param panels
     * @param isCursorFrozen Whether smoothing updates are disabled.
     */
    void tick(std::vector<std::unique_ptr<FractalPanel>>& panels, bool isCursorFrozen);

    /** @brief Gets the currently selected fractal index.
     * @return Selected fractal ID by value. */
    [[nodiscard]] int getSelectedFractal() const;

    /** @brief Gets the smoothed fractal-space cursor position.
     * @return Reference to the smoothed selection position. */
    [[nodiscard]] const vec2& getSelectionPos() const;


    /**
     * @brief Gets the unsmoothed fractal-space cursor position.
     * @return Reference to the true cursor position.
     */
    [[nodiscard]]
    const vec2& getTruePos() const;

private:
    int selectedFractal{-1};

    /// Smoothed fractal-space cursor position.
    vec2 selectionPos{};
    /// Immediate fractal-space cursor position without smoothing.
    vec2 realSelectionPos{};

    /** @brief Finds the fractal panel currently under the mouse cursor.
     * Iterates through all available panels and returns the first panel
     * whose bounds contain the normalized mouse position.
     * @param nMousePos Mouse position normalized to window coordinates.
     * @param panels Collection of fractal panels.
     * @return The hovered fractal ID, or -1 if none are hovered.
     */
    [[nodiscard]]
    static size_t findHoveredFractal(
        vec2 nMousePos,
        const std::vector<std::unique_ptr<FractalPanel>>& panels
    );
};

} // namespace gan