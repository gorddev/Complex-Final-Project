#include "Fractal.hpp"
#include <fstream>
#include <string>
#include <utility>



gan::Fractal::Fractal(const GLint uResolution, const GLint uMousePos, const GLint uIterations, const GLint uScale,
                const GLint uStartPos, const GLint uColorCount, const GLint uColors,
                const GLuint vao, const GLuint vbo, const GLuint shader, std::string name)
        :   fractalPos({0,0}),
            scale(1.0),
            iterations(30),
            name(std::move(name)),
            uResolution(uResolution),
            uMousePos(uMousePos),
            uIterations(uIterations),
            uScale(uScale), uStartPos(uStartPos), uColorCount(uColorCount),
            uColors(uColors), vao(vao), vbo(vbo),
            shader(shader)
{
    uNumColors1i();
    uColors3fv();
}

// --------------------------------------------------------- //

static void create_fractal_gl_object(GLuint& vao, GLuint& vbo, GLuint& program, const gan::path& vShd, const gan::path& fShd) {
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
    program = gan::err::unwrap(
        gan::gl::makeShaderProgram({vShd}, {fShd}),
        "Fractal::make()", "Failed to compile shader program.");
}

gan::Fractal gan::Fractal::make(const std::string& name, const gan::path& vertShader, const gan::path& fragShader) {
    // first ensure that SDL has been initialized
    gan::ensure_SDL_init();

    GLuint vao, vbo, program;
    create_fractal_gl_object(vao, vbo, program, vertShader, fragShader);

    return Fractal{
        glGetUniformLocation(program, "uResolution"),
        glGetUniformLocation(program, "uMousePos"),
        glGetUniformLocation(program, "uIterations"),
        glGetUniformLocation(program, "uScale"),
        glGetUniformLocation(program, "uStartPos"),
        glGetUniformLocation(program, "uColorCount"),
        glGetUniformLocation(program, "uColors"),
        vao, vbo, program,
        name
    };
}

std::unique_ptr<gan::Fractal> gan::Fractal::make_unique(const std::string& name, const gan::path& vertShader, const gan::path& fragShader) {
    // first ensure that SDL has been initialized
    ensure_SDL_init();

    GLuint vao, vbo, program;
    create_fractal_gl_object(vao, vbo, program, vertShader, fragShader);

    // ReSharper disable once CppDFAMemoryLeak
    return std::unique_ptr<Fractal>(new Fractal{
        glGetUniformLocation(program, "uResolution"),
        glGetUniformLocation(program, "uMousePos"),
        glGetUniformLocation(program, "uIterations"),
        glGetUniformLocation(program, "uScale"),
        glGetUniformLocation(program, "uStartPos"),
        glGetUniformLocation(program, "uColorCount"),
        glGetUniformLocation(program, "uColors"),
        vao, vbo, program,
        name
    });
}

std::string gan::Fractal::checkHealth() {
    std::string health;

    health += "====== Fractal Health =======\n"
            "Name       = " + name + "\n"
            "Scale      = " + std::to_string(scale) + "\n"
            "startPos   = " + std::format("{{{}, {}}}", fractalPos.x, fractalPos.y) + "\n"
            "iterations = " + std::to_string(iterations) + "\n"
            "numColors  = " + std::to_string(numColors) + "\n"
            "colors[0]  = " + std::format("{{{}, {}, {}}}", colors[0].x, colors[0].y, colors[0].z) + "\n"
            "--------- Uniforms ----------\n";
    auto health_check = [&health](const GLint uniform, const char name[]) {
        if (uniform < 0) {
            health += std::format("!! Uniform \'{}\' not found.\n", name);
        } else health += std::format(":: Uniform \'{}\' found @ index [{}].\n", name, uniform);
    };
    health_check(uResolution, "uResolution");
    health_check(uMousePos, "uMousePos");
    health_check(uIterations, "uIterations");
    health_check(uScale, "uScale");
    health_check(uStartPos, "uStartPos");

    health += "\n";
    return health;
}

void gan::Fractal::uStartPos2f() const {
    if (uStartPos >= 0) {
        glUseProgram(shader);
        glUniform2f(uStartPos, fractalPos.x + pixelPos.x*scale, fractalPos.y - pixelPos.y*scale);
    }
}

void gan::Fractal::uMousePos2f(vec2 selectionPos) const {
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
    std::println("FRACTAL DESTROYED");
    if (vao != 0) {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(shader);
    }
}

gan::Fractal::Fractal(Fractal&& o)
    noexcept : fractalPos({0, 0}),
               scale(1.0),
               iterations(30),
               name(o.name),
               uResolution(o.uResolution), uMousePos(o.uMousePos), uIterations(o.uIterations),
               uScale(o.uScale), uStartPos(o.uStartPos), uColorCount(o.uColorCount),
               uColors(o.uColors), vao(o.vao), vbo(o.vbo),
               shader(o.shader)
{
    o.vao    = 0;
    o.vbo    = 0;
    o.shader = 0;
}

gan::Fractal& gan::Fractal::operator=(Fractal&& o) noexcept{
    this->~Fractal();
    uResolution = o.uResolution;
    uMousePos   = o.uMousePos;
    uIterations = o.uIterations;
    uScale      = o.uScale;
    uStartPos   = o.uStartPos;
    vao         = o.vao;
    vbo         = o.vbo;
    shader      = o.shader;
    scale       = o.scale;
    fractalPos  = o.fractalPos;
    iterations  = o.iterations;
    o.vao       = 0;
    o.vbo       = 0;
    o.shader    = 0;
    return *this;
}


void gan::Fractal::draw(const Window& window, const vec2 selectionPos) {
    glUseProgram(shader);

    if (uResolution >= 0)
        glUniform2f(uResolution, static_cast<float>(window.getWidth()), static_cast<float>(window.getHeight()));
    if (uMousePos >= 0)
        glUniform2f(uMousePos, selectionPos.x, selectionPos.y);
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
    pixelPos  = {pos.x * window.getWidth(), pos.y * window.getHeight()};

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

    uStartPos2f();
}
