#include "FractalExplorer.hpp"

using namespace gan;


void FractalExplorer::display(const Window& window, vec2 mousePos) {

    // Now we go through each of our view

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


        ImGui::Text("%.6f + %.6fi", cursor.selectionPos.x, cursor.selectionPos.y);
        ImGui::End();
    }

reset_display:
    for (auto it = panels.begin(); it != panels.end(); ++it) {
        it->imguiBegin(window);

        bool remove = ImGui::Button("Remove Fractal View");

        it->imguiBody(window);
        it->imguiEnd();

        if (remove) {
            cachedFractals.push_back(it->extract());
            panels.erase(it);
            reorganize(window);
            goto reset_display;
        } else {
            it->draw(window, cursor.selectionPos);
        }
    }

    // First get what we need from the compiler GUI.
    auto id = compilerGUI.display(window);
    if (id >= 0 && id < fractal::totalFractalNum) {
        instantiateNewFractalPanel(id);
        reorganize(window);
    }

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

void FractalExplorer::instantiateNewFractalPanel(fractal_id id) {
    auto targetPanel = panels.size();

    if (targetPanel >= fractal::maxFractalViews) {
        // If we already have the max number of views on the screen, steal the
        // existing fractal from the current view
        targetPanel = fractal::maxFractalViews - 1;
        if (auto ptr = panels[targetPanel].extract()) {
            cachedFractals.push_back(std::move(ptr));
        }
    } else {
        // Otherwise just create a new fractalViewer
        panels.emplace_back();
    }

    auto it = locateCachedFractal(id);
    if (it != cachedFractals.end()) {
        panels[targetPanel].embed(std::move(*it));
        cachedFractals.erase(it);
        compilerGUI.clearCompileError();
    } else {
        try {
            auto unique = Fractal::make_unique(
                fractal::fractalInfo[id]
            );
            panels[targetPanel].embed(std::move(unique));
            compilerGUI.clearCompileError();
        } catch (...) {
            compilerGUI.reportCompileError();
            panels.pop_back();
        }
    }

    if (!panels.empty()) {
        std::println("{}", panels.back().checkHealth());
    }
}

std::vector<std::unique_ptr<Fractal>>::iterator FractalExplorer::locateCachedFractal(fractal_id id) {
    auto targetName = fractal::fractalInfo[id].name;
    auto it = std::ranges::find_if(cachedFractals,
        [&](const auto& f) { return f->name == targetName; });
    return it;
}