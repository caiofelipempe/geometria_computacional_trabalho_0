#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>
#include <vector>
#include <cstdint>
#include <functional>
#include "renderer.hpp"

class Window {
private:
    SDL_Window* window;
    SDL_GLContext context;

    void swapBuffers();
    bool pollEvents();

public:
    Window(int width, int height, const char* title);
    ~Window();
    void run(std::function<void(float, Renderer&)> update);
};

#endif // WINDOW_HPP