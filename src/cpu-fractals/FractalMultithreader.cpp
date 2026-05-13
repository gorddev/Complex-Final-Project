#include "../../include/cpu-fractals/FractalMultithreader.hpp"

using namespace gan;

void FractalMultithreader::assign(cfr::DrawMethod func, const cfr::FractalData& data, cfr::Surface& surf) {
    // Clears all current executions. No check if its currently running (intentional)
    threads.clear();
    const u32 lines_per_thread = surf.w/numThreads;

    for (u32 i = 0; i < numThreads - 1; i++) {
        u8* pix = surf.pixels.data() + (surf.w * i * lines_per_thread);

        threads.emplace_back(do_work, &running[i], &func, &data,
            cfr::DrawCommand{
                .pixels     = pix,
                .start_line = (i * lines_per_thread),
                .end_line   = ((i+1) * lines_per_thread),
                .pitch      = surf.w
            }
        );
    }

    u8* pix = surf.pixels.data() + (surf.w * (numThreads -1) * lines_per_thread);

    threads.emplace_back(do_work, &running[numThreads-1], &func, &data,
        cfr::DrawCommand {
            .pixels     = pix,
            .start_line = ((numThreads - 1)*lines_per_thread),
            .end_line   = (lines_per_thread + surf.w%numThreads),
            .pitch      = surf.w,
        }
    );
}

bool FractalMultithreader::joinable() const {
    return std::ranges::all_of(running, [](uint8_t b) { return !b; });
}

size_t FractalMultithreader::threadLimit() const {
    return numThreads;
}

void FractalMultithreader::do_work(uint8_t* b, cfr::DrawMethod* func, const cfr::FractalData* data, cfr::DrawCommand cmd) {
    *b = true;
    (*func)(*data, cmd);
    *b = false;
}