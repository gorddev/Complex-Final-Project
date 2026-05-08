#pragma once
#include <thread>
#include <vector>
#include <algorithm>

#include "cpu-fractal-fundamental-types/CPUFractalArgument.hpp"
#include "cpu-fractal-fundamental-types/CPUFractalData.hpp"
#include "cpu-fractal-fundamental-types/CPUFractalDrawFunction.hpp"
#include "cpu-fractal-fundamental-types/CPUFractalSurface.hpp"

/* Created by Gordie Novak on 4/16/26.
 * Purpose: 
 * Manages all threads to render multiple threads simultaneously.
 * Intentionally uses std::thread rather than std::jthread because termination of incomplete threads
 * is defined behavior for this class.
 */

namespace gan {

    class FractalMultithreader {
    public:
        FractalMultithreader()
            : numThreads(std::thread::hardware_concurrency()), running(numThreads) {}

        /** Assigns the multithreader to work on a specific function. Returns false if assignment fails.
         * @warning Will stop execution on the current fractal if assigned while not joinable.  */
        void assign(cfr::DrawMethod func, const cfr::FractalData& data, cfr::Surface& surf);

        /** Returns true if the multithreader can be assigned to a new fractal to render */
        [[nodiscard]] bool joinable() const;

        /** Returns the current number of threads utilized */
        [[nodiscard]] size_t threadLimit() const;

    private:
        /** Assigns a thread to do work on the given function.
         * @param b Bool that represents whether the thread is running right now or not
         * @param func The function that assigns a thread to work on
         * @param data Fractal-specific data concerning arguments and position.
         * @param cmd Contains which location and which pixels the function should write to. */
        static void do_work(uint8_t* b, cfr::DrawMethod* func, const cfr::FractalData* data, cfr::DrawCommand cmd);

        u32 numThreads;                     ///< Total number of threads being used by the multithreader.
        std::vector<std::thread> threads;   ///< Collection of each thread.
        std::vector<uint8_t> running;       ///< Whether each thread is running or not.
    };

}