#include <iostream>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include "Grafo.hpp"
#include <sstream>

namespace Printer
{
    void printVerticesPonderados(std::map<int, const Vertice*> vertices, std::ofstream &arquivo);
    std::string getRepresentacaoAresta(int id, std::string separador, int idDestino, Aresta &aresta, bool ponderada);
    void printArestasDirecionadas(std::map<int, const Vertice*> vertices, bool ponderadoNasArestas, std::ofstream &arquivo);
    void printArestasNaoDirecionadas(std::map<int, const Vertice*> vertices, bool ponderadoNasArestas, std::ofstream &arquivo);
    void printGrafo(Grafo &g, std::ofstream &arquivo);
    void printGrafo(Grafo &g);
}
