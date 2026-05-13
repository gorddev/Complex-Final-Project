#pragma once
#include <string>

#include "GPUUniforms.hpp"

namespace gan {

    struct GPUFractalInfo {
        const char* const name;
        const char* const fragShader;
        const char* const vertShader;
        const char* const description;

        gpu_f::Uniform extraUniforms[gpu_f::maxExtraUniforms];
        const uint32_t numExtraUniforms;

        constexpr GPUFractalInfo(const char name[], const char fragShader[], const char desc[] = "",
            const std::initializer_list<gpu_f::Uniform> additionalUniforms = {}, const char vertShader[] = "std.vert")
            : name(name), fragShader(fragShader), vertShader(vertShader), description(desc), extraUniforms{},
              numExtraUniforms(std::min(additionalUniforms.size(), gpu_f::maxExtraUniforms)) {
            int count = 0;
            for (auto& uniform : additionalUniforms) {
                extraUniforms[count++] = uniform;
                if (count == gpu_f::maxExtraUniforms)
                    break;
            }
        }
    };

}
