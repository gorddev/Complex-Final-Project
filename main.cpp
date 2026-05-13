


#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <FractalExplorer.hpp>

#include "include/gui/ImGuiSetup.hpp"

gan::Window win;
gan::Mouse mouse;

gan::FractalExplorer explorer;
ImGuiContext* imgui_context;

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

EM_BOOL on_browser_resize([[maybe_unused]] int eventType, [[maybe_unused]] const EmscriptenUiEvent *uiEvent,[[maybe_unused]] void *userData) {
    // Get the new CSS size of the browser window/container
    double css_w, css_h;
    emscripten_get_element_css_size("canvas", &css_w, &css_h);

    double dpr = emscripten_get_device_pixel_ratio();

    std::cerr << "on_browser_resize:" << std::endl;
    std::cerr << "  CSS: " << css_w << "x" << css_h << std::endl;
    std::cerr << "  DPR: " << dpr << std::endl;

    emscripten_set_canvas_element_size(
        "canvas",
        (int)(css_w * dpr),
        (int)(css_h * dpr)
    );

    win.setDimensions({(int)css_w, (int)css_h});

    std::cerr <<"dpr" << dpr << std::endl;

    return EM_TRUE;
}
#endif

SDL_AppResult SDL_AppInit(void**, int, [[maybe_unused]] char* argv[]) {
    // First, we create the window through which we view the program.
    win = gan::Window::make("Fractal Visualizer", {800, 800},
        gan::WindowHighDPI | gan::WindowFloatOnTop | gan::WindowResizable
    );
    win.setGLClearColor({0, 0, 0, 255});

    // If on the web, set a callback for browser winow resizing.
    #ifndef __EMSCRIPTEN__
    gan::files::initialize_filesystem();
    #else
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, win, EM_FALSE, on_browser_resize);
    on_browser_resize(0, nullptr, nullptr);
    // Set the assets folder we will use for our shaders.
    #endif
    gan::files::set_assets_folder("shaders");

    // create an imgui context with custom wrappers.
    imgui_context = gan::imgui::create_context(win);

    gan::SetupImGuiStyle();
    // add the font we'll use in the web browser.
    ImGui::GetIO().Fonts->AddFontFromFileTTF(
        (gan::files::assets()/"JetbrainsMono.ttf").c_str(),
                16.f);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void*) {
    // Clear the screen ach frame.
    glClear(GL_COLOR_BUFFER_BIT);

    // Start new imgui frame.
    gan::imgui::new_frame(imgui_context);
    const gan::dim2 winSize = win.getWindowPixelSize();
    glViewport(0, 0, winSize.x, winSize.y);

    // Display the fractals & GU
    explorer.display(win, mouse.getPos());
    gan::imgui::render(imgui_context);

    SDL_GL_SwapWindow(win);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void*, SDL_Event* event) {
    SDL_Event& e = *event;

    switch (e.type) {
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
        break;
    case SDL_EVENT_MOUSE_WHEEL:
        mouse.onSDLMouseWheel(e);
        explorer.onMouseWheel(win, mouse);
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        mouse.onSDLMouseButtonDown(e);
        explorer.onMouseDown(win, mouse);
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        mouse.onSDLMouseButtonUp(e);
        explorer.onMouseRelease(win, mouse);
        break;
    case SDL_EVENT_MOUSE_MOTION:
        mouse.onSDLMouseMotion(e);
        explorer.onMouseMotion(win, mouse);
        break;
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
    case SDL_EVENT_WINDOW_RESIZED:
        win.on_SDLWindowEvent(e);
        explorer.reorganize(win);
        break;
    case SDL_EVENT_KEY_DOWN:
        if (e.key.scancode == SDL_SCANCODE_RETURN) {
            explorer.toggleCursorFreeze();
        }
    default:
        break;
    }
    ImGui_ImplSDL3_ProcessEvent(&e);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit([[maybe_unused]] void* appstate, [[maybe_unused]] SDL_AppResult result) {
    gan::imgui::shutdown(imgui_context);
}