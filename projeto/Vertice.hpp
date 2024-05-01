#ifndef VERTICE_HPP
#define VERTICE_HPP

#include <vector>

struct Vertice
{
    int id;
    int peso;
    int numArestas;
    std::vector<Vertice *> adjacencias;
};

#endif
