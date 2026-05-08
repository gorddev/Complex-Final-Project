#pragma once

#include "cpu-fractal-fundamental-types/CPUFractalArgument.hpp"
#include "cpu-fractal-fundamental-types/CPUFractalDrawFunction.hpp"

namespace gan::cpu_f {

    inline constexpr u32 numFractals = 1;

    /** Used to initialize a CPU Fractal with all require information */
    struct CPUFractalInitializer {
        const char* name{};
        cfr::DrawMethod func{};

        cfr::Argument arguments[cfr::maxArguments]{};
        u32 numArguments{};

        constexpr CPUFractalInitializer() = default;

        constexpr CPUFractalInitializer(
            const char* name,
            cfr::DrawMethod func,
            std::initializer_list<cfr::Argument> arguments = {}
        )
        : name(name), func(func),
            numArguments(std::min(arguments.size(), cfr::maxArguments))
        {
            for (int i = 0; i < this->numArguments; i++) {
                this->arguments[i] = arguments.begin()[i];
            }
        }
    };

    struct pvec2 {
        mpfr::mpreal x, y;
    };


    inline constexpr CPUFractalInitializer CPUFractalData[numFractals] =  {
        {"Mandelbrot", [](const CPUArgument& dat, SDL_Surface* surf, u32 sl, u32 el) {
            for (u32 y = sl; y < el; y++) {
                const u32 row = y*surf->pitch;
                for (u32 x = 0; x < surf->w; x++) {
                    u32 pix = row + x;

                    pvec2 z = {0.0, 0.0};
                    pvec2 c = {dat.x + x*dat.step, dat.y + y*dat.step};

                    for (int i = 0; i < dat.iterations; i++) {
                        mpfr::mpreal x2 = z.x * z.x;
                        mpfr::mpreal y2 = z.y * z.y;
                        z.y = 2.0 * z.x * z.y + c.y;
                        z.x = x2 - y2 + c.x;
                        if (z.x*z.x + z.y*z.y >= 4.0) {
                            reinterpret_cast<u8*>(surf->pixels)[pix] = 255;
                        }
                    }
                }
            }
        }}
    };



}
