#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <fstream>
#include <vector>
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
    int numArestas;
    int numVertices;
    bool direcionado;
    bool ponderadoVertices;
    bool ponderadoArestas;
    std::vector<Vertice *> vertices;
    std::vector<Aresta *> arestas;
    void adicionaAdjacencias(int idA, int idB);
    Vertice* getVertice(int idVerticeAlvo);
    bool saoVizinhos(int idVerticeA, int idVerticeB);
};

#endif
