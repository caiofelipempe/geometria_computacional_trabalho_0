#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>

class Renderer {
private:

public:
    void clear();
    void drawPoligon(std::vector<float> hullVertices, std::vector<std::vector<unsigned int>> hullFaces, float r, float g, float b, float a);
    void drawPoint(float x, float y, float z, float r, float g, float b, float a);
    void drawLine(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b, float a);
    void lookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
    void drawSphere(float x, float y, float z, float radius, int slices, int stacks, float r, float g, float b, float a);
};

#endif // RENDERER_HPP