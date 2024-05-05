#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <fstream>
#include <map>
#include "Vertice.hpp"
#include "Aresta.hpp"

class Grafo {

public:
    Grafo(std::ifstream &arquivoInstancia, bool direcionado, bool ponderadoVertices, bool ponderadoArestas);
    Grafo(bool direcionado, bool ponderadoVertices, bool ponderadoArestas);
    ~Grafo();
    void print();
    void print(std::ofstream &output);
    void adicionaVertice(int idVertice, int peso = 0);
    void adicionaAresta(int idVerticeA, int idVerticeB, int peso = 0);
    void removeVertice(int idVertice);
    void removeAresta(int idVerticeA, int idVeritceB);
private:
    bool direcionado;
    bool ponderadoVertices;
    bool ponderadoArestas;
    std::map<int, Vertice*> vertices;
    void adicionaAdjacencias(int idA, int idB, int peso = 0);
    bool saoVizinhos(int idVerticeA, int idVerticeB);
    void printDirecionado(std::ofstream &output);
    void printNaoDirecionado(std::ofstream &output);
};

#endif
