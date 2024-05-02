#ifndef ARESTA_HPP
#define ARESTA_HPP

#include "Vertice.hpp"

struct Vertice;

struct Aresta
{
    int id;
    int peso;
    Vertice *destino;
};

#endif
