#include "renderer.hpp"

#include <GL/glu.h>
#include <cmath>
#include <array>

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::lookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ) {
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
}

void Renderer::drawPoligon(std::vector<float> hullVertices, std::vector<std::vector<unsigned int>> hullFaces, float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
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

void Renderer::drawSphere(float radius, int slices, int stacks, float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
    GLUquadric* quad = gluNewQuadric();
    gluSphere(quad, radius, slices, stacks);
    gluDeleteQuadric(quad);
}

void Renderer::drawCapsule(float radius, float height, int slices, int stacks, float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
    GLUquadric* quad = gluNewQuadric();

    // Draw bottom sphere
    gluSphere(quad, radius, slices, stacks);

    // Draw cylinder
    glTranslatef(0.0f, 0.0f, radius);
    gluCylinder(quad, radius, radius, height, slices, stacks);

    // Draw top sphere
    glTranslatef(0.0f, 0.0f, height);
    gluSphere(quad, radius, slices, stacks);

    gluDeleteQuadric(quad);
}

void Renderer::drawCylinder(float baseRadius, float topRadius, float height, int slices, int stacks, float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, baseRadius, topRadius, height, slices, stacks);
    gluDeleteQuadric(quad);
}