#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits.h>
#include <limits>
#include "Grafo.hpp"
#include "Printer.hpp"

/**
 * Construtor que instancia um grafo de acordo com as arestas definidas em um arquivo .dat lido como argumento para a execução.
 */
Grafo::Grafo(std::ifstream &arquivoInstancia, bool direcionado, bool arestasPonderadas, bool verticesPonderados)
{
    this->direcionado = direcionado;
    this->arestasPonderadas = arestasPonderadas;
    this->verticesPonderados = verticesPonderados;
    std::string linha;
    getline(arquivoInstancia, linha);
    while (getline(arquivoInstancia, linha))
    {
        std::stringstream stream(linha);
        std::string item;
        std::vector<int> itens;
        while (getline(stream, item, ' '))
        {
            itens.push_back(std::stoi(item));
        }
        int idVerticeU = itens[0];
        int idVerticeV = itens[1];
        int pesoAresta = itens[2];
        adicionaAresta(idVerticeU, idVerticeV, pesoAresta);
    }
}

/**
 * Construtor mais básico, não define quaisquer elementos dos conjuntos de vértices e de arestas.
 */
Grafo::Grafo(bool direcionado, bool verticesPonderados, bool arestasPonderadas)
{
    this->direcionado = direcionado;
    this->arestasPonderadas = arestasPonderadas;
    this->verticesPonderados = verticesPonderados;
}

Grafo::~Grafo()
{
    for (Vertice *vertice : vertices)
    {
        for (Aresta *aresta : vertice->arestas)
        {
            delete aresta;
        }
        delete vertice;
    }
}

/**
 * Método para impressão de um grafo diretamente para o console, seguindo a visão de sua lista de adjacências.
 */
void Grafo::print()
{
    Printer::printGrafo(vertices, direcionado, arestasPonderadas, verticesPonderados);
}

/**
 * Método público para escrever o grafo num arquivo especificado pela referência
 * à ofstream passada como parâmetro.
 */
void Grafo::print(std::ofstream &output)
{
    Printer::printGrafo(vertices, direcionado, arestasPonderadas, verticesPonderados, output);
}

bool Grafo::existeAresta(int idVerticeU, int idVerticeV)
{
    Vertice *u = getVertice(idVerticeU);
    if (u == nullptr)
    {
        return false; // não existe vértice V
    }
    for (Aresta *aresta : u->arestas)
    {
        if (aresta->destino->id == idVerticeV)
        {
            return true;
        }
    }
    return false; // V não é adjacente a U (ou não existe)
}

Vertice *Grafo::getVertice(int idAlvo)
{
    for (Vertice *vertice : vertices)
    {
        if (vertice->id == idAlvo)
        {
            return vertice;
        }
    }
    return nullptr;
}

void Grafo::adicionaAdjacencias(int idVerticeU, int idVerticeV, int peso)
{
    Vertice *u = getVertice(idVerticeU);
    Vertice *v = getVertice(idVerticeV);
    Aresta *e = new Aresta;
    e->destino = v;
    e->peso = peso;
    u->arestas.push_back(e);
}

/**
 * Adiciona um novo vértice ao grafo, caso não exista um com o id especificado.
 */
void Grafo::adicionaVertice(int idVertice, int peso)
{
    if (getVertice(idVertice) != nullptr)
    {
        return; // já existe o vértice com o id especificado
    }
    Vertice *u = new Vertice;
    u->id = idVertice;
    u->peso = peso;
    vertices.push_back(u);
}

void Grafo::adicionaAresta(int idVerticeU, int idVerticeV, int peso)
{
    if (existeAresta(idVerticeU, idVerticeV))
    {
        return; // já existe aresta pelo par
    }
    adicionaVertice(idVerticeU);
    adicionaVertice(idVerticeV);
    adicionaAdjacencias(idVerticeU, idVerticeV, peso);
    if (!direcionado)
    {
        adicionaAdjacencias(idVerticeV, idVerticeU, peso);
    }
}

void Grafo::removeAresta(int idVerticeU, int idVerticeV)
{
    if (!existeAresta(idVerticeU, idVerticeV))
    {
        return;
    }
    Vertice *u = getVertice(idVerticeU);
    for (Aresta *aresta : u->arestas)
    {
        if (aresta->destino->id == idVerticeV)
        {
            delete aresta;
        }
    }
    if (direcionado)
    {
        return;
    }
    Vertice *v = getVertice(idVerticeV);
    for (Aresta *aresta : v->arestas)
    {
        if (aresta->destino->id == idVerticeU)
        {
            delete aresta;
        }
    }
}

/**
 * Método recursivo auxiliar para a funcionalidade de fecho transitivo direto.
 * Busca os sucessores dos sucessores do vértice solicitado.
 */
void Grafo::auxFechoDireto(Vertice *vertice, std::set<int> &fecho)
{
    for (Aresta *aresta : vertice->arestas)
    {
        Vertice *sucessor = aresta->destino;
        fecho.insert(sucessor->id);
        auxFechoDireto(sucessor, fecho);
    }
}

/**
 * Imprime na tela o fecho transitivo direto de um vértice no grafo dado seu id.
 * - caso o grafo não seja direcionado, retorna, visto que a operação não pode ser feita;
 * - caso o vértice não existe no conjunto de vértices, retorna;
 */
void Grafo::fechoTransitivoDireto(int idVertice)
{
    if (!direcionado)
    {
        std::cout << "O grafo deve ser direcionado\n";
        return;
    }
    Vertice *v = getVertice(idVertice);
    if (v == nullptr)
    {
        std::cout << "Nao existe no grafo vertice com o id especificado (" << idVertice << ")\n";
        return;
    }
    std::set<int> fecho;
    for (Aresta *aresta : v->arestas)
    {
        Vertice *sucessor = aresta->destino;
        fecho.insert(sucessor->id);
        auxFechoDireto(sucessor, fecho);
    }
    if (fecho.empty())
    {
        std::cout << "O fecho transitivo direto do vértice é vazio\n";
        return;
    }
    std::cout << "Fecho transitivo direto do vertice " << idVertice << ":\n { ";
    for (int idVerticeFecho : fecho)
    {
        std::cout << idVerticeFecho << " ";
    }
    std::cout << "}\n";
}

void Grafo::removeVertice(int idVertice)
{
    Vertice *v = getVertice(idVertice);
    if (v == nullptr)
    {
        return; // vértice buscado não existe
    }
    for (Aresta *aresta : v->arestas)
    {
        removeAresta(idVertice, aresta->destino->id);
        removeAresta(aresta->destino->id, idVertice);
    }
    delete v;
}
  
bool Grafo::auxDijkstra() {
    for (auto v : vertices) {
        if (v->aberto && v->distancia != std::numeric_limits<int>::max()) {
            return true;
        }
    }
    return false;
}

std::list<int> Grafo::listaAdjacentes(int idVertice) {
    std::list<int> lista;
    for (auto a : getVertice(idVertice)->arestas) {
        lista.push_front(a->destino->id);
    }
    return lista;
}

void Grafo::Dijkstra(int idVertice) {
    if (!direcionado)
    {
        std::cout << "O grafo deve ser direcionado\n";
        return;
    }
    if(!arestasPonderadas)
    {
        std::cout << "O grafo ter arestas ponderadas\n";
        return;
    }
    Vertice *v = getVertice(idVertice);
    if (v == nullptr)
    {
        std::cout << "Nao existe no grafo vertice com o id especificado (" << idVertice << ")\n";
        return;
    }
    for(auto a: vertices){
        for (auto ares : a->arestas) {
            if(a->peso < 0){
                std::cout << "O peso das sarestas não pode ser negativo\n";
                return;
            }
        }
    }
    v->distancia = 0; // Distância do vértice de origem para ele mesmo é 0

    while (auxDijkstra()) {
        // Pegar o vértice com a menor distância
        Vertice* verticeAtual = nullptr;
        int menorDistancia = std::numeric_limits<int>::max();
        for (auto vert : vertices) {
            if (vert->aberto && vert->distancia < menorDistancia) {
                menorDistancia = vert->distancia;
                verticeAtual = vert;
            }
        }

        // Verificar se encontrou um vértice válido
        if (verticeAtual == nullptr) {
            break;
        }

        verticeAtual->aberto = false; // Marcar o vértice atual como fechado
        std::cout << verticeAtual->id << " distancia: " << verticeAtual->distancia << "\n";
        
        // Atualizar as distâncias dos vértices adjacentes
        for(auto a: vertices){
            for (auto ares : a->arestas) {
            Vertice* destino = ares->destino;
            if (destino->aberto && verticeAtual->distancia + ares->peso < destino->distancia) {
                destino->distancia = a->distancia + ares->peso;
            }
        }
        }
    }
}
    
