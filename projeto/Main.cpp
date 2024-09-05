#include <iostream>
#include <fstream>
#include "Reader.hpp"
#include "Grafo.hpp"

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
    Grafo* grafo = Reader::readGrafo(input);
    input.close();
    grafo->print();
    std::ofstream output(arquivoSaida);
    grafo->print(output);
    output.close();
    delete grafo;
}