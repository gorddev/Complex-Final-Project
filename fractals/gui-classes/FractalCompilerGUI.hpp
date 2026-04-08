#pragma once
#include "SDL_API.h"
#include "../FractalConstants.hpp"

/* Created by Gordie Novak on 4/7/26.
 * Purpose: 
 * Small menu that's used to compile new fractals and add theme to the scene */

namespace gan {

    struct FractalCompilerGUI {
        int currentCompilerWindowItem{};

        /** Returns an index of the fractal to compile back to the FractalExplorer if there is something to compile.
         * -1 if there is nothing to compile.*/
        int display(const Window& window) {
            int ret = -1;
            ImGui::Begin("Compiler", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);

            ImGui::Combo("##Compile", &currentCompilerWindowItem, fractal::fractalNames, 4, fractal::totalFractalNum);
            if (ImGui::Button("Compile")) {
                ret = currentCompilerWindowItem;
            }

            ImGui::SameLine();
            ImGui::Text("Hover For Help");
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Right-Click to move around fractal\n"
                                  "Scroll to Zoom in/out\n"
                                  "Click 'Compile' to generate a fractal\n"
                                  "Edit fractal properties in the fractal menu.");
            }

            ImGui::SetWindowPos({0.f, window.getHeight() - ImGui::GetWindowSize().y});

            ImGui::Text("Window Size: %i %i", window.getWidth(), window.getHeight());
            ImGui::End();

            return ret;
        }

    };

}
