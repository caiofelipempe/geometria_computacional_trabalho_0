#ifndef CONVEX_HULL_HPP
#define CONVEX_HULL_HPP

#include <vector>
#include <cstdint>
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> K;
typedef K::Point_3 Point_3;

std::tuple<std::vector<float>, std::vector<std::vector<unsigned int>>> computeConvexHull(std::vector<Point_3> points);
std::tuple<std::vector<float>, std::vector<std::vector<unsigned int>>> computeConvexHullFromOBJ(const std::string& nomeArquivo);

#endif // CONVEX_HULL_HPP