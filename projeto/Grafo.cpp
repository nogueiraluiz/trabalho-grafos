#include <iostream>

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "Grafo.hpp"

// Todas as implementações até o momento foram feitas com grafos direcionados e ponderados nas arestas em mente.

Grafo::Grafo(std::ifstream &arquivoInstancia, bool direcionado, bool ponderadoVertices, bool ponderadoArestas)
{
    direcionado = direcionado;
    ponderadoVertices = ponderadoVertices;
    ponderadoArestas = ponderadoArestas;
    std::string linha;
    getline(arquivoInstancia, linha);
    int tamanhoInicialEsperado = std::stoi(linha);
    numArestas = 0;
    numVertices = 0;
    while (getline(arquivoInstancia, linha)) {
        std::stringstream stream(linha);
        std::string item;
        std::vector<int> itens;
        while (getline(stream, item, ' ')) {
            itens.push_back(std::stoi(item));
        }
        std::cout << "Lendo linha: " << linha << "\n";
        int idVerticeA = itens.at(0);
        int idVerticeB = itens.at(1);
        int pesoAresta = itens.at(2);
        adicionaAresta(idVerticeA, idVerticeB, pesoAresta); // para grafos direcionados e ponderados nas arestas a princípio
    }
}

Grafo::~Grafo()
{
    for (int i = 0; i < numVertices; i++)
    {
        Vertice *temp = vertices[i];
        delete temp;
    }
    for (int i = 0; i < numArestas; i++)
    {
        Aresta *temp = arestas[i];
        delete temp;
    }
}

void Grafo::print()
{
    std::cout << "Número de vértices: " << vertices.size() << '\n';
    for (Vertice *vertice : vertices)
    {
        std::cout << vertice->id << " -> ";
        for (Vertice *vizinho : vertice->adjacencias)
        {
            std::cout << vizinho->id << " ";
        }
        std::cout << '\n';
    } 
}

// Verifica se um vértice já existe na lista de vértices do grafo e, caso não esteja, o adiciona
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
    vertices.push_back(v);
    numVertices++;
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
    for (Vertice *vizinho : a->adjacencias)
    {
        if (vizinho->id == idVerticeB)
        {
            return true;
        }
    }
    return false;
}

void Grafo::adicionaAdjacencias(int idVerticeA, int idVerticeB)
{
    // TODO: Verificar 'direcionado' e, caso 'false', adicionar A às adjacências de B também
    if (saoVizinhos(idVerticeA, idVerticeB))
    {
        return;
    }
    Vertice* verticeA = getVertice(idVerticeA);
    Vertice* verticeB = getVertice(idVerticeB);
    if (verticeA != nullptr  && verticeB != nullptr)
    {
        verticeA->adjacencias.push_back(verticeB);
        verticeA->numArestas++;
    }
}

void Grafo::adicionaAresta(int idVerticeA, int idVerticeB, int peso)
{
    adicionaVertice(idVerticeA);
    adicionaVertice(idVerticeB);
    adicionaAdjacencias(idVerticeA, idVerticeB);;
    Aresta *e = new Aresta;
    e->idVerticeA = idVerticeA;
    e->idVerticeB = idVerticeB;
    arestas.push_back(e);
    numArestas++;
}

void Grafo::removeAresta(int idVerticeA, int idVeritceB)
{
    
}

void Grafo::removeVertice(int idVertice)
{

}
