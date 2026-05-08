#include <FractalExplorer.hpp>


#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

gan::Window win;
gan::Mouse mouse;

gan::FractalExplorer explorer;
ImGuiContext* imgui_context;

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

EM_BOOL on_browser_resize([[maybe_unused]] int eventType, [[maybe_unused]] const EmscriptenUiEvent *uiEvent,[[maybe_unused]] void *userData) {
    double w, h;
    // Get the new CSS size of the browser window/container
    emscripten_get_element_css_size("canvas", &w, &h);

    // Manually update SDL's window size, which will trigger SDL_EVENT_WINDOW_RESIZED
    win.setDimensions({static_cast<int>(w), static_cast<int>(h)});

    return EM_TRUE;
}
#endif

SDL_AppResult SDL_AppInit(void**, int, [[maybe_unused]] char* argv[]) {
    // First, we create the window through which we view the program.
    win = gan::Window::make("Fractal Visualizer", {800, 800}
    #ifndef MY_FUN_VAR
    , gan::WindowHighDPI | gan::WindowFloatOnTop //, If we aren't on the web, use highDPI display.
    #endif
    );
    win.setResizable(true);
    win.setGLClearColor({0, 0, 0, 255});

    // If on the web, set a callback for browser window resizin.
    #ifndef __EMSCRIPTEN__
    gan::files::initialize_filesystem(argv[0]);
    #else
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, win, EM_FALSE, on_browser_resize);
    on_browser_resize(0, nullptr, nullptr);
    // Set the assets folder we will use for our shaders.
    #endif
    gan::files::set_assets_folder("shaders");

    // create an imgui context with custom wrappers.
    imgui_context = gan::imgui::create_context(win);
    // add the font we'll use in the web browser.
    ImGui::GetIO().Fonts->AddFontFromFileTTF(
        (gan::files::assets()/"JetbrainsMono.ttf").c_str(),
                18.f);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void*) {
    // Clear the screen ach frame.
    glClear(GL_COLOR_BUFFER_BIT);

    // Start new imgui frame.
    gan::imgui::new_frame(imgui_context);

    glViewport(0, 0, win.getWidth()*win.getDPIScale(), win.getHeight()*win.getDPIScale());

    // Display the fractals & GUI
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
        mouse.updateWheelWithSDLEvent(e);
        explorer.onMouseWheel(win, mouse);
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
    case SDL_EVENT_MOUSE_MOTION:
        mouse.updateWithSDL();
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