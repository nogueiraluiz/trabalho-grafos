#include <iostream>

#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include "Grafo.hpp"

Grafo::Grafo(std::ifstream &arquivoInstancia, bool direcionado, bool ponderadoVertices, bool ponderadoArestas)
{
    this->direcionado = direcionado;
    this->ponderadoVertices = ponderadoVertices;
    this->ponderadoArestas = ponderadoArestas;
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

Grafo::~Grafo()
{
    for (Vertice *v : vertices)
    {
        for (Aresta *e : v->arestas)
        {
            delete e;
        }
        delete v;
    }
}

void Grafo::print()
{
    std::cout << "Número de vértices: " << vertices.size() << '\n';
    for (Vertice *vertice : vertices)
    {
        std::cout << vertice->id << " -> { ";
        for (Aresta *arestaIncidente : vertice->arestas)
        {
            std::cout << arestaIncidente->destino->id << " ";
        }
        std::cout << "}\n";
    } 
}

void Grafo::printDirecionado(std::ofstream &output)
{
    output << "digraph G {\n\n";
    for (Vertice *v : vertices)
    {
        for (Aresta *e : v->arestas)
        {
            output << '\t' << v->id << " -> " << e->destino->id;
            if (ponderadoArestas)
            {
                output << " [weight=" << e->peso << "]";
            }
            output << '\n';
        }
    }
    output << "\n}\n";
}

void Grafo::printNaoDirecionado(std::ofstream &output)
{
    output << "graph G {\n\n";
    std::vector<int> impressos; 
    for (Vertice *v : vertices)
    {
        for (Aresta *e : v->arestas)
        {   
            if (std::count(impressos.begin(), impressos.end(), e->destino->id))
            {
                continue;
            }
            output << '\t' << v->id << " -- " << e->destino->id;
            if (ponderadoArestas)
            {
                output << " [weight=" << e->peso << "]";
            }
            output << '\n';
        }
        impressos.push_back(v->id);
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
    for (Vertice *vertice : vertices)
    {
        if (vertice->id == idVertice)
        {
            return;
        }
    }
    Vertice *v = new Vertice;
    v->id = idVertice;
    v->peso = peso;
    vertices.push_back(v);
}

Vertice* Grafo::getVertice(int idVerticeAlvo)
{
    for (Vertice *vertice : vertices)
    {  
        if (vertice->id == idVerticeAlvo)
        {
            return vertice;
        }
    }
    return nullptr; // nao encontrado
}

// Válido para grafos direcionados ou não.
// idVerticeA deve ser referente ao ponto de partida da aresta em casos de grafos direcionados.
bool Grafo::saoVizinhos(int idVerticeA, int idVerticeB)
{
    Vertice *a = getVertice(idVerticeA);
    for (Aresta *aresta : a->arestas)
    {
        if (aresta->destino->id == idVerticeB)
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
    Vertice* verticeA = getVertice(idVerticeA);
    Aresta* aresta = new Aresta;
    Vertice* verticeB = getVertice(idVerticeB);
    aresta->destino = verticeB;
    aresta->peso = peso;
    if (verticeA != nullptr)
    {
        verticeA->arestas.push_back(aresta);
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
    Vertice *a = getVertice(idVerticeA);
    if (a == nullptr)
    {
        return; // A não existe no conjunto de vértices
    }
    Vertice *b = getVertice(idVerticeB);
    if (b == nullptr)
    {
        return; // B não existe no conjunto de vértices
    }
    for (Aresta* aresta : a->arestas)
    {
        if (aresta->destino->id == idVerticeB)
        {
            a->arestas.remove(aresta);
            delete aresta;
            if (direcionado)
            {
                return;
            }
            else
            {
                break;
            }
        }
    }
    if (!direcionado)
    {
        for (Aresta* aresta : b->arestas)
        {
            if (aresta->destino->id == idVerticeA)
            {
                b->arestas.remove(aresta);
                delete aresta;
                return;
            }
        }
    }
    std::cout << "Não foi encontrada aresta definida pelos vértices " << idVerticeA 
            << "e " << idVerticeB << '\n';
}

void Grafo::removeVertice(int idVertice)
{
    Vertice *v = getVertice(idVertice);
    if (v == nullptr)
    {
        return; // v não existe
    }
    for (Vertice *vertice : vertices)
    {
        if (vertice->id != idVertice)
        {
            removeAresta(idVertice, vertice->id);
            removeAresta(vertice->id, idVertice);
        }
    }
    vertices.remove(v);
    delete v;
}
