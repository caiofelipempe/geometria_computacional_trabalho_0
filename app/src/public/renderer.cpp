#include "renderer.hpp"

#include <GL/glu.h>
#include <cmath>
#include <array>

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::lookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ) {
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
}

void Renderer::drawPoligon(std::vector<float> hullVertices, std::vector<std::vector<unsigned int>> hullFaces) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, hullVertices.data());

    for (const auto& face : hullFaces) {
        glBegin(GL_POLYGON);
        for (unsigned int idx : face) {
            glArrayElement(idx);
        }
        glEnd();
    }

    glDisableClientState(GL_VERTEX_ARRAY);
}