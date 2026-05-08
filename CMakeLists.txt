cmake_minimum_required(VERSION 4.0)

project(Fractal_Visualizer)

set(CMAKE_CXX_STANDARD 23)


if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

# Add the SDL_API we use for all sorts of convenient stuff
add_subdirectory("/Users/gordie/Developer/SDL_API" SDL_API)

if (NOT EMSCRIPTEN)
    add_subdirectory("external/boost" SYSTEM)
endif()

add_executable(Fractal_Visualizer
    main.cpp
    src/gpu-fractals/GPUFractal.hpp
    src/gpu-fractals/GPUFractal.cpp
    src/gpu-fractals/GPUFractalPanel.cpp
    src/gui-classes/ColorSelectorGUI.cpp
    src/FractalExplorer.cpp
    src/cpu-fractals/CPUFractal.cpp
    src/cpu-fractals/FractalMultithreader.cpp
)

target_include_directories(Fractal_Visualizer PRIVATE
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/src
    #${MPFR_INCLUDE_DIR}
)

if (NOT EMSCRIPTEN)
    target_include_directories(Fractal_Visualizer PRIVATE SYSTEM
        ${CMAKE_SOURCE_DIR}/external/boost/include
    )
    target_link_libraries(Fractal_Visualizer PRIVATE
        Boost::multiprecision
    )
endif()



find_package(SDL3)

target_link_libraries(Fractal_Visualizer
    PUBLIC
    SDL_API
)


target_compile_options(Fractal_Visualizer PRIVATE
    -O3
    -ffast-math
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -flto
    -g0
)


target_link_options(Fractal_Visualizer PRIVATE
    -flto
    -ffast-math
    -g0
)

if(EMSCRIPTEN)
    add_subdirectory("cmakelists/emscripten")
endif()


