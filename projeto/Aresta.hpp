#ifndef ARESTA_HPP
#define ARESTA_HPP

#include "Vertice.hpp"

struct Aresta
{
    unsigned int _id;
    int _peso;
    Vertice *_cabeca;
    Vertice *_cauda;
};

#endif
