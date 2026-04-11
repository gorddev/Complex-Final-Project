cmake_minimum_required(VERSION 4.0)
project(Complex_Final_Proj)

set(CMAKE_CXX_STANDARD 23)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()


add_subdirectory("/Users/gordie/Developer/SDL_API" SDL_API)

add_executable(Complex_Final_Proj
    main.cpp
    src/fractals/Fractal.hpp
    src/fractals/Fractal.cpp
    src/fractals/FractalPanel.cpp
    src/gui-classes/ColorSelectorGUI.cpp
    src/FractalExplorer.cpp
)

target_link_libraries(Complex_Final_Proj PRIVATE
    SDL_API
    SDL3
)

target_compile_options(Complex_Final_Proj PRIVATE
    -O3
)

if(EMSCRIPTEN)
    add_subdirectory("cmakelists/emscripten")
endif()


