#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include "Grafo.hpp"
#include "Printer.hpp"

/**
 * Construtor que instancia um grafo de acordo com as arestas definidas em um arquivo .dat lido como argumento para a execução.
*/
Grafo::Grafo(std::ifstream &arquivoInstancia, bool direcionado,  bool ponderadoArestas, bool ponderadoVertices)
{
    this->direcionado = direcionado;
    this->ponderadoArestas = ponderadoArestas;
    this->ponderadoVertices = ponderadoVertices;
    std::string linha;
    getline(arquivoInstancia, linha);
    while (getline(arquivoInstancia, linha)) {
        std::stringstream stream(linha);
        std::string item;
        std::vector<int> itens;
        while (getline(stream, item, ' ')) {
            itens.push_back(std::stoi(item));
        }
        int idVerticeA = itens[0];
        int idVerticeB = itens[1];
        int pesoAresta = itens[2];
        adicionaAresta(idVerticeA, idVerticeB, pesoAresta);
    }
}

/**
 * Construtor mais básico, não define quaisquer elementos dos conjuntos de vértices e de arestas.
*/
Grafo::Grafo(bool direcionado, bool ponderadoVertices, bool ponderadoArestas)
{
    this->direcionado = direcionado;
    this->ponderadoArestas = ponderadoArestas;
    this->ponderadoVertices = ponderadoVertices;
}

Grafo::~Grafo()
{
    for (const auto& [id, vertice] : vertices) {
        Vertice *v = vertice;
        for (const auto& [idDestino, aresta] : vertice->arestas)
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
    Printer::printGrafo(*this);
}

/**
 * Método público para escrever o grafo num arquivo especificado pela referência
 * à ofstream passada como parâmetro. 
*/
void Grafo::print(std::ofstream &output)
{
    Printer::printGrafo(*this, output);
}

/**
 * Adiciona um novo vértice ao grafo, caso não exista um com o id especificado.
*/
void Grafo::adicionaVertice(int idVertice, int peso)
{
    if (vertices.count(idVertice) > 0) {
        return; // o vértice já existe
    }
    Vertice *v = new Vertice;
    v->id = idVertice;
    v->peso = peso;
    vertices[idVertice] = v;
}

/**
 * Verifica se dois vértices A e B são vizinhos, ou seja, se há aresta de A para B. 
*/
bool Grafo::saoVizinhos(int idVerticeA, int idVerticeB)
{
    const auto& iterator = vertices.find(idVerticeA);
    if (iterator == vertices.end()) {
        return false;
    }
    Vertice *a = iterator->second;  
    for (const auto& [idDestino, aresta] : a->arestas)
    {
        if (idDestino == idVerticeB)
        {
            return true;
        }
    }
    return false;
}

void Grafo::adicionaAdjacencias(int idVerticeA, int idVerticeB, int peso)
{
    Vertice* verticeA = vertices.find(idVerticeA)->second;
    Aresta* aresta = new Aresta;
    Vertice* verticeB = vertices.find(idVerticeB)->second;
    aresta->destino = verticeB;
    aresta->peso = peso;
    if (verticeA != nullptr)
    {
        verticeA->arestas[idVerticeB] = aresta;
    }
}

void Grafo::adicionaAresta(int idVerticeA, int idVerticeB, int peso)
{
    if (saoVizinhos(idVerticeA, idVerticeB))
    {
        return; // já existe aresta pelo par
    }
    adicionaVertice(idVerticeA);
    adicionaVertice(idVerticeB);
    adicionaAdjacencias(idVerticeA, idVerticeB, peso);
    if (!direcionado) {
        adicionaAdjacencias(idVerticeB, idVerticeA, peso);
    }
}

void Grafo::removeAresta(int idVerticeA, int idVerticeB)
{
    const auto& iteratorA = vertices.find(idVerticeA);
    bool removido;
    if (iteratorA == vertices.end())
    {
        return; // A não existe no conjunto de vértices
    }
    const auto& iteratorB = vertices.find(idVerticeA);
    if (iteratorB == vertices.end())
    {
        return; // B não existe no conjunto de vértices
    }
    Vertice *a = iteratorA->second;
    for (const auto& [idDestino, aresta] : a->arestas)
    {
        if (idDestino == idVerticeB)
        {
            a->arestas.erase(idDestino);
            delete aresta;
            removido = true;
            break;
        }
    }
    if (!direcionado)
    {
        Vertice *b = iteratorB->second;
        for (const auto& [idDestino, aresta]: b->arestas)
        {
            if (idDestino == idVerticeA)
            {
                b->arestas.erase(idDestino);
                delete aresta;
                return;
            }
        }
    }
    if (!removido) {
        std::cout << "Não foi encontrada aresta definida pelos vértices " << idVerticeA 
                << "e " << idVerticeB << '\n';
    }
}

std::map<int, Vertice*> Grafo::getMapaVertices() { return vertices; }

bool Grafo::getOpcaoVertices() { return this->ponderadoVertices; }

bool Grafo::getOpcaoArestas() { return this->ponderadoArestas; }

bool Grafo::getOpcaoDirecionado() { return this->direcionado; }

void Grafo::removeVertice(int idVertice)
{
    const auto& iterator = vertices.find(idVertice);
    if (iterator == vertices.end())
    {
        return; // vértice buscado não existe
    }
    for (const auto& [idAtual, idDestino] : vertices)
    {
        if (idAtual == idVertice) {
            continue; // não deve haver self-loop no conjunto de arestas
        }
        removeAresta(idVertice, idAtual);
        removeAresta(idAtual, idVertice);
    }
    vertices.erase(idVertice);
    delete iterator->second;
}
