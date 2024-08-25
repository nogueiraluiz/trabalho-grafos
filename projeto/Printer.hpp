#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include "Grafo.hpp"
#include <sstream>

namespace Printer
{
    void printGrafo(std::vector<Vertice*>& vertices, bool direcionado, bool arestasPonderadas, bool verticesPonderados, std::ofstream &arquivo);
    void printGrafo(std::vector<Vertice*>& vertices, bool direcionado, bool arestasPonderadas, bool verticesPonderados);
    void printVerticesPonderados(std::vector<Vertice*>& vertices, std::ofstream &arquivo);
    std::string getRepresentacaoAresta(int idOrigem, std::string separador, int idDestino, const Aresta &aresta, bool ponderada);
    void printArestasDirecionadas(std::vector<Vertice*>& vertices, bool ponderadoNasArestas, std::ofstream &arquivo);
    void printArestasNaoDirecionadas(std::vector<Vertice*>& vertices, bool ponderadoNasArestas, std::ofstream &arquivo);
    void printArvoreCaminhamento(std::vector<Vertice*>& vertices);
    std::string getRepresentacaoArestaRetorno(int idOrigem, std::string separador, int idDestino, const Aresta &aresta);
    void printArvoreCaminhamento(std::vector<Vertice*>& vertices, std::ofstream &arquivo);
}

#endif