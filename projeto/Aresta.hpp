#ifndef ARESTA_HPP
#define ARESTA_HPP

#include "Vertice.hpp"

struct Vertice;

struct Aresta
{
    int peso;
    Vertice *origem;
    Aresta *prox;
    Vertice *destino;
};

#endif
