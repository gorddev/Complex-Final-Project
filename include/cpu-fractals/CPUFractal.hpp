#pragma once

#include "SDL_API.h"

#include "FractalMultithreader.hpp"

/* Created by Gordie Novak on 4/13/26.
 * Purpose: 
 * CPU-Renders a specific fractal. */

namespace gan {

    /** CPU Fractal represents a single instance of a CPU fractal being drawn. It contains:
     * - A @code FractalData@endcode object, which includes*/
    struct CPUFractal {
        cfr::FractalData data{};        ///< Contains all information needed to render this CPU Fractal.
        cfr::Surface surf{};            ///< Dynamic array of pixels that represents the fractal texture
        VBuffer<Vertex> buffer;         ///< OpenGL triangles the CPUFractal draws to.
        GLint shader;                   ///< Shader program each CPUFractal uses.
        Texture tex{};                  ///< OpenGL texture this fractal is bound to.

        cfr::DrawMethod func = nullptr;

    // *************** Constructors ****************** //

        static CPUFractal make(cfr::DrawMethod);

        CPUFractal(const CPUFractal&)            = delete;
        CPUFractal& operator=(const CPUFractal&) = delete;

        CPUFractal(CPUFractal&&) noexcept;
        CPUFractal& operator=(CPUFractal&&) noexcept;

    // **************** Binding/Assignment ************ //

        void embed(cfr::DrawMethod);
        void beginRender(FractalMultithreader& multithreader);

        void updateRender();

        void resize(int w, int h);

        std::string checkHealth();

    private:
        CPUFractal() = default;
        CPUFractal(cfr::DrawMethod);
    };

}




