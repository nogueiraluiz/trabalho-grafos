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
                std::set<int> &visitados);
        void preencheFlorestaRandomizado(std::vector<std::vector<Aresta *>> &floresta,
                std::list<Aresta *> &arestas,
                std::set<int> &visitados,
                float alfa);
        int calculaGap(std::vector<std::vector<Aresta *>> &floresta);
        int calculaNovoGap(std::vector<std::vector<Aresta *>> &floresta, Aresta *candidata);
        void adicionaNovaAresta(std::vector<std::vector<Aresta *>> &floresta,
                std::list<Aresta *> &arestas,
                std::set<int> &visitados);
        int geraIndiceAleatorioEntreZeroE(int max);
        void adicionaNovaArestaRandomizado(std::vector<std::vector<Aresta *>> &floresta,
                std::list<Aresta *> &arestas,
                std::set<int> &visitados,
                float alfa);
        Grafo *gulosoComum(Grafo *grafo, int numeroParticoes);
        Grafo *gulosoRandomizado(Grafo *grafo, int numeroParticoes, float alfa);

        void atualizaProbabilidades(float probabilidades[], float mediaQualidades[], float alfas[], int melhorGap);

        int escolheAlfa(float probabilidades[]);

        Grafo *gulosoRandomizadoReativo(Grafo *grafo, int numeroParticoes);
}

#endif