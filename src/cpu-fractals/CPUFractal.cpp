#include "./CPUFractal.hpp"

using namespace gan;

CPUFractal CPUFractal::make(cfr::DrawMethod func) {



    return CPUFractal(func);
}

CPUFractal::CPUFractal(CPUFractal&& o) noexcept
    : data(o.data), surf(std::move(o.surf)), tex(std::move(o.tex))
{}

CPUFractal& CPUFractal::operator=(CPUFractal&& o) noexcept {
    std::swap(data, o.data);
    std::swap(surf, o.surf);
    std::swap(tex, o.tex);
    return *this;
}

void CPUFractal::embed(cfr::DrawMethod f) {
    func = f;
}

void CPUFractal::beginRender(FractalMultithreader& multithreader) {
    if (multithreader.joinable()) {
        multithreader.assign(func, data, surf);
    }
}

void CPUFractal::updateRender() {
    auto opt = Texture::make_packed(surf.pixels.data(), surf.w, surf.h, GAN_LINEAR, 0, 1, GL_RGBA, GL_R8);
    if (opt) {
        tex = std::move(opt.value());
    }
}

void CPUFractal::resize(int w, int h) {
    surf.resize(w, h);
}

CPUFractal::CPUFractal(cfr::DrawMethod f) : func(f) {
}
