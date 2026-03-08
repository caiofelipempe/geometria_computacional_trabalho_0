#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>

class Renderer {
private:

public:
    void clear();
    void drawPoligon(std::vector<float> hullVertices, std::vector<std::vector<unsigned int>> hullFaces, float r, float g, float b, float a);
    void lookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
    void drawSphere(float radius, int slices, int stacks, float r, float g, float b, float a);
    void drawCapsule(float radius, float height, int slices, int stacks, float r, float g, float b, float a);
    void drawCylinder(float baseRadius, float topRadius, float height, int slices, int stacks, float r, float g, float b, float a);
};

#endif // RENDERER_HPP