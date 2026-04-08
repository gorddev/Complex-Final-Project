#include "ColorSelectorGUI.hpp"

using namespace gan;

void ColorSelectorGUI::display(std::unique_ptr<Fractal>& fractal) {
    if (ImGui::CollapsingHeader("Colors")) {
        if (ImGui::SliderInt("Num Colors: ", &fractal->numColors, fractal::min_colors, fractal::max_colors)) {
            fractal->uNumColors1i();
            if (currentColor >= fractal->numColors) {
                currentColor = fractal->numColors - 1;
            }
        }
        ImGui::Separator();
        for (size_t i = 0; i < fractal->numColors; ++i) {
            auto j = fractal->numColors - i - 1;
            ImGui::PushStyleColor(ImGuiCol_Button, {fractal->colors[j].r, fractal->colors[j].g, fractal->colors[j].b, 0.7f});
            bool pop_style = currentColor == i;
            if (pop_style) {
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 8.0f);
            }
            if (ImGui::Button((std::string("C") + std::to_string(i)).c_str())) {
                currentColor = i;
            } ImGui::SameLine();
            if (pop_style) {
                ImGui::PopStyleVar();
            }
            ImGui::PopStyleColor();
        }
        ImGui::NewLine();
        ImGui::Separator();
        ImGui::Text("%s", (std::string("Color: ") + std::to_string(currentColor)).c_str());

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x/2);
        if (ImGui::ColorPicker3("##Select Color", reinterpret_cast<float*>(fractal->colors.data() + fractal->numColors - currentColor - 1),
            ImGuiColorEditFlags_NoInputs)) {
            fractal->uColors3fv();
        }
    }
}


