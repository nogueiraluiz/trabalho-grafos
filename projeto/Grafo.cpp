#include <iostream>

#include <vector>
#include <string>
#include <sstream>

#include "Grafo.hpp"

Grafo::Grafo(std::ifstream &arquivoInstancia, int optDirec, int optPV, int optPA)
{
    vertices = new std::vector<Vertice*>;
    direcionado = optDirec;
    ponderadoV = optPV;
    ponderadoA = optPA;
    std::string linha;
    getline(arquivoInstancia, linha);
    qtdVertices = std::stoi(linha);
    qtdArestas = 0;
    while (getline(arquivoInstancia, linha)) {
        std::stringstream stream(linha);
        std::string item;
        std::vector<int> itens;
        while (getline(stream, item, ' ')) {
            itens.push_back(std::stoi(item));
        }
        int idCabeca = itens.at(0);
        int idCauda = itens.at(1);
        int pesoAresta = itens.at(2);
        adicionaAresta(idCabeca, idCauda, pesoAresta);
    }
}

Grafo::~Grafo()
{

}

void Grafo::adicionaAresta(int idVerticeA, int idVerticeB, int peso) {
    Vertice* a = new Vertice;
    Vertice* b = new Vertice;
    if (!a || !b) {
        std::cout << "Falha na alocação" << '\n';
    }
    a->_id = idVerticeA;
    b->_id = idVerticeB;
    a->_adjacencias->push_back(b);
    b->_adjacencias->push_back(a);
    vertices->push_back(a);
    vertices->push_back(b);
    Aresta* e = new Aresta;
    if (!e) {
        std::cout << "Falha na alocação" << '\n';
        
    }
    e->_cabeca = a;
    e->_cauda = b;
    e->_peso = peso;
    arestas->push_back(e);
}

void Grafo::print() {
    std::cout << "O grafo possui " << vertices << '\n';
    for (int i = 0; i < vertices->size(); i++) {
        Vertice* vertice = vertices->at(i);
        std::cout << vertice->_id << " -> ";
        for (int j = 0; j < vertice->_adjacencias->size(); j++) {
            Vertice* vertice_adj = vertice->_adjacencias->at(j);
            if (j == vertice->_adjacencias->size())
                std::cout << vertice_adj->_id << '\n';
            std::cout << vertice_adj->_id << ", ";
        }
    }
}
