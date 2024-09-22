#ifndef ALGORITMOS_HPP
#define ALGORITMOS_HPP

#include <vector>
#include <set>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <random>
#include "Grafo.hpp"
#include "Vertice.hpp"
#include "Aresta.hpp"

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
                std::set<int> &visitados);
        void preencheFlorestaRandomizado(std::vector<std::vector<Aresta *>> &floresta,
                std::vector<Aresta *> &arestas,
                std::set<int> &visitados,
                float alfa);
        int calculaGap(std::vector<std::vector<Aresta *>> &floresta);
        int calculaNovoGap(std::vector<std::vector<Aresta *>> &floresta, Aresta *candidata);
        void adicionaNovaAresta(std::vector<std::vector<Aresta *>> &floresta,
                std::vector<Aresta *> &arestas,
                std::set<int> &visitados);
        int geraAleatorioEntreZeroE(int max);
        void adicionaNovaArestaRandomico(std::vector<std::vector<Aresta *>> &floresta,
                std::vector<Aresta *> &arestas,
                std::set<int> &visitados,
                float alfa);
        Grafo *gulosoComum(Grafo *grafo, int numeroParticoes);
        Grafo *gulosoRandomizado(Grafo *grafo, int numeroParticoes, float alfa);

}

#endif