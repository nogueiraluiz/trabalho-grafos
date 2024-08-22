#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <set>
#include <map>
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
    Grafo *verticesDeArticulacao();
    void caminhamentoProfundidade(int idVerticeInicio);

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
    void inicializaMatrizDistancias(std::vector<std::vector<int>> &distancias, int ordem);
    void atualizaMatrizDistancias(std::vector<std::vector<int>> &distancias, int ordem, int indice);
    std::vector<std::vector<int>> getMatrizDistancias();
    void inicializaMatrizesFloyd(std::vector<std::vector<int>>& distancias, std::vector<std::vector<int>>& proximos, int ordem);
    void atualizaMatrizesFloyd(std::vector<std::vector<int>>& distancias, std::vector<std::vector<int>>& proximos, int ordem, int indice);
    bool auxFechoIndireto(std::set<int>& fecho, std::vector<Vertice*>& naoUtilizados, Grafo *grafoFecho);
    void liberaMemoriaArestas(Aresta* arestas);
    void buscaProfundidadeComponente(Vertice *v, std::map<Vertice*, bool>& usados, std::set<Vertice*>& componente);
    std::set<Vertice*> buscaComponente(Vertice *v, std::map<Vertice*, bool>& usados);
    std::set<std::set<Vertice*>> getComponentesConexas();
    void buscaProfundidadeArticulacoes(std::set<int>& articulacoes, Vertice *v, Vertice* pai, std::map<int, int>& tempoEntrada, std::map<int, int>& minimo, std::map<int, bool> visitado, int& cronometro);
    std::set<int> encontraArticulacoesComponente(Vertice *v, std::set<Vertice*> componente);
    int getExcentricidade(const std::vector<int> &distanciasVertice);
    void caminhaProfundidade(Vertice *u, std::map<Vertice *, int> &entrada, std::map<Vertice *, int> &saida, std::map<Vertice *, int> &cor, int &tempo);
    void preencheMapaInicialProfundidade(std::map<Vertice *, int> &mapa);
  
};

#endif
