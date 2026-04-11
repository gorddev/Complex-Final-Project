#include "FractalExplorer.hpp"

using namespace gan;


void FractalExplorer::display(const Window& window, vec2 mousePos) {

    // Now we go through each of our view

    cursor.tick(compilerGUI.freezeCursor);

    if (cursor.selectedFractal != -1) {

        ImGui::SetNextWindowPos(ImVec2(mousePos.x + 5, mousePos.y - 30));

        ImGui::Begin("OverlayWindow", nullptr,
                    ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_AlwaysAutoResize |
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoScrollbar |
                    ImGuiWindowFlags_NoInputs |
                    ImGuiWindowFlags_NoBackground);


        const vec2& realPos = cursor.getTruePos();
        ImGui::Text("%.6f + %.6fi", realPos.x, realPos.y);
        ImGui::End();
    }

reset_display:
    for (auto it = panels.begin(); it != panels.end(); ++it) {
        it->imguiBegin(window);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
        bool remove = ImGui::Button("Remove Fractal");
        ImGui::PopStyleColor(3);

        it->imguiBody(window);
        it->imguiEnd();

        if (remove) {
            panels.erase(it);
            reorganize(window);
            goto reset_display;
        } else {
            it->draw(window, !compilerGUI.freezeCursor, cursor.selectionPos);
        }
    }

    // First get what we need from the compiler GUI.
    auto result = compilerGUI.display(window, panels.size());
    if (result == FractalCompilerGUI::NEW_FRACTAL_PANEL) {
        instantiateFractalPanel(compilerGUI.currentFractalSelection, true);
        reorganize(window);
    } else if (result == FractalCompilerGUI::INPLACE_FRACTAL) {
        instantiateFractalPanel(compilerGUI.currentFractalSelection, false);
        reorganize(window);
    }
}

void FractalExplorer::toggleCursorFreeze() {
    compilerGUI.freezeCursor = !compilerGUI.freezeCursor;
}

void FractalExplorer::reorganize(const Window& window) {
    switch (panels.size()) {
    case 1:
        panels[0].reframe(window, {0.0, 0.0}, {1.0, 1.0});
        break;
    case 2:
        panels[0].reframe(window, {0.0, 0.0}, {0.5, 1.0});
        panels[1].reframe(window, {0.5, 0.0}, {0.5, 1.0});
        break;
    case 3:
        panels[0].reframe(window, {0.0, 0.0}, {1.0, 0.5});
        panels[1].reframe(window, {0.0, 0.5}, {0.5, 0.5});
        panels[2].reframe(window, {0.5, 0.5}, {0.5, 0.5});
        break;
    case 4:
        panels[0].reframe(window, {0.0, 0.0}, {0.5, 0.5});
        panels[1].reframe(window, {0.5, 0.0}, {0.5, 0.5});
        panels[2].reframe(window, {0.0, 0.5}, {0.5, 0.5});
        panels[3].reframe(window, {0.5, 0.5}, {0.5, 0.5});
        break;
    default:
        break;
    }
}

void FractalExplorer::onMouseMotion(const Window&window, const Mouse& mouse) {
    cursor.onMouseMotion(window, mouse, panels);
}

void FractalExplorer::onMouseWheel(const Window& window, const Mouse& mouse) {
    cursor.onMouseWheel(window, mouse, panels);
}

void FractalExplorer::instantiateFractalPanel(const fractal_id id, bool newPanel) {
    size_t targetPanel = panels.size();
    if (newPanel || panels.empty()) {
        if (targetPanel >= fractal::maxFractalViews) {
            targetPanel = fractal::maxFractalViews - 1;
        } else {
            panels.emplace_back();
        }
    } else if (panels.empty()) {
        panels.emplace_back();
    } else {
        targetPanel--;
    }

    auto uniqueOpt = Fractal::make_unique(
        fractal::fractalInfo[id]
    );
    if (uniqueOpt) {
        panels[targetPanel].embed(std::move(uniqueOpt.value()));
        compilerGUI.clearCompileError();
    } else {
        compilerGUI.reportCompileError();
        panels.pop_back();
    }
}