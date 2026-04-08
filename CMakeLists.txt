cmake_minimum_required(VERSION 4.0)
project(Complex_Final_Proj)

set(CMAKE_CXX_STANDARD 23)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()


add_subdirectory("/Users/gordie/Developer/SDL_API" SDL_API)

add_executable(Complex_Final_Proj
    main.cpp
    fractals/Fractal.hpp
    fractals/Fractal.cpp
    fractals/FractalPanel.cpp
    fractals/gui-classes/ColorSelectorGUI.cpp
    fractals/FractalExplorer.cpp
)

target_link_libraries(Complex_Final_Proj PRIVATE
    SDL_API
    SDL3
)

if(EMSCRIPTEN)
    add_subdirectory("cmakelists/emscripten")
endif()


