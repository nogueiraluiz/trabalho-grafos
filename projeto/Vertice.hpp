#ifndef VERTICE_HPP
#define VERTICE_HPP

#include <vector>

struct Vertice
{
    unsigned int _id;
    int peso;
    std::vector<Vertice*>* _adjacencias;
};

#endif
