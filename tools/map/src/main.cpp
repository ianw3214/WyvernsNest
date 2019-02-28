#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <vector>
#include <string>
#include <stdexcept>

#include "platform/opengl/opengl.hpp"
#include <SDL2/SDL_opengl.h>

struct Input {
    bool mouse_click;
    bool mouse_release;
    bool mouse_held;
};

namespace SDL2 {

    struct WinData {
        // Variables set by SDL
        SDL_Window * window;
        SDL_GLContext context;

        std::string name;
        int width;
        int height;
    };

    void init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            throw new std::runtime_error("Could not initialize SDL2");
        }
    }

    void createWindow(WinData& data) {
        data.window = SDL_CreateWindow(data.name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, data.width, data.height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
        if (data.window == nullptr) throw new std::runtime_error("Could not create window");
        data.context = SDL_GL_CreateContext(data.window);
        if (data.context == NULL) throw new std::runtime_error("Could not create openGL context");
    }

    void fillInput(Input& input) {
        input.mouse_click = false;
        input.mouse_release = false;
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                exit(0);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                input.mouse_click = true;
            }
            if (e.type == SDL_MOUSEBUTTONUP) {
                input.mouse_click = false;
            }
        }
    }
};

int main(int argc, char* argv[]) {

    SDL2::init();

    SDL2::WinData window;
    window.name = "Map Editor";
    window.width = 1280;
    window.height = 720;
    SDL2::createWindow(window);

    GL::init();

    while (true) {
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        Input input;
        SDL2::fillInput(input);

        SDL_GL_SwapWindow(window.window);

    }

    return 0;

}