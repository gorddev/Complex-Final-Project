#include "./Fractal.hpp"
#include <fstream>
#include <string>
#include <utility>

#include "FractalInfo.hpp"


gan::Fractal::Fractal(const GLint uResolution, const GLint uMousePos, const GLint uIterations, const GLint uScale,
                const GLint uCenterPos, const GLint uWindowPos, const GLint uColorCount, const GLint uColors,
                const GLuint vao, const GLuint vbo, const GLuint shader, std::string name,
                fractal::Uniform extraUniforms[], const size_t numExtraUniforms, const char description[])
        :   centerPos({0,0}),
            scale(fractal::defaultScale),
            properties{},
            numProperties(numExtraUniforms),
            iterations(50),
            name(std::move(name)),
            description(description),
            uResolution(uResolution), uMousePos(uMousePos), uIterations(uIterations),
            uScale(uScale),
            uCenterPos(uCenterPos), uWindowDimensions(uWindowPos), uColorCount(uColorCount),
            uColors(uColors),
            vao(vao), vbo(vbo),
            shader(shader)
{
    uNumColors1i();
    uColors3fv();

    // Get all extra uniforms into the fractal.
    for (int i = 0; i < std::min(numExtraUniforms, fractal::maxExtraUniforms); i++) {
        properties[i] = extraUniforms[i];
        uProperties[i] = glGetUniformLocation(shader, extraUniforms[i].shaderHandle);
        uProperty(i);
    }

    healthStr = checkHealth();
}

// --------------------------------------------------------- //
namespace {
    enum ErrCode {
        SUCCESS,
        FAILURE,
    };
}

ErrCode create_fractal_gl_object(GLuint& vao, GLuint& vbo, GLuint& program, const gan::path& vShd, const gan::path& fShd) {
    // next we create our vertex array;
    constexpr float vertices[] = {
        -1.f,-1.f, 0.f,   -1.f, 1.f, 0.f,  1.f, 1.f, 0.f,
        -1.f,-1.f, 0.f,    1.f,-1.f, 0.f,  1.f, 1.f, 0.0, };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // buffers
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // now time for the shader program;
    auto programOpt = gan::gl::makeShaderProgram({vShd}, {fShd});
    if (!programOpt) {
        return FAILURE;
    }

    program = programOpt.value();
    return SUCCESS;
}

std::optional<gan::Fractal> gan::Fractal::make(FractalInfo info) {
    // first ensure that SDL has been initialized
    gan::ensure_SDL_init();

    GLuint vao, vbo, program;

    const ErrCode e = create_fractal_gl_object(vao, vbo, program, info.vertShader, info.fragShader);

    if (e != SUCCESS) return std::nullopt;

    return Fractal{
        glGetUniformLocation(program, "uResolution"),
        glGetUniformLocation(program, "uMousePos"),
        glGetUniformLocation(program, "uIterations"),
        glGetUniformLocation(program, "uScale"),
        glGetUniformLocation(program, "uCenterPos"),
        glGetUniformLocation(program, "uWindowDimensions"),
        glGetUniformLocation(program, "uColorCount"),
        glGetUniformLocation(program, "uColors"),
        vao, vbo, program,
        info.name,
        info.extraUniforms, info.numExtraUniforms,
        info.description
    };
}

std::optional<std::unique_ptr<gan::Fractal>> gan::Fractal::make_unique(FractalInfo info) {
    // first ensure that SDL has been initialized
    ensure_SDL_init();

    GLuint vao, vbo, program;
    ErrCode e = create_fractal_gl_object(vao, vbo, program, info.vertShader, info.fragShader);

    if (e != SUCCESS) return std::nullopt;
    // ReSharper disable once CppDFAMemoryLeak
    return std::unique_ptr<Fractal>(new Fractal{
        glGetUniformLocation(program, "uResolution"),
        glGetUniformLocation(program, "uMousePos"),
        glGetUniformLocation(program, "uIterations"),
        glGetUniformLocation(program, "uScale"),
        glGetUniformLocation(program, "uCenterPos"),
        glGetUniformLocation(program, "uWindowDimensions"),
        glGetUniformLocation(program, "uColorCount"),
        glGetUniformLocation(program, "uColors"),
        vao, vbo, program,
        info.name,
        info.extraUniforms, info.numExtraUniforms,
        info.description
    });
}

void smooth_move(const gan::fractal::UniformData& data, gan::fractal::UniformData& sendData, float smooth_factor) {
    sendData.x += (data.x - sendData.x) * smooth_factor;
    sendData.y += (data.y - sendData.y) * smooth_factor;
    sendData.z += (data.z - sendData.z) * smooth_factor;
    sendData.w += (data.w - sendData.w) * smooth_factor;
}

void gan::Fractal::tick() {
    for (size_t i = 0; i < numProperties; ++i) {
        if (properties[i].type < fractal::INT) {
            smooth_move(properties[i].data, smoothProperties[i], properties[i].smoothing);
            uProperty(i);
        }
    }
}

std::string gan::Fractal::checkHealth() const {
    std::string health;
    std::string unhealthy;

    auto vtstr = [](const vec2& v) {
        return std::string("{") + std::to_string(v.x) + ", " + std::to_string(v.y) + "}";
    };

    health += "====== Fractal Health =======\n"
            "Name       = " + name + "\n"
            "WinPos     = " + vtstr(windowPos) + "\n"
            "WinSize    = " + vtstr(windowSize) + "\n"
            "CenterPos  = " + vtstr(centerPos) + "\n"
            "Scale      = " + std::to_string(scale) + "\n"
            "NumProps   = " + std::to_string(numProperties) + "\n" +
            std::format("vao, vbo, shd -> {{{}, {}, {}}}\n", vao, vbo, shader) +
            "\n--------- Uniforms ----------\n";
    auto health_check = [&health, &unhealthy](const GLint uniform, const char name[]) {
        if (uniform < 0) {
            unhealthy += std::format("!! Uniform \'{}\' not found.\n", name);
        } else {
            health += std::format(":: Uniform \'{}\' found @ {}\n", name, uniform);
        }
    };
    health_check(uResolution, "uResolution");
    health_check(uMousePos, "uMousePos");
    health_check(uIterations, "uIterations");
    health_check(uScale, "uScale");
    health_check(uCenterPos, "uCenterPos");
    health_check(uWindowDimensions, "uCenterPos");
    health_check(uColorCount, "uColors");
    health_check(uColors, "uColors");

    for (size_t i = 0; i < numProperties; ++i) {
        health_check(uProperties[i], properties[i].shaderHandle);
    }

    if (unhealthy.empty()) {
        health += "All uniforms found.\n";
    } else {
        health += "\n--------- Warnings ----------\n";
        health += unhealthy;
    }

    auto v4tstr = [](const fractal::UniformData& v) {
        return std::string("{") + std::to_string(v.x) + ", " + std::to_string(v.y) + ", "
        + std::to_string(v.z) + ", " + std::to_string(v.w) + "}";
    };

    auto iv4tstr = [](const fractal::UniformData& v) {
        return std::string("{") + std::to_string(v.ix) + ", " + std::to_string(v.iy) + ", "
        + std::to_string(v.iz) + ", " + std::to_string(v.iw) + "}";
    };

    if (numProperties > 0)
        health += "\n~ Additional Property Info ~\n";
    for (size_t i = 0; i < numProperties; ++i) {
        if (properties[i].type <= fractal::VEC4) {
            health += std::format("Prop Name: {}\n"
                                     ":: Exact:  {}\n"
                                     ":: Smooth: {}", properties[i].propertyName,
                                     v4tstr(properties[i].data),
                                     v4tstr(smoothProperties[i]));
        } else {
            health += std::format("Prop Name: {}\n"
                                     ":: Value:  {}\n",
                                     properties[i].propertyName,
                                     iv4tstr(properties[i].data));
        }
    }

    return health;
}

void gan::Fractal::updateHealth() {
    healthStr = checkHealth();
}

void gan::Fractal::uCenterPos2f() const {
    if (uCenterPos >= 0) {
        glUseProgram(shader);
        glUniform2f(uCenterPos, centerPos.x, centerPos.y);
    }
}

void gan::Fractal::uWindowDimensions4f() const {
    if (uWindowDimensions >= 0) {
        glUseProgram(shader);
        glUniform4f(uWindowDimensions, windowPos.x, windowPos.y, windowSize.x, windowSize.y);
    }
}

void gan::Fractal::uMousePos2f(const vec2 selectionPos) const {
    if (uMousePos >= 0) {
        glUseProgram(shader);
        glUniform2f(uMousePos, selectionPos.x, selectionPos.y);
    }
}

void gan::Fractal::uScale1f() const {
    if (uScale >= 0) {
        glUseProgram(shader);
        glUniform1f(uScale, scale);
    }
}

void gan::Fractal::uIterations1i() const {
    if (uIterations >= 0) {
        glUseProgram(shader);
        glUniform1i(uIterations, iterations);
    }
}

void gan::Fractal::uNumColors1i() const {
    if (uColorCount >= 0) {
        glUseProgram(shader);
        glUniform1i(uColorCount, std::min(numColors, 8));
    }
}

void gan::Fractal::uColors3fv() const {
    if (uColors >= 0) {
        glUseProgram(shader);
        glUniform3fv(uColors, 8, reinterpret_cast<const float*>(&colors[0]));
    }
}

gan::Fractal::~Fractal() {
    if (vao != 0) {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(shader);
    }
}

gan::Fractal::Fractal(Fractal&& o)
    noexcept : centerPos({0, 0}),
               scale(1.0), numProperties(o.numProperties),
               iterations(50),
               name(o.name),
               uResolution(o.uResolution), uMousePos(o.uMousePos), uIterations(o.uIterations),
               uScale(o.uScale), uCenterPos(o.uCenterPos), uWindowDimensions(0), uColorCount(o.uColorCount),
               uColors(o.uColors), uProperties{}, vao(o.vao), vbo(o.vbo),
               shader(o.shader) {
    o.vao    = 0;
    o.vbo    = 0;
    o.shader = 0;
    for (int i = 0; i < numProperties; i++) {
        properties[i] = o.properties[i];
    }
}

gan::Fractal& gan::Fractal::operator=(Fractal&& o) noexcept{
    this->~Fractal();
    uResolution = o.uResolution;
    uMousePos   = o.uMousePos;
    uIterations = o.uIterations;
    uScale      = o.uScale;
    uCenterPos   = o.uCenterPos;
    vao         = o.vao;
    vbo         = o.vbo;
    shader      = o.shader;
    scale       = o.scale;
    centerPos  = o.centerPos;
    iterations  = o.iterations;
    for (int i = 0; i < numProperties; i++) {
        properties[i] = o.properties[i];
    }
    *const_cast<size_t*>(&numProperties) = o.numProperties;
    o.vao       = 0;
    o.vbo       = 0;
    o.shader    = 0;
    return *this;
}

void gan::Fractal::uProperty(const size_t id) const {
    if (id < numProperties && uProperties[id] >= 0) {
        const fractal::Uniform& i = properties[id];
        const fractal::UniformData& s = smoothProperties[id];

        glUseProgram(shader);
        switch (properties[id].type) {
        case fractal::INT:
            glUniform1i(uProperties[id], i.data.ix);
            break;
        case fractal::IVEC2:
            glUniform2i(uProperties[id], i.data.ix, i.data.iy);
            break;
        case fractal::IVEC3:
            glUniform3i(uProperties[id], i.data.ix, i.data.iy, i.data.iz);
            break;
        case fractal::IVEC4:
            glUniform4i(uProperties[id], i.data.ix, i.data.iy, i.data.iz, i.data.iw);
            break;
        case fractal::FLOAT:
            glUniform1f(uProperties[id], s.x);
            break;
        case fractal::VEC2:
            glUniform2f(uProperties[id], s.x, s.y);
            break;
        case fractal::VEC3:
            glUniform3f(uProperties[id], s.x, s.y, s.z);
            break;
        case fractal::VEC4:
            glUniform4f(uProperties[id], s.x, s.y, s.z, s.w);
            break;
        default:
            std::string err = "Got invalid enum in uProperty() function Fractal: ";
            err += std::to_string(static_cast<int>(properties[id].type));
            throw std::runtime_error(err);
            break;
        }
    }
}


void gan::Fractal::draw(const Window& window) const {
    glUseProgram(shader);

    if (uResolution >= 0)
        glUniform2f(uResolution, window.getWidth()*window.getDPIScale(), window.getHeight()*window.getDPIScale());
    if (uIterations >= 0)
        glUniform1i(uIterations, iterations);
    if (uScale >= 0)
        glUniform1f(uScale, scale);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void gan::Fractal::reframe(const Window& window)  {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glUseProgram(shader);

    vec2 pos  = windowPos;
    vec2 size = windowSize;
    pos.x -= 0.5;
    pos.x *= 2;
    pos.y = 1 - pos.y;
    pos.y -= 0.5;
    pos.y *= 2;

    size.w *= 2;
    size.h *= 2;

    const vec2 tl = {pos.x, pos.y};        // top left
    const vec2 bl = {tl.x, tl.y - size.h}; // bottom left
    const vec2 br = {tl.x + size.w, bl.y}; // bottom right
    const vec2 tr = {br.x, tl.y};          // top right

    const float vertices[] = {
        tl.x, tl.y, 0.0,   bl.x, bl.y, 0.0,   br.x, br.y, 0.0,
        tl.x, tl.y, 0.0,   tr.x, tr.y, 0.0,   br.x, br.y, 0.0,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(0);

    uCenterPos2f();
    uWindowDimensions4f();
}
