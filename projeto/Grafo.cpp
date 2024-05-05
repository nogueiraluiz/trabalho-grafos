#include <iostream>

#include <vector>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include "Grafo.hpp"

Grafo::Grafo(std::ifstream &arquivoInstancia, bool direcionado,  bool ponderadoArestas, bool ponderadoVertices)
{
    this->direcionado = direcionado;
    this->ponderadoArestas = ponderadoArestas;
    this->ponderadoVertices = ponderadoVertices;
    std::string linha;
    getline(arquivoInstancia, linha);
    int tamanhoInicialEsperado = std::stoi(linha);
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
        adicionaAresta(idVerticeA, idVerticeB, pesoAresta); // para grafos direcionados e ponderados nas arestas a princípio
    }
}

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

// Verifica se um vértice já existe na lista de vértices do grafo e, caso não esteja, o adiciona
// NOTA: funcionando para ordenados e não ordenados, vértices ponderados ou não
void Grafo::adicionaVertice(int idVertice, int peso)
{
    if (vertices.count(idVertice) > 0) {
        return; // checa se já existe
    }
    Vertice *v = new Vertice;
    v->id = idVertice;
    v->peso = peso;
    vertices[idVertice] = v;
}

// Válido para grafos direcionados ou não.
// idVerticeA deve ser referente ao ponto de partida da aresta em casos de grafos direcionados.
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
    // TODO: Verificar se a aresta já existe
    if (saoVizinhos(idVerticeA, idVerticeB))
    {
        return;
    }
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

// Funcionando para direcionados ou não
void Grafo::adicionaAresta(int idVerticeA, int idVerticeB, int peso)
{
    adicionaVertice(idVerticeA);
    adicionaVertice(idVerticeB);
    if (!direcionado) {
        adicionaAdjacencias(idVerticeB, idVerticeA, peso);
    }
    adicionaAdjacencias(idVerticeA, idVerticeB, peso);
}

// Caso exista o vértice correspondente a idVerticeA, busca em suas arestas incidentes
// uma o conecte ao vértice correspondente a idVerticeB, a remove e libera a memória
// alocada, caso a encontre.
// Se o grafo em questão não for direcionado, remove a aresta criada no caminho contrário.
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
        if (idAtual != idVertice)
        {
            removeAresta(idVertice, idAtual);
            removeAresta(idAtual, idVertice);
        }
    }
    vertices.erase(idVertice);
    delete itr->second;
}
