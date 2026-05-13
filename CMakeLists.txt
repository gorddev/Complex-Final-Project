cmake_minimum_required(VERSION 4.0)

project(fractal)

set(CMAKE_CXX_STANDARD 23)


if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

# Add the SDL_API we use for all sorts of convenient stuff
add_subdirectory("/Users/gordie/Developer/SDL_API" SDL_API)

if (NOT EMSCRIPTEN)
    add_subdirectory("external/boost" SYSTEM)
endif()

add_executable(fractal
    main.cpp
    include/gpu-fractals/GPUFractal.hpp
    src/gpu-fractals/GPUFractal.cpp
    src/gpu-fractals/GPUFractalPanel.cpp
    src/gui/ColorSelectorGUI.cpp
    src/FractalExplorer.cpp
    src/cpu-fractals/CPUFractal.cpp
    src/cpu-fractals/FractalMultithreader.cpp
    src/gui/FractalCursor.cpp
)

target_include_directories(fractal PRIVATE
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/include
    #${MPFR_INCLUDE_DIR}
)

if (NOT EMSCRIPTEN)
    target_include_directories(fractal PRIVATE SYSTEM
        ${CMAKE_SOURCE_DIR}/external/boost/include
    )
    target_link_libraries(fractal PRIVATE
        Boost::multiprecision
    )
endif()



find_package(SDL3)

target_link_libraries(fractal
    PUBLIC
    SDL_API
)


target_compile_options(fractal PRIVATE
    -O0
    -ffast-math
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -flto
    -g0
)


target_link_options(fractal PRIVATE
    -flto
    -ffast-math
    -g0
)

if(EMSCRIPTEN)
    add_subdirectory("cmakelists/emscripten")
endif()


