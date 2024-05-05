#ifndef VERTICE_HPP
#define VERTICE_HPP

#include <map>
#include "Aresta.hpp"

struct Aresta;

struct Vertice
{
    int id;
    int peso;
    std::map<int, Aresta *> arestas;
};

#endif
