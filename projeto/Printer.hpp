#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <iostream>
#include <list>
#include <set>
#include <string>
#include <algorithm>
#include "Grafo.hpp"
#include <sstream>

namespace Printer
{
    void printGrafo(std::list<Vertice*>& vertices, bool direcionado, bool arestasPonderadas, bool verticesPonderados, std::ofstream &arquivo);
    void printGrafo(std::list<Vertice*>& vertices, bool direcionado, bool arestasPonderadas, bool verticesPonderados);
    void printVerticesPonderados(std::list<Vertice*>& vertices, std::ofstream &arquivo);
    std::string getRepresentacaoAresta(int idOrigem, std::string separador, int idDestino, const Aresta &aresta, bool ponderada);
    void printArestasDirecionadas(std::list<Vertice*>& vertices, bool ponderadoNasArestas, std::ofstream &arquivo);
    void printArestasNaoDirecionadas(std::list<Vertice*>& vertices, bool ponderadoNasArestas, std::ofstream &arquivo);
}

#endif
