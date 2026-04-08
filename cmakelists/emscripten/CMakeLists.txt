

target_link_options(Complex_Final_Proj PRIVATE
    # c++ flags
    -Werror
    -Wall
    -O0
    -std=c++23
    -Wno-error=limited-postlink-optimizations
    -DGL_SILENCE_DEPRECATION
    # emscripten flags
    "--shell-file=${CMAKE_SOURCE_DIR}/cmakelists/emscripten/em_shell.html"
    "-sUSE_WEBGL2=1"
    "--preload-file=assets-local"
    "--emrun"
    "-sMAXIMUM_MEMORY=512MB"
    "-sALLOW_MEMORY_GROWTH"
    "-sNO_DISABLE_EXCEPTION_CATCHING"
    "-sGL_ASSERTIONS=1"
    "-sGL_DEBUG=1"
    "-sASSERTIONS=1"

    #"-fwasm-exceptions"
    #"-fsanitize=address"
    #"-fno-omit-frame-pointer"
    #"-sEXPORTED_FUNCTIONS=['_main','_SDL_main']"
)