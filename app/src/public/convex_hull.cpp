#include "convex_hull.hpp"
#include <GL/glu.h>
#include <random>
#include <map>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/convex_hull_3.h>

typedef CGAL::Simple_cartesian<double> K;
typedef K::Point_3 Point_3;
typedef CGAL::Polyhedron_3<K> Polyhedron;
typedef Polyhedron::Vertex_handle Vertex_handle;
typedef Polyhedron::Facet_handle Facet_handle;

std::tuple<std::vector<float>, std::vector<std::vector<unsigned int>>> computeConvexHull() {
    std::vector<float> hullVertices;
    std::vector<std::vector<unsigned int>> hullFaces;

    std::vector<Point_3> points;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-5.0, 5.0);

    // Generate 50 random points
    for (int i = 0; i < 50; ++i) {
        points.push_back(Point_3(dis(gen), dis(gen), dis(gen)));
    }

    
    Polyhedron poly;
    CGAL::convex_hull_3(points.begin(), points.end(), poly);

    // Extract vertices
    hullVertices.clear();
    std::map<Vertex_handle, unsigned int> vertexIndex;
    unsigned int idx = 0;
    for (auto v = poly.vertices_begin(); v != poly.vertices_end(); ++v) {
        hullVertices.push_back(v->point().x());
        hullVertices.push_back(v->point().y());
        hullVertices.push_back(v->point().z());
        vertexIndex[v] = idx++;
    }

    // Extract faces
    hullFaces.clear();
    for (auto f = poly.facets_begin(); f != poly.facets_end(); ++f) {
        std::vector<unsigned int> face;
        auto h = f->facet_begin();
        do {
            face.push_back(vertexIndex[h->vertex()]);
            ++h;
        } while (h != f->facet_begin());
        hullFaces.push_back(face);
    }

    return {hullVertices, hullFaces};
}