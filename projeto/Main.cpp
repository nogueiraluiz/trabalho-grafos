#include <iostream>
#include <fstream>
#include "Grafo.hpp"
#include <string>


int main(int argc, char *argv[])
{
    if (argc < 6) {
        return 1; // número insuficiente de argumentos
    }
    char *arquivoInput = argv[1];
    char *arquivoOutput = argv[2];
    int direcionado = std::stoi(argv[3]);
    int arestasPonderadas = std::stoi(argv[4]);
    int verticesPonderados = std::stoi(argv[5]);
    std::ifstream input(arquivoInput);
    std::ofstream output;
    output.open(arquivoOutput);
    Grafo *g = new Grafo(input, direcionado, arestasPonderadas, verticesPonderados);
    g->print();
    g->print(output);
    g->Dijkstra(1);
    input.close();
    output.close();
    return 0;
}
