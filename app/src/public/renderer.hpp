#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SDL2/SDL.h>
#include <vector>
#include <cstdint>

void initSDL();
SDL_Window* createWindow(int width, int height, const char* title);
void swapBuffers(SDL_Window* window);
bool pollEvents();
void cleanup(SDL_Window* window);
void drawPolygon(std::vector<float> hullVertices, std::vector<std::vector<unsigned int>> hullFaces);

#endif // RENDERER_HPP