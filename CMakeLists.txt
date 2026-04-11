cmake_minimum_required(VERSION 4.0)
project(Fractal_Visualizer)

set(CMAKE_CXX_STANDARD 23)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()


add_subdirectory("/Users/gordie/Developer/SDL_API" SDL_API)

add_executable(Fractal
    main.cpp
    src/fractals/Fractal.hpp
    src/fractals/Fractal.cpp
    src/fractals/FractalPanel.cpp
    src/gui-classes/ColorSelectorGUI.cpp
    src/FractalExplorer.cpp
)

target_link_libraries(Fractal PRIVATE
    SDL_API
    SDL3
)

#[[
target_compile_options(Fractal PRIVATE
    -Oz
)#]]

#[[
target_link_options(Fractal PRIVATE
    -fsanitize=address
    -fsanitize=undefined
)#]]

if(EMSCRIPTEN)
    add_subdirectory("cmakelists/emscripten")
endif()


