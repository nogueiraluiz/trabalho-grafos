#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>
#include <fstream>
#include <list>
#include "Vertice.hpp"
#include "Aresta.hpp"

class Grafo {

public:
    Grafo(std::ifstream &arquivoInstancia, bool direcionado, bool verticesPonderados, bool arestasPonderadas);
    Grafo(bool direcionado, bool verticesPonderados, bool arestasPonderadas);
    ~Grafo();
    void print();
    void print(std::ofstream &output);
    void adicionaVertice(int idVertice, int peso = 0);
    void adicionaAresta(int idVerticeU, int idVerticeV, int peso = 0);
    void removeVertice(int idVertice);
    void removeAresta(int idVerticeU, int idVerticeV);
    bool getOpcaoVertices();
    bool getOpcaoArestas();
    bool getOpcaoDirecionado();
private:
    bool direcionado;
    bool verticesPonderados;
    bool arestasPonderadas;
    std::list<Vertice*> vertices;
    Vertice* getVertice(int id);
    void adicionaAdjacencias(int idA, int idB, int peso = 0);
    bool existeAresta(int idVerticeU, int idVerticeV);
};

#endif
