#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <set>
#include "Vertice.hpp"
#include "Aresta.hpp"

class Grafo
{

public:
    Grafo(std::ifstream &arquivoInstancia, bool direcionado, bool verticesPonderados, bool arestasPonderadas);
    Grafo(bool direcionado, bool verticesPonderados, bool arestasPonderadas);
    ~Grafo();
    void print();
    void print(std::ofstream &output);
    void adicionaVertice(int idVertice, int peso = 0);
    void adicionaAresta(int idVerticeU, int idVerticeV, int peso = 0);
    void removeVertice(int idVertice);
    bool removeAresta(int idVerticeU, int idVerticeV);
    Grafo *fechoTransitivoDireto(int idVertice);
    Grafo *fechoTransitivoIndireto(int idVertice);
    Grafo *caminhoMinimoFloyd(int idVerticeU, int idVerticeV);
    void analiseExcentricidade();

private:
    bool direcionado;
    bool verticesPonderados;
    bool arestasPonderadas;
    std::vector<Vertice *> vertices;
    Vertice *getVertice(int id);
    void adicionaAdjacencias(int idA, int idB, int peso = 0);
    bool existeAresta(int idVerticeU, int idVerticeV);
    void auxFechoDireto(Vertice* vertice, std::set<int>& fecho, Grafo *grafoFecho);
    int encontraIndiceVertice(int id);
    void inicializaMatrizDistancias(std::vector<std::vector<int>>& distancias, int ordem);
    void atualizaMatrizDistancias(std::vector<std::vector<int>>& distancias, int ordem, int indice);
    std::vector<std::vector<int>> getMatrizDistancias();
    void inicializaMatrizesFloyd(std::vector<std::vector<int>>& distancias, std::vector<std::vector<int>>& proximos, int ordem);
    void atualizaMatrizesFloyd(std::vector<std::vector<int>>& distancias, std::vector<std::vector<int>>& proximos, int ordem, int indice);
    int getExcentricidade(const std::vector<int>& distanciasVertice);
    bool auxFechoIndireto(std::set<int>& fecho, std::vector<Vertice*>& naoUtilizados, Grafo *grafoFecho);
    void liberaMemoriaArestas(Aresta* arestas);
};

#endif
