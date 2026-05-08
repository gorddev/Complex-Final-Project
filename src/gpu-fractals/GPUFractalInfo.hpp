#pragma once
#include <string>

namespace gan {

    namespace gpu_fractal {
        inline constexpr size_t maxExtraUniforms = 3;

        enum UniformType {
            FLOAT,
            VEC2,
            VEC3,
            VEC4,
            INT,
            IVEC2,
            IVEC3,
            IVEC4,
        };

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

    struct GPUFractalInfo {
        const char* const name;
        const char* const fragShader;
        const char* const vertShader;
        const char* const description;

        gpu_fractal::Uniform extraUniforms[gpu_fractal::maxExtraUniforms];
        const uint32_t numExtraUniforms;

        constexpr GPUFractalInfo(const char name[], const char fragShader[], const char desc[] = "",
            const std::initializer_list<gpu_fractal::Uniform> additionalUniforms = {}, const char vertShader[] = "std.vert")
            : name(name), fragShader(fragShader), vertShader(vertShader), description(desc), extraUniforms{},
              numExtraUniforms(std::min(additionalUniforms.size(), gpu_fractal::maxExtraUniforms)) {
            int count = 0;
            for (auto& uniform : additionalUniforms) {
                extraUniforms[count++] = uniform;
                if (count == gpu_fractal::maxExtraUniforms)
                    break;
            }
        }
    };

}