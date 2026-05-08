#include "./GPUFractalPanel.hpp"
#include <print>

#include "gpu_fractaldef.hpp"

constexpr float thickness = 0.005f;

void gan::GPUFractalPanel::genBorder(const Window& window, vec2 pos, vec2 size, vec4 color, const float thickness) {
    const float cx     = (pos.x - 0.5f + size.w / 2.0f) * 2.0f;
    const float cy     = (0.5f - pos.y - size.h / 2.0f) * 2.0f;
    const float w      = size.w * 2.0f;
    const float h      = size.h * 2.0f;
    const float aspect = window.getWidth() / static_cast<float>(window.getHeight());
    const float th     = thickness * 2.0f;
    const float tw     = (thickness * 2.0f) / aspect;
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

    borderVB = VBuffer<CVertex>::make(borderVertices, 24);
}

gan::GPUFractalPanel::GPUFractalPanel()
    : panelID(std::to_string(fractal::getPanelNumber())) {}

bool gan::GPUFractalPanel::embed(const fractal_id id) {
    if (id >= gpu_fractal::totalFractalNum) {
        GAN_AppendLog("GPUFractalPanel::embed()", "Fractal id provided: ", id, " is above total fractal num.");
        return false;
    }
    auto opt = GPUFractal::make_unique(gpu_fractal::fractalInfo[id]);
    if (!opt) {
        GAN_AppendLog("GPUFractalPanel::embed()", "Fractal Failed to generate.");
        return false;
    }
    this->frac = std::move(opt.value());
    return true;
}

void gan::GPUFractalPanel::imguiBegin(const Window& window) {
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

    ImGui::SetWindowPos({frac->windowPos.x * window.getWidth(), frac->windowPos.y * window.getHeight()}, ImGuiCond_Always);
    imguiWindowSize = {ImGui::GetWindowSize().x / window.getWidth(), ImGui::GetWindowSize().y / window.getHeight()};
}

void gan::GPUFractalPanel::imguiBody(const Window& window) {


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

void gan::GPUFractalPanel::displayProperty(const size_t id) const {
    auto& p = frac->properties[id];
    bool update = false;

    ImGui::PushID(id);
    ImGui::Text("%s", p.propertyName);
    switch (p.type) {
    case gpu_fractal::FLOAT:
        update = ImGui::SliderFloat("##p", reinterpret_cast<float*>(&p.data), p.bounds.min, p.bounds.max, "%.6f");
        break;
    case gpu_fractal::VEC2:
        update = ImGui::SliderFloat2("##p", reinterpret_cast<float*>(&p.data), p.bounds.min, p.bounds.max, "%.6f");
        break;
    case gpu_fractal::VEC3:
        update = ImGui::SliderFloat3("##p", reinterpret_cast<float*>(&p.data), p.bounds.min, p.bounds.max, "%.6f");
        break;
    case gpu_fractal::VEC4:
        update = ImGui::SliderFloat4("##p", reinterpret_cast<float*>(&p.data), p.bounds.min, p.bounds.max, "%.6f");
        break;
    case gpu_fractal::INT:
        update = ImGui::SliderInt("##p", reinterpret_cast<int*>(&p.data), p.bounds.min, p.bounds.max);
        break;
    case gpu_fractal::IVEC2:
        update = ImGui::SliderInt2("##p", reinterpret_cast<int*>(&p.data), p.bounds.min, p.bounds.max);
        break;
    case gpu_fractal::IVEC3:
        update = ImGui::SliderInt3("##p", reinterpret_cast<int*>(&p.data), p.bounds.min, p.bounds.max);
        break;
    case gpu_fractal::IVEC4:
        update = ImGui::SliderInt4("##p", reinterpret_cast<int*>(&p.data), p.bounds.min, p.bounds.max);
        break;
    }

    if (update) {
        frac->uProperty(id);
    }

    ImGui::PopID();
}

void gan::GPUFractalPanel::imguiEnd() {
    ImGui::End();
    ImGui::PopID();
}

static GLuint borderShaderProgram() {
    constexpr char vert[] =
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
    constexpr char frag[] =
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
    std::println("Making shader program");
    return gan::err::unwrap(gan::gl::rawMakeShaderProgram({{vert, sizeof(vert)}}, {{frag, sizeof(frag)}}),
        "borderShaderProgram() in FractalPanel.cpp", "Oops");
}

void gan::GPUFractalPanel::draw(const gan::Window& window, bool updateMouse, vec2 mousePos) {
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

std::string gan::GPUFractalPanel::checkHealth() {
    return std::format("===== Fractal Panel Health ======\n"
                      "Panel ID    = {}\n{}",
                      panelID,
                      frac->checkHealth());

}

bool gan::GPUFractalPanel::containedWithin(vec2 m_pos) const {
    if (frac == nullptr) return false;
    if (m_pos.y <= (imguiWindowSize.h+frac->windowPos.y) && m_pos.x <= (imguiWindowSize.w+frac->windowPos.x))
        return false;
    return (m_pos.x >= frac->windowPos.x &&
            m_pos.x <= frac->windowPos.x + frac->windowSize.x &&
            m_pos.y >= frac->windowPos.y &&
            m_pos.y <= frac->windowPos.y + frac->windowSize.y);
}

gan::vec2 gan::GPUFractalPanel::normalizeToFractalPos(const Window& window, vec2 pos) const {
    // 1. Shift by windowPos to get coordinates relative to the panel's top-left (0..1 within panel)
    pos.x -= frac->windowPos.x;
    pos.y += frac->windowPos.y;
    // 3. Shift to (-0.5 .. 0.5) and flip Y (OpenGL Y is up, but window Y is down)
    pos -= vec2{0.5f, 0.5f};
    pos.y *= -1.0f;
    // 4. Scale and adjust for aspect ratio
    pos *= frac->scale;
    pos.x *= (window.getWidth()) / (window.getHeight());
    return frac->centerPos + pos;
}

void gan::GPUFractalPanel::reframe(const Window& window, vec2 pos, vec2 size) {
    if (frac != nullptr) {
        frac->windowPos = pos;
        frac->windowSize = size;
        frac->reframe(window);
    }
    genBorder(window, pos, size, {.8f, .8f, .8f, 1.f}, 0.005);
}

void gan::GPUFractalPanel::reframe(const Window& window) {
    if (frac != nullptr) {
        frac->reframe(window);
        genBorder(window, frac->windowPos, frac->windowSize, {.8f, .8f, .8f, 1.f}, 0.005);
    } else {
        genBorder(window, {0.1f, 0.1f}, {0.1f, 0.1f}, {.8f, .8f, .8f, 1.f}, thickness);
    }
}

void gan::GPUFractalPanel::moveFractal(const Window& window, const Mouse& m) {
    const float aspect_ratio = (window.getWidth())/window.getHeight();
    frac->centerPos.x -= m.getDeltaX()*frac->scale/window.getWidth() * aspect_ratio;
    frac->centerPos.y += m.getDeltaY()*frac->scale/window.getHeight();
    frac->uCenterPos2f();
}

void gan::GPUFractalPanel::zoom(const Window& window, const Mouse& mouse) {
    #ifndef __EMSCRIPTEN__
    constexpr float zoomAmount = -0.1f;
    #else
    constexpr float zoomAmount = -0.3f;
    #endif

    const float oldScale = frac->scale;
    const float zoomFactor = std::exp(mouse.getScrollWheelY() * zoomAmount);
    const float newScale = oldScale * zoomFactor;

    const vec2 mpos = mouse.getPos(); // pixel coords
    const auto winSize = vec2{window.getWidth(), window.getHeight()};

    const vec2 ndc = vec2{mpos.x / winSize.x, mpos.y/winSize.y} * 2.0f - vec2{1.0f, 1.f};

    const vec2 windowPos{frac->windowPos.x, -1.f*frac->windowPos.y};
    const vec2 adjPos = (ndc - windowPos*2.f);
    const vec2 before = frac->centerPos + adjPos* oldScale;

    frac->scale = newScale;
    frac->uScale1f();

    const vec2 after = frac->centerPos + adjPos * newScale;
    vec2 composite = before - after;
    composite.y *= -1.f;

    composite.x *= window.getWidth()/window.getHeight();

    frac->centerPos += composite/2.f;
    frac->uCenterPos2f();
}

void gan::GPUFractalPanel::setCenterPos(vec2 startPos) const {
    if (frac == nullptr) return;
    frac->centerPos = startPos;
    frac->uCenterPos2f();
}

