#pragma once

#include <cstdint>

namespace gan::gpu_f {
    // Determines how many additional parameters any given gpu-fractal can have
    inline constexpr size_t maxExtraUniforms = 3;

    // The type of uniform the user wants to upload.
    enum UniformType {
        FLOAT, VEC2, VEC3, VEC4,
        INT, IVEC2, IVEC3, IVEC4,
    };

    // Contains the data for the uniform.
    struct UniformData {
        union { float x; int ix; };
        union { float y; int iy; };
        union { float z; int iz; };
        union { float w; int iw; };

        constexpr UniformData() = default;
        explicit constexpr UniformData(float x, float y = 0, float z = 0, float w = 0)
            : x(x), y(y), z(z), w(w) {}
        explicit constexpr UniformData(int x, int y = 0, int z = 0, int w = 0)
            : ix(x), iy(y), iz(z), iw(w) {}
    };

    // The maximum and minimum any given uniform can have.
    struct UniformBounds {
        float min, max;
    };

    struct Uniform {
        UniformData data{};
        UniformBounds bounds{};

        const char* propertyName{};
        const char* shaderHandle{};

        float smoothing = 0.05f;

        UniformType type{};

        constexpr Uniform() = default;
        constexpr Uniform(const char name[], const char shaderHandle[],
            const UniformType type, const UniformBounds bounds = {-100.f, 100.f}, const UniformData defaultData = {}, const float smoothing = 0.05f)
            : data(defaultData), bounds(bounds), propertyName(name), shaderHandle(shaderHandle), smoothing(smoothing), type(type) {}
    };

}