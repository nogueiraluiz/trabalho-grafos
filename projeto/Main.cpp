#include <iostream>
#include <fstream>
#include "Grafo.hpp"


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
    std::vector<int> subconjunto =  {1, 2, 3, 4, 5, 6}; //subconjunto de vértices
    g->kruskal(subconjunto);
    g->print();
    g->print(output);
    input.close();
    output.close();
    return 0;
}
