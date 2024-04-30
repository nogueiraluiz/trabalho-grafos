#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <fstream>
#include <vector>
#include "Vertice.hpp"
#include "Aresta.hpp"

class Grafo {

public:
    Grafo(std::ifstream &arquivoInstancia, int optDirec, int optPV, int optPA);
    ~Grafo();
    void print();
private:
    void adicionaAresta(int idVerticeA, int IdVerticeB, int peso);
    unsigned int qtdArestas;
    unsigned int qtdVertices;
    bool direcionado;
    bool ponderadoV;
    bool ponderadoA;
    std::vector<Vertice*>* vertices;
    std::vector<Aresta*>* arestas;
};

#endif
