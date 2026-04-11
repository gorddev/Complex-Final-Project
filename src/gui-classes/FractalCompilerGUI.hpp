#pragma once
#include "SDL_API.h"
#include "../fractaldef.hpp"

/* Created by Gordie Novak on 4/7/26.
 * Purpose: 
 * Small menu that's used to compile new fractals and add theme to the scene */

namespace gan {

    struct FractalCompilerGUI {
        int currentFractalSelection{};      ///< Current fractal that is selected by the fractal array
        bool freezeCursor = false;          ///< If the cursor is currently frozen.

        enum displayResult {
            NO_RESULT ,
            NEW_FRACTAL_PANEL,
            INPLACE_FRACTAL
        };

        /** Returns an index of the fractal to compile back to the FractalExplorer if there is something to compile.
         * -1 if there is nothing to compile.*/
        displayResult display(const Window& window, const size_t num_fractal_panels) {
            displayResult ret = NO_RESULT;
            ImGui::Begin("Compiler", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);

            ImGui::Combo("##Compile", &currentFractalSelection, fractal::fractalNames, fractal::totalFractalNum, 8);
            if (num_fractal_panels == 0) {
                if (ImGui::Button("Compile")) {
                    ret = NEW_FRACTAL_PANEL;
                }
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.2f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 1.0f, 0.4f, 1.0f));
                if (ImGui::Button("Recompile")) {
                    ret = INPLACE_FRACTAL;
                } ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.4f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.7f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 1.0f, 1.0f, 1.0f));
                if (ImGui::Button("Compile New")) {
                    ret = NEW_FRACTAL_PANEL;
                }
                ImGui::PopStyleColor(6);
                ImGui::Checkbox("Freeze Cursor (Press Enter)", &freezeCursor);
            }

            ImGui::Text("> Hover For Help <");
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Right-Click to move around fractal\n"
                                  "Scroll to Zoom in/out\n"
                                  "Click 'Compile' to generate a fractal\n"
                                  "Edit fractal properties in the fractal menu.");
            }

            ImGui::SetWindowPos({0.f, window.getHeight() - ImGui::GetWindowSize().y});

            ImGui::Text("Window Size: %i %i", window.getWindowPixelSize().x, window.getWindowPixelSize().y);

            if (failedLastCompilation) {
                ImGui::Text("Compilation Failed. Hover for error.");
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("%s", compilationFailureError.c_str());
                }
            }

            ImGui::End();

            return ret;
        }

        void reportCompileError() {
            failedLastCompilation = true;
            compilationFailureError = GAN_GetLog();
            GAN_ClearLog();
        }

        void clearCompileError() {
            failedLastCompilation = false;
        }

    private:
        bool failedLastCompilation = false; ///< Whether the last compilation failed or not
        std::string compilationFailureError;///< Current compilation failure error.

    };

}
