#pragma once

#include "SDL_API.h"

namespace gan {
    void SetupImGuiStyle() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        // --- Main Style Settings ---
        style.WindowRounding    = 6.0f;
        style.ChildRounding     = 4.0f;
        style.FrameRounding     = 4.0f;
        style.GrabRounding      = 4.0f;
        style.PopupRounding     = 4.0f;
        style.ScrollbarRounding = 12.0f;
        style.WindowPadding     = ImVec2(10, 10);
        style.FramePadding      = ImVec2(6, 4);
        style.ItemSpacing       = ImVec2(8, 6);

        // --- Color Palette ---
        colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_Border]                 = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    }
}