#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>
#include <set>
#include <utility>
#include <CGAL/Simple_cartesian.h>
#include <random>
#include "convex_hull_compute.hpp"
#include "window.hpp"

typedef CGAL::Simple_cartesian<double> K;
typedef K::Point_3 Point_3;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[]) {
    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Trabalho 001 - CGAL Convex Hull");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-5.0, 5.0);

    std::vector<Point_3> randomPoints;
    for (int i = 0; i < 50; ++i) {
        randomPoints.push_back(Point_3(dis(gen), dis(gen), dis(gen)));
    }
    auto randomHull = computeConvexHull(randomPoints);
    
    auto hulls = computeConvexHullFromOBJ("assets/entrada.obj");
    auto points = readOBJPoints("assets/entrada.obj");

    float angle = 0.0f;
    bool drawRandomHull = false;
    bool drawPoints = false;
    bool drawVertices = false;
    bool drawLines = false;
    bool drwFaces = true;

    window.run([&](float deltaTime, Renderer& renderer) {
        renderer.clear();
        renderer.lookAt(15.0f * std::cos(angle), 10.0f, 15.0f * std::sin(angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        angle += 0.01f;

        if(drawRandomHull) {
            auto& randomVertices = std::get<0>(randomHull);
            auto& randomFaces = std::get<1>(randomHull);

            if(drwFaces){
                renderer.drawPoligon(randomVertices, randomFaces, 0.5f, 0.5f, 0.5f, 1.0f);
            }

            if(drawLines) {
                for (const auto& face : randomFaces) {
                    for (size_t i = 0; i < face.size(); ++i) {
                        unsigned int idx1 = face[i];
                        unsigned int idx2 = face[(i + 1) % face.size()];

                        float x1 = randomVertices[idx1 * 3];
                        float y1 = randomVertices[idx1 * 3 + 1];
                        float z1 = randomVertices[idx1 * 3 + 2];

                        float x2 = randomVertices[idx2 * 3];
                        float y2 = randomVertices[idx2 * 3 + 1];
                        float z2 = randomVertices[idx2 * 3 + 2];

                        renderer.drawLine(x1, y1, z1, x2, y2, z2, 0.0f, 1.0f, 0.0f, 1.0f);
                    }
                }
            }

            if (drawVertices) {
                for (size_t i = 0; i < randomVertices.size(); i += 3) {
                    renderer.drawSphere(randomVertices[i], randomVertices[i+1], randomVertices[i+2], 0.05f, 4, 4, 1.0f, 1.0f, 0.0f, 1.0f);
                }
            }

            if(drawPoints){
                for (size_t i = 0; i < randomPoints.size(); i++) {
                    renderer.drawSphere(randomPoints[i][0], randomPoints[i][1], randomPoints[i][2], 0.05f, 4, 4, 1.0f, 0.0f, 0.0f, 1.0f);
                }
            }
        } else {
            for(auto hull : hulls) {
                auto& nome = std::get<0>(hull);
                auto& vertices = std::get<0>(std::get<1>(hull));
                auto& faces = std::get<1>(std::get<1>(hull));

                if(drwFaces) {
                    if(nome == "olho_esquerdo" || nome == "olho_direito") {
                        renderer.drawPoligon(vertices, faces, 0.1f, 0.1f, 0.1f, 1.0f);
                    } else if(nome == "nariz") {
                        renderer.drawPoligon(vertices, faces, 0.8f, 0.5f, 0.1f, 1.0f);
                    } else if(nome == "chapeu_cima") {
                        renderer.drawPoligon(vertices, faces, 0.1f, 0.1f, 0.1f, 1.0f);
                    } else if(nome == "chapeu_baixo") {
                        renderer.drawPoligon(vertices, faces, 0.1f, 0.1f, 0.1f, 1.0f);
                    } else {
                        renderer.drawPoligon(vertices, faces, 0.8f, 0.8f, 0.8f, 1.0f);
                    }
                }

                if (drawVertices) {
                    for (size_t i = 0; i < vertices.size(); i += 3) {
                        renderer.drawSphere(vertices[i], vertices[i+1], vertices[i+2], 0.05f, 4, 4, 1.0f, 1.0f, 0.0f, 1.0f);
                    }
                }

                if (drawLines) {
                    for (const auto& face : faces) {
                        for (size_t i = 0; i < face.size(); ++i) {
                            unsigned int idx1 = face[i];
                            unsigned int idx2 = face[(i + 1) % face.size()];

                            float x1 = vertices[idx1 * 3];
                            float y1 = vertices[idx1 * 3 + 1];
                            float z1 = vertices[idx1 * 3 + 2];

                            float x2 = vertices[idx2 * 3];
                            float y2 = vertices[idx2 * 3 + 1];
                            float z2 = vertices[idx2 * 3 + 2];

                            renderer.drawLine(x1, y1, z1, x2, y2, z2, 0.0f, 1.0f, 0.0f, 1.0f);
                        }
                    }
                }
            }

            if (drawPoints) {
                for (size_t i = 0; i < points.size(); i++) {
                    renderer.drawSphere(points[i][0], points[i][1], points[i][2], 0.05f, 4, 4, 1.0f, 0.0f, 0.0f, 1.0f);
                }
            }
        }
    }, [&](int keyState, SDL_KeyCode key) {
        if (keyState == 1) {
            if (key == SDLK_p) {
                drawPoints = !drawPoints;
            } else if (key == SDLK_l) {
                drawLines = !drawLines;
            }
            else if (key == SDLK_f) {
                drwFaces = !drwFaces;
            }
            else if (key == SDLK_v) {
                drawVertices = !drawVertices;
            }
            else if (key == SDLK_r) {
                drawRandomHull = !drawRandomHull;
            }
        }
    });

    return 0;
}