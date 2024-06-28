#ifndef VERTICE_HPP
#define VERTICE_HPP

#include <limits.h>
#include "Aresta.hpp"
#include <list>

struct Aresta;

struct Vertice
{
    int id;
    int peso;
    int distancia = std::numeric_limits<int>::max(); //Distância Dijkstra
    bool aberto = true; //Aux dijkstra
    Vertice *predecessor = nullptr;
    std::list<Aresta*> arestas;
};

#endif
