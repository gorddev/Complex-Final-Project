#pragma once
#include <vector>
#include <SDL_API.h>

namespace gan::cfr {
    /** Surface the CPU Renderer draws to */
    struct Surface {
        std::vector<u8> pixels; ///< Each u8 represents one pixel. Yes: it is only one color channel per pixel.
        u32 w, h;

        void resize(u32 width, u32 height) {
            this->w = width;
            this->h = height;
            this->pixels.resize(width * height);
        }
    };
}