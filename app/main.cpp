#include <SDL2/SDL.h>
#include <GL/glu.h>
#include <iostream>
#include <CGAL/Simple_cartesian.h>
#include "convex_hull.hpp"
#include "src/public/renderer.hpp"

typedef CGAL::Simple_cartesian<double> K;
typedef K::Point_3 Point_3;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[]) {
    initSDL();
    SDL_Window* window = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Polyhedron");

    auto hull = computeConvexHull();

    float angle = 0.0f;

    while (pollEvents()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Camera rotation around the cube
        gluLookAt(50.0f * cos(angle), 30.0f, 50.0f * sin(angle),  // eye
                  0.0f, 0.0f, 0.0f,                          // center
                  0.0f, 1.0f, 0.0f);                         // up

        drawPolygon(std::get<0>(hull), std::get<1>(hull));

        swapBuffers(window);

        angle += 0.01f;  // Rotate slowly
        SDL_Delay(16);   // ~60 FPS
    }

    cleanup(window);

    return 0;
}