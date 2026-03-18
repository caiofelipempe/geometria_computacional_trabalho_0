#include "convex_hull_compute.hpp"
#include <GL/glu.h>
#include <map>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/convex_hull_3.h>

typedef CGAL::Simple_cartesian<double> K;
typedef CGAL::Polyhedron_3<K> Polyhedron;
typedef Polyhedron::Vertex_handle Vertex_handle;
typedef Polyhedron::Facet_handle Facet_handle;

std::tuple<std::vector<float>, std::vector<std::vector<unsigned int>>> computeConvexHull(std::vector<Point_3> points) {
    std::vector<float> hullVertices;
    std::vector<std::vector<unsigned int>> hullFaces;
    
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

std::vector<std::tuple<std::string, std::tuple<std::vector<float>, std::vector<std::vector<unsigned int>>>>> computeConvexHullFromOBJ(const std::string& nomeArquivo) {
    std::vector<std::tuple<std::string, std::vector<Point_3>>> objetos;
    std::ifstream arquivo(nomeArquivo);
    std::vector<std::tuple<std::string, std::tuple<std::vector<float>, std::vector<std::vector<unsigned int>>>>> hulls;
    
    if (!arquivo.is_open()) {
        throw std::runtime_error("Erro ao abrir arquivo");
    }
    
    std::string linha;
    std::string objetoAtual = "default";
    std::vector<Point_3> verticesAtuais;
    bool primeiroObjeto = true;
    
    auto finalizarObjetoAtual = [&]() {
        if (!verticesAtuais.empty() || !primeiroObjeto) {
            objetos.emplace_back(objetoAtual, verticesAtuais);
            verticesAtuais.clear();
        }
        primeiroObjeto = false;
    };
    
    while (std::getline(arquivo, linha)) {
        // Remove espaços extras
        linha.erase(0, linha.find_first_not_of(" \t\r\n"));
        
        if (linha.empty() || linha[0] == '#') continue;
        
        if (linha[0] == 'o' || linha[0] == 'g') {
            // Finaliza objeto anterior
            finalizarObjetoAtual();
            
            // Pega nome do novo objeto
            std::istringstream ss(linha.substr(1));
            ss >> objetoAtual;
            if (objetoAtual.empty()) {
                objetoAtual = "objeto_" + std::to_string(objetos.size() + 1);
            }
        }
        else if (linha.size() >= 2 && linha[0] == 'v' && 
                 (linha[1] == ' ' || linha[1] == '\t')) {
            
            std::istringstream ss(linha.substr(1));
            float x, y, z;
            if (ss >> x >> y >> z) {
                verticesAtuais.push_back(Point_3(x, y, z));
            }
        }
    }
    
    // Finaliza último objeto
    if (!verticesAtuais.empty() || objetos.empty()) {
        objetos.emplace_back(objetoAtual, verticesAtuais);
    }

    for (auto& obj : objetos) {
        hulls.push_back(std::make_tuple(std::get<0>(obj), computeConvexHull(std::get<1>(obj))));
    }
    
    return hulls;
}