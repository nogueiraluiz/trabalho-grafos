#include <iostream>
#include <fstream>
#include "Reader.hpp"
#include "Grafo.hpp"
#include "Algoritmos.hpp"

int main(int argc, char *argv[])
{
    char* arquivoInstancia = argv[1];
    std::cout << "Arquivo de instância: " << arquivoInstancia << std::endl;
    char* arquivoSaida = argv[2];
    std::ifstream input(arquivoInstancia);
    if (!input.is_open())
    {
        std::cout << "Erro ao abrir o arquivo de instância\n";
        return 1;
    }
    std::cout << "Lendo instância\n";
    Grafo* grafo = Reader::readGrafo(input);
    std::cout << "Instância lida\n";
    input.close();
    for (Vertice* vertice : grafo->vertices)
    {
        std::cout << "Vertice: " << vertice->id << " Peso: " << vertice->peso << std::endl;
        for (Aresta* aresta = vertice->arestas; aresta != nullptr; aresta = aresta->prox)
        {
            if (aresta->origem->id < aresta->destino->id)
            std::cout << "Aresta: " << aresta->origem->id << " " << aresta->destino->id << std::endl;
        }
    }
    std::cout << "Criando solução por algoritmo construtivo\n";
    Grafo *solucao = Algoritmos::construtivo(grafo, std::stoi(argv[3]));
    if (solucao == nullptr)
    {
        std::cout << "Solução não encontrada\n";
        return 1;
    }
    std::ofstream output(arquivoSaida);
    solucao->print(output);
    output.close();
    delete solucao;
    delete grafo;
}