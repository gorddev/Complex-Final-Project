#include "./FractalPanel.hpp"

#include "../fractaldef.hpp"
#include "imgui-integration/Imgui_Init.hpp"

constexpr float bsize = 0.01;
constexpr float thickness = 0.005f;

void gan::FractalPanel::genBorder(const Window& window, vec2 pos, vec2 size, vec4 color, const float thickness = 0.01) {
    const float cx     = (pos.x - 0.5f + size.w/2)*2;
    const float cy     = -(pos.y - 0.5f + size.h/2)*2;
    const float w      = (size.w)*2;
    const float h      = (size.h)*2;
    const float aspect = window.getWidth() / static_cast<float>(window.getHeight());
    const float th     = thickness;          // Vertical thickness (Top/Bottom bars)
    const float tw     = thickness / aspect;
    constexpr float z  = 0.f;
    const vec4 c       = color;

    float left   = cx - w / 2.0f;
    float right  = cx + w / 2.0f;
    float top    = cy + h / 2.0f;
    float bottom = cy - h / 2.0f;

    const CVertex borderVertices[24] = {
        // TOP
        {left,  top, z, c.r, c.g, c.b, c.a},
        {right, top, z, c.r, c.g, c.b, c.a},
        {right, top - th,z, c.r, c.g, c.b, c.a},

        {left,  top, z, c.r, c.g, c.b, c.a},
        {right, top - th,z, c.r, c.g, c.b, c.a},
        {left,  top - th,z, c.r, c.g, c.b, c.a},

        // BOTTOM
        {left,  bottom + th, z, c.r, c.g, c.b, c.a},
        {right, bottom + th, z, c.r, c.g, c.b, c.a},
        {right, bottom, z, c.r, c.g, c.b, c.a},

        {left,  bottom + th, z, c.r, c.g, c.b, c.a},
        {right, bottom, z, c.r, c.g, c.b, c.a},
        {left,  bottom, z, c.r, c.g, c.b, c.a},

        // LEFT
        {left,      top, z, c.r, c.g, c.b, c.a},
        {left + tw,  top, z, c.r, c.g, c.b, c.a},
        {left + tw,  bottom, z, c.r, c.g, c.b, c.a},

        {left,      top, z, c.r, c.g, c.b, c.a},
        {left + tw,  bottom, z, c.r, c.g, c.b, c.a},
        {left,      bottom, z, c.r, c.g, c.b, c.a},

        // RIGHT
        {right - tw, top, z, c.r, c.g, c.b, c.a},
        {right,     top, z, c.r, c.g, c.b, c.a},
        {right,     bottom, z, c.r, c.g, c.b, c.a},

        {right - tw, top, z, c.r, c.g, c.b, c.a},
        {right,     bottom, z, c.r, c.g, c.b, c.a},
        {right - tw, bottom, z, c.r, c.g, c.b, c.a},
    };

    if (borderVB.vao != 0) {
        VBuffer<CVertex>::destroy(borderVB);
    }

    borderVB = std::move(VBuffer<CVertex>::make(borderVertices, 24));
}

gan::FractalPanel::FractalPanel(std::unique_ptr<gan::Fractal> fractal)
    :  frac(std::move(fractal))
{
    static int numPanels = 0;
    panelID = std::to_string(numPanels++);
}

gan::FractalPanel::~FractalPanel() {
    VBuffer<CVertex>::destroy(borderVB);
}

std::unique_ptr<gan::Fractal> gan::FractalPanel::embed(std::unique_ptr<Fractal> fractal) {
    if (fractal != nullptr) {
        std::unique_ptr<Fractal> old_fractal = std::move(this->frac);
        this->frac = std::move(fractal);
        return std::move(old_fractal);
    }
    return nullptr;
}

std::unique_ptr<gan::Fractal> gan::FractalPanel::extract() {
    return std::move(frac);
}


void gan::FractalPanel::imguiBegin(const Window& window) {
    SDL_GL_MakeCurrent(window, window.getGLContext());
    ImGui::PushID(panelID.c_str());
    if (frac) {
        // ~~~~~~ Begin the Window ~~~~~
        ImGui::Begin(std::format("{} [{}]", frac->name, panelID).c_str(), nullptr
            ,ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);

        ImGui::Separator(); //—————————————————
    } else {
        ImGui::Begin(std::format("~ERR~ {} [{}]. frac = nullptr", frac->name, panelID).c_str());
    }

    ImGui::SetWindowPos({(frac->windowPos.x+thickness/2)*window.getWidth(), (frac->windowPos.y+thickness/2)*window.getHeight()});
}

void gan::FractalPanel::imguiBody(const Window& window) {


    if (frac) {
        interactedThisFrame = false;

        // ~~~~~~ Window Positioning ~~~~~~~
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, {0.4, 0.4, 0.2, 1.f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.5, 0.6, 0.3, 1.f});
        if (ImGui::Button("Recenter")) { // HOME button
            frac->scale = fractal::defaultScale;
            frac->centerPos = {0.0, 0.0};
            frac->uScale1f();
            frac->uCenterPos2f();
        }
        ImGui::PopStyleColor(2);



        // -- Description --
        if (!frac->description.empty()) {
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, {0.3, 0.3, 0.3, 1.f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.5, 0.5, 0.5, 1.f});
            ImGui::Button("Description");
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("%s", frac->description.c_str());
            }
            ImGui::PopStyleColor(2);
        }


        ImGui::Separator(); //—————————————————
        if (ImGui::SliderFloat("Scale", &frac->scale, 0.000001, 100.0, "%.6f", ImGuiSliderFlags_Logarithmic)) {
            frac->uIterations1i();
            interactedThisFrame = true;
        }

        // ----- Iterations ------
        if (ImGui::SliderInt("Iterations", &frac->iterations, 1, fractal::maxIterations, "%d", ImGuiSliderFlags_Logarithmic)) {
            frac->uIterations1i();
            interactedThisFrame = true;
        }

        ImGui::Separator(); //—————————————————

        // ----- Fractal Property Types
        for (size_t i = 0; i < frac->numProperties; i++) {
            displayProperty(i);
        }

        // ---- Colors -----
        color_selector.display(frac);


        // ---- Debug ----
        ImGui::Text("> DEBUG");
        if (ImGui::IsItemHovered()) {
            if (frac->healthStr.empty())
                frac->healthStr = frac->checkHealth();
            ImGui::SetTooltip("%s", frac->healthStr.c_str());
        } else frac->healthStr.clear();

        imguiWindowSize = window.normalizeToWindow({ImGui::GetWindowSize().x, ImGui::GetWindowSize().y});
    }
}

void gan::FractalPanel::displayProperty(const size_t id) const {
    auto& p = frac->properties[id];
    bool update = false;

    ImGui::PushID(id);
    ImGui::Text("%s", p.propertyName);
    switch (p.type) {
    case fractal::FLOAT:
        update = ImGui::SliderFloat("##p", reinterpret_cast<float*>(&p.data), p.bounds.min, p.bounds.max);
        break;
    case fractal::VEC2:
        update = ImGui::SliderFloat2("##p", reinterpret_cast<float*>(&p.data), p.bounds.min, p.bounds.max);
        break;
    case fractal::VEC3:
        update = ImGui::SliderFloat3("##p", reinterpret_cast<float*>(&p.data), p.bounds.min, p.bounds.max);
        break;
    case fractal::VEC4:
        update = ImGui::SliderFloat4("##p", reinterpret_cast<float*>(&p.data), p.bounds.min, p.bounds.max);
        break;
    case fractal::INT:
        update = ImGui::SliderInt("##p", reinterpret_cast<int*>(&p.data), p.bounds.min, p.bounds.max);
        break;
    case fractal::IVEC2:
        update = ImGui::SliderInt2("##p", reinterpret_cast<int*>(&p.data), p.bounds.min, p.bounds.max);
        break;
    case fractal::IVEC3:
        update = ImGui::SliderInt3("##p", reinterpret_cast<int*>(&p.data), p.bounds.min, p.bounds.max);
        break;
    case fractal::IVEC4:
        update = ImGui::SliderInt4("##p", reinterpret_cast<int*>(&p.data), p.bounds.min, p.bounds.max);
        break;
    }

    if (update) {
        frac->uProperty(id);
    }

    ImGui::PopID();
}

void gan::FractalPanel::imguiEnd() {
    ImGui::End();
    ImGui::PopID();
}

static GLuint borderShaderProgram() {
    const char vert[] =
#ifdef SDL_API_OpenGL_Core
R"(#version 410 core)"
#else
R"(#version 300 es)"
#endif
R"(
precision mediump float;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec3 vPos;
out vec4 vColor;

void main() {
    gl_Position = vec4(aPos, 1.0);
    vPos = aPos;
    vColor = aColor;
}
)";
    const char frag[] =
#ifdef SDL_API_OpenGL_Core
R"(#version 410 core)"
#else
R"(#version 300 es)"
#endif
R"(
precision mediump float;

in vec3 vPos;
in vec4 vColor;

out vec4 color;

void main() {
    color = vColor;
}
)";
    return gan::err::unwrap(gan::gl::rawMakeShaderProgram({{vert, sizeof(vert)}}, {{frag, sizeof(frag)}}),
        "borderShaderProgram() in FractalPanel.cpp", "Oops");
}

void gan::FractalPanel::draw(const gan::Window& window, bool updateMouse, vec2 mousePos) {
    if (frac != nullptr) {
        frac->tick();
        frac->draw(window);
        if (updateMouse) {
            frac->uMousePos2f(mousePos);
        }
        static GLuint shader = borderShaderProgram();
        glUseProgram(shader);
        glBindVertexArray(borderVB.vao);
        glDrawArrays(GL_TRIANGLES, 0, 24);
    }
}

std::string gan::FractalPanel::checkHealth() {
    return std::format("===== Fractal Panel Health ======\n"
                      "Panel ID    = {}\n{}",
                      panelID,
                      frac->checkHealth());

}

bool gan::FractalPanel::containedWithin(vec2 pos) const {
    if (frac == nullptr) return false;
    if (pos.x >= frac->windowPos.x &&
        pos.y >= frac->windowPos.y)
    {
        return (!(pos.x <= frac->windowPos.x + imguiWindowSize.x &&
                pos.y <= frac->windowPos.y + imguiWindowSize.y) &&
                pos.x <= frac->windowPos.x + frac->windowSize.x &&
                pos.y <= frac->windowPos.y + frac->windowSize.y);
    }
    return false;
}

gan::vec2 gan::FractalPanel::normalizeToFractalPos(vec2 pos, const Window& window) const {
    pos.y = 1 - pos.y;
    pos -= frac->windowPos;
    pos -= vec2{0.5, 0.5};
    pos *= frac->scale;
    pos.x*= static_cast<float>(window.getWidth())/window.getHeight();
    return frac->centerPos + pos;
}

void gan::FractalPanel::reframe(const Window& window, vec2 pos, vec2 size) {
    if (frac != nullptr) {
        frac->windowPos = pos;
        frac->windowSize = size;
        frac->reframe(window);
    }
    genBorder(window, pos, size, {.8f, .8f, .8f, 1.f}, 0.005);
}

void gan::FractalPanel::reframe(const Window& window) {
    if (frac != nullptr) {
        frac->reframe(window);
        genBorder(window, frac->windowPos, frac->windowSize, {.8f, .8f, .8f, 1.f}, 0.005);
    } else {
        genBorder(window, {0.1f, 0.1f}, {0.1f, 0.1f}, {.8f, .8f, .8f, 1.f}, thickness);
    }
}

void gan::FractalPanel::moveFractal(const vec2 delta) const {
    if (!interactedThisFrame) {
        frac->centerPos.x -= delta.x;
        frac->centerPos.y -= delta.y;
        frac->uCenterPos2f();
    }
}

float gan::FractalPanel::getScale() const {
    if (frac == nullptr) return 0.f;
    return frac->scale;
}

void gan::FractalPanel::setScale(const float scale) const {
    if (frac == nullptr) return;
    frac->scale = scale;
    frac->uScale1f();
}

gan::vec2 gan::FractalPanel::getCenterPos() const {
    if (frac == nullptr) return {0.f, 0.f};
    return frac->centerPos;
}

gan::vec2 gan::FractalPanel::getWindowPos() const {
    if (frac == nullptr) return {0.f, 0.f};
    return frac->windowPos;
}

gan::vec2 gan::FractalPanel::getWindowDimensions() const {
    if (frac == nullptr) return vec2{0.f, 0.f};
    return frac->windowSize;
}

void gan::FractalPanel::setCenterPos(vec2 startPos) const {
    if (frac == nullptr) return;
    frac->centerPos = startPos;
    frac->uCenterPos2f();
}

