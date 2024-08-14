#ifndef ARESTA_HPP
#define ARESTA_HPP

#include "Vertice.hpp"

struct Vertice;

struct Aresta
{
    int peso;
    int id_origem;
    Vertice* destino;
};

#endif
