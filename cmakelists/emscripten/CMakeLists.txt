

target_link_options(fractal PRIVATE
    -Werror
    -Wall
    -O0
    -std=c++23
    -Wno-error=limited-postlink-optimizations
    -DGL_SILENCE_DEPRECATION
    "--shell-file=${CMAKE_SOURCE_DIR}/cmakelists/emscripten/em_shell.html"
    "-sUSE_WEBGL2=1"
    "--preload-file=shaders"
    "--emrun"
    "-pthread"
    "-Wno-experimental"

    #"-sDISABLE_EXCEPTION_CATCHING=1"
    #"-sGL_ASSERTIONS=1"
    #"-sGL_DEBUG=1"
    #"-sASSERTIONS=1"
    #"-fwasm-exceptions"
    #"-fsanitize=address"
    #"-fno-omit-frame-pointer"
    #"-sEXPORTED_FUNCTIONS=['_main','_SDL_main']"
)