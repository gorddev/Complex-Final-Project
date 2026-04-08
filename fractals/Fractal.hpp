#pragma once

#include "SDL_API.h"
#include <span>

#include "FractalConstants.hpp"

namespace gan {

    struct Fractal {
        // ~~~~~~~~~ Positioning ~~~~~~~~~~
        vec2 windowPos{};     ///< Where the fractal is positioned in the window. {0.0 to 1.0}
        vec2 windowSize{};    ///< The size of fractal within the window {0.0 to 1.0}
        vec2 pixelPos{};      ///< The positon of the fractal in the window in pixels.
        vec2 fractalPos;    ///< Starting position where the fractal starts drawing.
        float scale;        ///< How far zoomed in we are. {0.0001 to 5.0}
        // ~~~~~~ Rendering Detail ~~~~~~~~
        int iterations;     ///< How many iterations we perform on the fractal. {2 to 1000}
        // ~~~~~~~~~ Coloring ~~~~~~~~~~~~~
        std::array<vec3, 8> colors = fractal::default_colors;
        int numColors = fractal::default_num_colors;
        // ~~~~~~~~~~~ Name ~~~~~~~~~~~~~~~
        const std::string name;

        /** Creates a fractal object from the provided shaders.
         * @param name
         * @param vertShader The vertex shader that represents the fractal (should usually just be pretty simple)
         * @param fragShader The path to the fragment shader you want to compile.
         * @return A fully made fractal object ready for use.
         * @warning Will throw a @code std::runtime_error()@endcode if creation fails. */
        static Fractal make(const std::string& name, const gan::path& vertShader, const gan::path& fragShader);

        /** Creates a fractal object from the provided shaders.
         * @param name
         * @param vertShader The vertex shader that represents the fractal (should usually just be pretty simple)
         * @param fragShader The path to the fragment shader you want to compile.
         * @return A fully made fractal object ready for use.
         * @warning Will throw a @code std::runtime_error()@endcode if creation fails.*/
        static std::unique_ptr<Fractal> make_unique(const std::string& name, const gan::path& vertShader, const gan::path& fragShader);

        // ======================================================== //

        /** Returns a string with fractal health information **/
        std::string checkHealth();

        // --- Uniform Updates ---//

        /** Updates the start position of the fractal to the selected value */
        void uStartPos2f() const;
        /** Updates the selection position of the fractal to the provided value */
        void uMousePos2f(vec2) const;
        /** Updates the scale of the fractal with the internal scale vlaue */
        void uScale1f() const;
        /** Updates the number of iterations of the fractal with the internal iteration number */
        void uIterations1i() const;
        /** Updates the number of colors in the fractal */
        void uNumColors1i() const;
        /** Updates specific colors within the fractal */
        void uColors3fv() const;


        ~Fractal();

        Fractal(const Fractal&) = delete;
        Fractal& operator=(const Fractal&) = delete;
        Fractal(Fractal&&) noexcept;
        Fractal& operator=(Fractal&&) noexcept;
    private:
        // Internal uniform locations
        GLint   uResolution,
                uMousePos,
                uIterations,
                uScale,
                uStartPos,
                uColorCount,
                uColors;
        // vertex buffer making up the vao
        GLuint vao, vbo;
        GLuint shader;

        Fractal(GLint uResolution, GLint uMousePos, GLint uIterations, GLint uScale,
                GLint uStartPos, GLint uColorCount, GLint uColors,
                GLuint vao, GLuint vbo, GLuint shader, std::string  name);

        /** Draws the fractal with the window & current mouse position **/
        void draw(const Window& window, vec2 selectionPos);
        /** Sets the frame of the current fractal to the internal windowPos and windowSize member vars. **/
        void reframe(const Window& window);

        friend class FractalPanel;
    };
}
