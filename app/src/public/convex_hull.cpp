#include "convex_hull.hpp"
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

std::tuple<std::vector<float>, std::vector<std::vector<unsigned int>>> computeConvexHullFromOBJ(const std::string& nomeArquivo) {
    std::vector<Point_3> vertices;
    std::ifstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        throw std::runtime_error("Erro ao abrir o arquivo: " + nomeArquivo);
    }
    
    std::string linha;
    int linhaNumero = 0;
    
    while (std::getline(arquivo, linha)) {
        linhaNumero++;
        
        // Remove espaços em branco no início e fim
        linha.erase(0, linha.find_first_not_of(" \t\r\n"));
        linha.erase(linha.find_last_not_of(" \t\r\n") + 1);
        
        // Ignora linhas vazias ou comentários
        if (linha.empty() || linha[0] == '#') {
            continue;
        }
        
        // Verifica se é uma linha de vértice (começa com 'v')
        if (linha.size() >= 1 && linha[0] == 'v') {
            // Verifica se é um vértice geométrico (não é vt, vn, vp)
            if (linha.size() >= 2 && (linha[1] == ' ' || linha[1] == '\t')) {
                std::istringstream ss(linha.substr(1)); // Pula o 'v'
                float x, y, z = 1.0f;
                
                // Tenta ler as coordenadas
                ss >> x >> y >> z;
                
                vertices.push_back(Point_3(x, y, z));
                
                // Debug opcional
                // std::cout << "Vértice lido na linha " << linhaNumero 
                //           << ": v " << x << " " << y << " " << z 
                //           << " " << w << std::endl;
            }
        }
    }
    
    arquivo.close();
    std::cout << "Total de vértices lidos: " << vertices.size() << std::endl;
    
    return computeConvexHull(vertices);
}