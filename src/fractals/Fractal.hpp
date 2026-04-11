#pragma once

#include "SDL_API.h"
#include <span>

#include "../fractaldef.hpp"
#include "FractalInfo.hpp"

namespace gan {

    struct Fractal {
        // ~~~~~~~~~ Positioning ~~~~~~~~~~
        vec2 windowPos{};     ///< Where the fractal is positioned in the window. {0.0 to 1.0}
        vec2 windowSize{};    ///< The size of fractal within the window {0.0 to 1.0}
        vec2 centerPos;       ///< Starting position where the fractal starts drawing.
        float scale;          ///< How far zoomed in we are. {0.0001 to 5.0}
        /** Extra uniforms for fractals apart from the pre-provided ones */
        fractal::Uniform properties[fractal::maxExtraUniforms];
        const size_t numProperties;
        // ~~~~~~ Rendering Detail ~~~~~~~~
        int iterations;     ///< How many iterations we perform on the fractal. {2 to 1000}
        // ~~~~~~~~~ Coloring ~~~~~~~~~~~~~
        std::array<vec3, fractal::max_colors> colors = fractal::default_colors;
        int numColors = fractal::defaultNumColors;
        // ~~~~~~~~~~~ Name ~~~~~~~~~~~~~~~
        const std::string name;
        const std::string description;
        std::string healthStr;

        /** Creates a fractal object from the provided shaders.
         * @param info
         * @return A fully made fractal object ready for use.
         * @warning Will throw a @code std::runtime_error()@endcode if creation fails. */
        static std::optional<gan::Fractal> make(FractalInfo info);

        /** Creates a fractal object from the provided shaders.
         * @param info
         * @return A fully made fractal object ready for use.
         * @warning Will throw a @code std::runtime_error()@endcode if creation fails.*/
        static std::optional<std::unique_ptr<gan::Fractal>> make_unique(FractalInfo info);

        // ======================================================== //

        void tick();

        /** Returns a string with fractal health information **/
        std::string checkHealth() const;
        void updateHealth();

        // --- Uniform Updates ---//

        /** Updates the start position of the fractal to the selected value */
        void uCenterPos2f() const;
        /** Updates the internal window dimensions of the fractal. */
        void uWindowDimensions4f() const;
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

        void uProperty(size_t id) const;

    private:
        // Internal uniform locations
        GLint   uResolution,
                uMousePos,
                uIterations,
                uScale,
                uCenterPos,
                uWindowDimensions,
                uColorCount,
                uColors;
        // Extra uniform handles.
        GLint uProperties[fractal::maxExtraUniforms]{};
        // Secret smoothed uniform data.
        fractal::UniformData smoothProperties[fractal::maxExtraUniforms]{};
        // vertex buffer making up the vao
        GLuint vao, vbo;
        GLuint shader;

        Fractal(GLint uResolution, GLint uMousePos, GLint uIterations, GLint uScale,
                GLint uCenterPos, GLint uWindowPos, GLint uColorCount, GLint uColors,
                GLuint vao, GLuint vbo, GLuint shader, std::string name,
                fractal::Uniform extraUniforms[], size_t numExtraUniforms, const char description[]);

        /** Draws the fractal with the window & current mouse position **/
        void draw(const Window& window) const;
        /** Sets the frame of the current fractal to the internal windowPos and windowSize member vars. **/
        void reframe(const Window& window);

        friend class FractalPanel;
    };
}
