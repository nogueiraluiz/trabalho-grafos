#ifndef VERTICE_HPP
#define VERTICE_HPP

#include <list>
#include "Aresta.hpp"

struct Aresta;

struct Vertice
{
    int id;
    int peso;
    std::list<Aresta *> arestas;
};

#endif
