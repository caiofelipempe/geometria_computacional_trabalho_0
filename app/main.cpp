#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>
#include <set>
#include <utility>
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

    std::set<std::pair<unsigned int, unsigned int>> edges;
    auto& vertices = std::get<0>(hull);
    auto& faces = std::get<1>(hull);
    for (const auto& face : faces) {
        for (size_t j = 0; j < face.size(); j++) {
            unsigned int a = face[j], b = face[(j + 1) % face.size()];
            if (a > b) std::swap(a, b);
            edges.insert({a, b});
        }
    }

    float angle = 0.0f;

    window.run([&](float deltaTime, Renderer& renderer) {
        renderer.clear();
        renderer.lookAt(15.0f * std::cos(angle), 15.0f, 15.0f * std::sin(angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        renderer.drawPoligon(vertices, faces, 1.0f, 1.0f, 1.0f, 1.0f);
        for (size_t i = 0; i < vertices.size(); i += 3) {
            glPushMatrix();
            glTranslatef(vertices[i], vertices[i+1], vertices[i+2]);
            renderer.drawSphere(0.1f, 10, 10, 1.0f, 0.0f, 0.0f, 1.0f);
            glPopMatrix();
        }
        for (const auto& edge : edges) {
            unsigned int a = edge.first, b = edge.second;
            float x1 = vertices[3 * a], y1 = vertices[3 * a + 1], z1 = vertices[3 * a + 2];
            float x2 = vertices[3 * b], y2 = vertices[3 * b + 1], z2 = vertices[3 * b + 2];
            float dx = x2 - x1, dy = y2 - y1, dz = z2 - z1;
            float length = sqrt(dx * dx + dy * dy + dz * dz);
            if (length < 1e-6) continue;
            float vx = dx / length, vy = dy / length, vz = dz / length;
            float dot = vz;
            float rot_angle = acos(dot) * 180.0f / M_PI;
            float axis_x = -vy, axis_y = vx, axis_z = 0.0f;
            glPushMatrix();
            glTranslatef(x1, y1, z1);
            glRotatef(rot_angle, axis_x, axis_y, axis_z);
            renderer.drawCylinder(0.02f, 0.02f, length, 8, 1, 0.0f, 0.0f, 1.0f, 1.0f);
            glPopMatrix();
        }
        angle += 0.01f;
    });

    return 0;
}