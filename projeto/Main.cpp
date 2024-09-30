#include <iostream>
#include <fstream>
#include <chrono>
#include "Reader.hpp"
#include "Grafo.hpp"
#include "Algoritmos.hpp"

int main(int argc, char* argv[])
{
    char* arquivoInstancia = argv[1];
    std::ifstream input(arquivoInstancia);
    int particoes = Reader::getNumeroDeParticoes(input);
    Grafo* grafo = Reader::readGrafo(input);
    input.close();
    Grafo* solucao = Algoritmos::gulosoRandomizadoReativo(grafo, particoes);
    delete solucao;
    delete grafo;
}