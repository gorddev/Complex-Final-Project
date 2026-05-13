# Fractal Visualizer

A moderately-performant, real-time fractal visualizer built with `C++23` (my beloved) and OpenGL/WebGL. 

try it out at [gordienovak.com](https://gordienovak.com/)

### Feeeetures :

- gpu rendering of fractals with side-by-side comparison 
  (e.g. mandelbrot set & julia set -› see connected/non-connected behavior)
- GUI that's slightly hostile but gets the job done
- built-in support for cross-compiling to local machine code _and_ the web.



### dependencies:
I used a lot of libraries that made this project waaaay easier:
  - `SDL3` as the window provider and OpenGL context-handler
  - `GLAD` for OpenGL function loading. 
  - `Dear ImGui` for the user interface. 
  - `Boost` for multiprecision floats (*currently unused, it's a  WIP*)
  - `SDL_API` a custom library i wrote that handles a lot of the behind-the-scenes SDL gruntwork. currently closed-source
  - `fstring` custom stack-allocated string library i wrote in `C++` for low-cost debugging & simple type conversion to UTF8.
    - uses `#include <charconv>` for type conversions. 

### languaguages i used:
- `C++23` for the core of the project. you should totally try c++23 it's so good if you hate yourself. 
- `GLSL` for the GPU-side rendering.
- `CMake Language` for build system builder.
- `HTML5` for web-cross compilation shell-file
- `JavaScript` because someone decided it was the best language for the web. 
- `WASM (WebAssembly)` as the compilation target for `C++` code. 

### tools/compilers i used:
- `Apple Clang 17` and `gcc-15` for the local c++23 compilers. its usually good to have two
- `homebrew` as the package manager for mac
- `cmake` for building because `Makefiles` are scary
  - `Ninja` as the generator
  - `emcmake` as the toolchain specifier for `cmake` for setting up build environments
  - `emmake` as the dependency installer for `emscripten` libraries.
- `emscripten SDK` for cross-compiling to WASM. 
- `CLion` as the primary IDE. `Visual Studio Code` or `Zed` for smaller tasks and testing. 
- `fsanitize=undefined,address` for memory leak debugging.

## Project Philosophy
here's my general approach to programming this project in general:
- **compile-time work**: Utilize the flexibility of `C++23 constexpr/consteval/constinit` to create 
very low startup times by offloading work to the compile-time interpreter.


- **polymorphic fractal behavior**: Make Fractal classes polymorphic such that it is easier to add more rendering styles (like GPU & CPU)


- **interface inheritance**: inhertance should *only* occur with abstract base classes that contain no member variables. 


- **input flow (single responsibility principle \#1)**: input is propagates clearly through a chain of command: `FractalExplorer` distributes input to appropriate `FractalPanels`,
yet `FractalPanels` decide how to interpret that input. 


- **data responsibility (single responsibility principle \#2)**: Every class can either be in control of raw data, or in control of other classes that manage raw data. For example,the `FractalCursor` class only exists within
a `FractalExplorer`, so making a separate class seems silly. However, it isolates cursor-related logic from the main logic loop of `FractalExplorer` to reduce coupling.


- **inplace-allocation**: If something can be allocated locally without another heap allocation, it should be done to reduce heap-fragmentation, especially with WASM.
This is achieved through the beauty of `C`-style `union` structs. Although unsafe, they are tagged
with `enum`s and can skip over checks typically required of `std::variant<Ts...>`. Additionally, by storing everything in
local `std::array`s or `C`-style arrays with maximum limit caps, we can make reasonable estimates about the scope of the program
while using about only ~500 bytes of memory per fractal. That's about `0.00005%` of a gigabyte.


- **documentation**: hahaha im just joking


- **move-semantics** use `C++11` move semantics where-ever possible to reduce use-after-free dangers. this includes `std::unique_ptr<T>`.  


- there's more but im lazy


## Build Instructions:
You can't build it yourself right now because it utilizes an API I wrote that's not done yet. (See the absolute path in CmakeLists.txt).

uhhh i'll fix it later. 
