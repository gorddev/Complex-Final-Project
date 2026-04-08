#include "FractalPanel.hpp"

#include "core/types/vec.hpp"
#include "core/types/opengl/Vertex.hpp"
#include "core/types/opengl/VBuffer.hpp"
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

    borderVB = VBuffer<CVertex>::make(borderVertices, 24);
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
        if (ImGui::Button("Home")) {
            frac->scale = 1.f;
            frac->fractalPos = {0.0, 0.0};
            frac->uScale1f();
            frac->uStartPos2f();
        }
        if (ImGui::SliderFloat("Scale", &frac->scale, 0.000001, 100.0, "%.6f", ImGuiSliderFlags_Logarithmic)) {
            frac->uIterations1i();
            interactedThisFrame = true;
        }
        ImGui::Text("FractalPos: {%.3f, %.3f}", frac->fractalPos.x, frac->fractalPos.y);

        ImGui::Separator(); //—————————————————

        // ----- Iterations ------
        if (ImGui::SliderInt("Iterations", &frac->iterations, 1, 1000, "%d", ImGuiSliderFlags_Logarithmic)) {
            frac->uIterations1i();
            interactedThisFrame = true;
        }

        ImGui::Separator(); //—————————————————

        // ---- Colors -----
        color_selector.display(frac);

        imguiWindowSize = window.normalizeToWindow({ImGui::GetWindowSize().x, ImGui::GetWindowSize().y});
    }
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

void gan::FractalPanel::draw(const gan::Window& window, const vec2 selectionPos) {
    if (frac != nullptr) {
        frac->draw(window, selectionPos);
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

gan::vec2 gan::FractalPanel::normalizeToPanelPos(vec2 pos, const Window& window) {
    return {(pos.x-frac->pixelPos.x/2)/window.getWidth(), (pos.y-frac->pixelPos.y/2)/window.getHeight()};

}

gan::vec2 gan::FractalPanel::normalizeToFractalPos(vec2 pos, const Window& window) const {
    pos.y = 1 - pos.y;
    pos.x -= frac->fractalPos.x/window.getWidth();
    pos.y -= frac->fractalPos.y/window.getHeight();
    pos.x -= frac->windowPos.x;
    pos.y += frac->windowPos.y;
    pos.x *= frac->scale;
    pos.y *= frac->scale;
    return pos;
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
        frac->fractalPos.x += delta.x;
        frac->fractalPos.y += delta.y;
        frac->uStartPos2f();
    }
}

float gan::FractalPanel::getScale() {
    return frac->scale;
}

void gan::FractalPanel::setScale(float scale) {
    frac->scale = scale;
    frac->uScale1f();
}

gan::vec2 gan::FractalPanel::getStartPos() const {
    return frac->fractalPos;
}

void gan::FractalPanel::setStartPos(vec2 startPos) {
    frac->fractalPos = startPos;
    frac->uStartPos2f();
}

