#ifndef VERTICE_HPP
#define VERTICE_HPP

#include "Aresta.hpp"
#include <list>

struct Aresta;

struct Vertice
{
    int id;
    int peso;
    std::list<Aresta*> arestas;
};

#endif
