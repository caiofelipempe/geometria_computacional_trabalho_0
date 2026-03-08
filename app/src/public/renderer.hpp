#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>

class Renderer {
private:

public:
    void clear();
    void drawPoligon(std::vector<float> hullVertices, std::vector<std::vector<unsigned int>> hullFaces);
    void lookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
};

#endif // RENDERER_HPP