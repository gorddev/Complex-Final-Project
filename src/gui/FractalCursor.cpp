#include "../../include/gui/FractalCursor.hpp"

using namespace gan;

size_t FractalCursor::findHoveredFractal(
    vec2 nMousePos,
    const std::vector<std::unique_ptr<FractalPanel>>& panels
) {
    for (size_t i = 0; i < panels.size(); ++i) {
        if (panels[i]->containedWithin(nMousePos)) {
            return static_cast<size_t>(i);
        }
    }

    return -1;
}

void FractalCursor::onMouseDown(
    const Window& window,
    const Mouse& mouse,
    const std::vector<std::unique_ptr<FractalPanel>>& panels)
 {
    const vec2 nPos = window.normalizeToWindow(mouse.getPos());

    selectedFractal = findHoveredFractal(nPos, panels);

    if (selectedFractal == -1) {
        return;
    }

    panels[selectedFractal]->onMouseDown(window, mouse);
}

void FractalCursor::onMouseMotion(
    const Window& window,
    const Mouse& mouse,
    const std::vector<std::unique_ptr<FractalPanel>>& panels)
{
    const vec2 nPos = window.normalizeToWindow(mouse.getPos());

    int hovered = findHoveredFractal(nPos, panels);
    if (selectedFractal >= 0 && selectedFractal < static_cast<int>(panels.size()) && hovered != selectedFractal) {
        panels[selectedFractal]->onMouseLeave(window, mouse);
    }
    selectedFractal = hovered;

    if (selectedFractal == -1) {
        return;
    }

    panels[selectedFractal]->onMouseMotion(window, mouse);

    realSelectionPos =
        panels[selectedFractal]->normalizeToFractalPos(window, nPos);
}

void FractalCursor::onMouseWheel(
    const Window& window,
    const Mouse& mouse,
    const std::vector<std::unique_ptr<FractalPanel>>& panels)
{

    const vec2 nMousePos = window.normalizeToWindow(mouse.getPos());

    selectedFractal = findHoveredFractal(nMousePos, panels);

    if (selectedFractal == -1) {
        return;
    }

    panels[selectedFractal]->onMouseWheel(window, mouse);

    realSelectionPos =
        panels[selectedFractal]->normalizeToFractalPos(
            window,
            nMousePos
        );
}

void FractalCursor::onMouseRelease(const Window& window, const Mouse& mouse,
    const std::vector<std::unique_ptr<FractalPanel>>& panels)
{
    if (selectedFractal != -1 && selectedFractal < static_cast<int>(panels.size())) {
        panels[selectedFractal]->onMouseRelease(window, mouse);
    }
}

void FractalCursor::tick(std::vector<std::unique_ptr<FractalPanel>>& panels, bool isCursorFrozen) {
    if (!isCursorFrozen) {
        selectionPos +=
            (realSelectionPos - selectionPos) * 0.1f;
        for (auto& p : panels) {
            p->updateCursorPos(selectionPos);
        }
    }
}

int FractalCursor::getSelectedFractal() const {
    return selectedFractal;
}

const vec2& FractalCursor::getSelectionPos() const {
    return selectionPos;
}

const vec2& FractalCursor::getTruePos() const {
    return realSelectionPos;
}

