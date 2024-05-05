#include <iostream>

#include <vector>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include "Grafo.hpp"

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
    std::cout << "Número de vértices: " << vertices.size() << '\n';
    for (const auto& [id, vertice] : vertices)
    {
        std::cout << id << " -> { ";
        for (const auto& [idDestino, vertice] : vertice->arestas)
        {
            std::cout << idDestino << " ";
        }
        std::cout << "}\n";
    } 
}

/**
 * Escreve para o ofstream especificado a saída de um grafo direcionado para visualização seguindo a linguagem DOT.
*/
void Grafo::printDirecionado(std::ofstream &output)
{
    output << "digraph G {\n\n";
    for (const auto& [id, vertice] : vertices)
    {
        for (const auto& [idDestino, aresta] : vertice->arestas)
        {
            output << '\t' << id << " -> " << idDestino;
            if (ponderadoArestas)
            {
                output << " [weight=" << aresta->peso << "]";
            }
            output << '\n';
        }
    }
    output << "\n}\n";
}


/**
 * Escreve para o ofstream especificado a saída de um grafo não direcionado para
 * visualização seguindo a linguagem DOT.
*/
void Grafo::printNaoDirecionado(std::ofstream &output)
{
    output << "graph G {\n\n";
    std::set<int> impressos; 
    for (const auto& [id, vertice] : vertices)
    {
        for (const auto& [idVizinho, aresta] : vertice->arestas)
        {   
            if (std::count(impressos.begin(), impressos.end(), idVizinho))
            {
                continue;
            }
            output << '\t' << id << " -- " << idVizinho;
            if (ponderadoArestas)
            {
                output << " [weight=" << aresta->peso << "]";
            }
            output << '\n';
        }
        impressos.insert(id);
    }
    output << "\n}\n";
}

/**
 * Método público para escrever o grafo num arquivo especificado pela referência
 * à ofstream passada como parâmetro. 
*/
void Grafo::print(std::ofstream &output)
{
    if (direcionado)
    {
        printDirecionado(output);
    }
    else
    {
        printNaoDirecionado(output);
    }
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
    const auto& itr = vertices.find(idVerticeA);
    if (itr == vertices.end()) {
        return false;
    }
    Vertice *a = itr->second;  
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
    const auto& itrA = vertices.find(idVerticeA);
    bool removido;
    if (itrA == vertices.end())
    {
        return; // A não existe no conjunto de vértices
    }
    const auto& itrB = vertices.find(idVerticeA);
    if (itrB == vertices.end())
    {
        return; // A não existe no conjunto de vértices
    }
    Vertice *a = itrA->second;
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
        Vertice *b = itrB->second;
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

void Grafo::removeVertice(int idVertice)
{
    const auto& itr = vertices.find(idVertice);
    if (itr == vertices.end())
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
    delete itr->second;
}
