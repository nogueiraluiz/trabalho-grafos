#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <fstream>
#include <vector>
#include <list>
#include "Vertice.hpp"
#include "Aresta.hpp"

class Grafo {

public:
    Grafo(std::ifstream &arquivoInstancia, bool direcionado, bool ponderadoVertices, bool ponderadoArestas);
    ~Grafo();
    void print();
    void adicionaAresta(int idVerticeA, int idVerticeB, int peso = 0);
    void adicionaVertice(int idVertice, int peso = 0);
    void removeAresta(int idVerticeA, int idVeritceB);
    void removeVertice(int idVertice);
private:
    bool direcionado;
    bool ponderadoVertices;
    bool ponderadoArestas;
    std::list<Vertice *> vertices;
    void adicionaAdjacencias(int idA, int idB, int peso = 0);
    Vertice* getVertice(int idVerticeAlvo);
    bool saoVizinhos(int idVerticeA, int idVerticeB);
};

#endif
