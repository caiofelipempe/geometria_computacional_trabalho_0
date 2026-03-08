#include <SDL2/SDL.h>
#include <GL/glu.h>
#include <iostream>
#include <CGAL/Simple_cartesian.h>
#include "convex_hull.hpp"
#include "window.hpp"

typedef CGAL::Simple_cartesian<double> K;
typedef K::Point_3 Point_3;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[]) {
    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Polyhedron");

    auto hull = computeConvexHull();

    float angle = 0.0f;

    window.run([&](float deltaTime, Renderer& renderer) {
        renderer.clear();
        renderer.lookAt(100.0f * std::cos(angle), 5.0f, 100.0f * std::sin(angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        renderer.drawPoligon(std::get<0>(hull), std::get<1>(hull));
        angle += 0.01f;
    });

    return 0;
}