#ifndef ALGORITMOS_HPP
#define ALGORITMOS_HPP

#include <vector>
#include <set>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "Grafo.hpp"
#include "Vertice.hpp"
#include "Aresta.hpp"
#include <limits>

/**
 * Algoritmos da segunda entrega do trabalho.
 */
namespace Algoritmos
{
    int gapAresta(Aresta *aresta);
    std::vector<Aresta *> coletaEOrdenaArestas(Grafo *grafo);
    int numeroDeAdjacencias(Aresta *aresta, std::vector<std::vector<Aresta *>> &particoes);
    bool saoAdjacentes(Aresta *e, Aresta *aresta);
    bool isAdjacenteAFloresta(Aresta *aresta, std::vector<std::vector<Aresta *>> &floresta);
    void preencheFloresta(std::vector<std::vector<Aresta *>> &floresta,
                          std::vector<Aresta *> &arestas,
                          std::set<Vertice *> &visitados);
    int calculaGap(std::vector<std::vector<Aresta *>> &floresta);
    int calculaNovoGap(std::vector<std::vector<Aresta *>> &floresta, Aresta* candidata);
    void adicionaNovaAresta(std::vector<std::vector<Aresta *>> &floresta, 
            std::vector<Aresta *> &arestas, 
            std::set<Vertice *> visitados);
    Grafo *construtivo(Grafo *grafo, int numeroParticoes);

}

#endif