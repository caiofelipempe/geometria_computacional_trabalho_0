#ifndef CONVEX_HULL_HPP
#define CONVEX_HULL_HPP

#include <vector>
#include <cstdint>
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> K;
typedef K::Point_3 Point_3;

extern std::vector<float> hullVertices;
extern std::vector<std::vector<unsigned int>> hullFaces;

std::tuple<std::vector<float>, std::vector<std::vector<unsigned int>>> computeConvexHull();

#endif // CONVEX_HULL_HPP