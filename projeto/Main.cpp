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
    int ponderadoArestas = std::stoi(argv[4]);
    int ponderadoVertices = std::stoi(argv[5]);
    std::ifstream arquivo(arquivoInput);
    Grafo *g = new Grafo(arquivo, direcionado, ponderadoVertices, ponderadoArestas);
    g->print();
    arquivo.close();
    return 0;
}
