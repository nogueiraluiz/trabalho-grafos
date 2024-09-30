#ifndef ALGORITMOS_HPP
#define ALGORITMOS_HPP

#include <vector>
#include <list>
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
        std::list<Aresta *> coletaArestasOrdenadas(Grafo *grafo);
        int numeroDeVerticesAdjacentes(Aresta *aresta, std::set<int> &visitados);
        bool saoAdjacentes(Aresta *e, Aresta *aresta);
        bool isAdjacenteAFloresta(Aresta *aresta, std::vector<std::vector<Aresta *>> &floresta);
        void preencheFloresta(std::vector<std::vector<Aresta *>> &floresta,
                std::list<Aresta *> &arestas,
                std::set<int> &visitados,
                std::vector<int> &minimos,
                std::vector<int> &maximos);
        void preencheFlorestaRandomizado(std::vector<std::vector<Aresta *>> &floresta,
                std::list<Aresta *> &arestas,
                std::set<int> &visitados,
                float alfa,
                std::vector<int> &minimos,
                std::vector<int> &maximos);
        int novoCalculaGap(std::vector<int> &minimos, std::vector<int> &maximos);
        int calculaGap(std::vector<std::vector<Aresta *>> &floresta);
        int calculaNovoGap(std::vector<std::vector<Aresta *>> &floresta, Aresta *candidata);
        int calculaImpacto(Aresta *candidata,
                std::vector<std::vector<Aresta *>> &floresta,
                std::vector<int> &minimos,
                std::vector<int> &maximos);
        void adicionaNovaAresta(std::vector<std::vector<Aresta *>> &floresta,
                std::list<Aresta *> &arestas,
                std::set<int> &visitados,
                std::vector<int> &minimos,
                std::vector<int> &maximos);
        int geraIndiceAleatorioEntreZeroE(int max);
        void adicionaNovaArestaRandomizado(std::vector<std::vector<Aresta *>> &floresta,
                std::list<Aresta *> &arestas,
                std::set<int> &visitados,
                float alfa,
                std::vector<int> &minimos,
                std::vector<int> &maximos);
        Grafo *gulosoComum(Grafo *grafo, int numeroParticoes);
        Grafo *gulosoRandomizado(Grafo *grafo, int numeroParticoes, float alfa);

}

#endif