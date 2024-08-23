#ifndef VERTICE_HPP
#define VERTICE_HPP

#include "Aresta.hpp"

struct Aresta;

struct Vertice
{
    int id;
    int peso;
    Aresta* arestas;
};

#endif
