#ifndef READER_HPP
#define READER_HPP

#include "Grafo.hpp"

namespace Reader
{
    Grafo *readGrafo(std::ifstream &arquivoInstancia);
    void readVertices(std::ifstream &arquivoInstancia, Grafo* grafo);
    void readPesos(std::ifstream &arquivoInstancia, Grafo* grafo);
    void readArestas(std::ifstream &arquivoInstancia, Grafo* grafo);
}

#endif