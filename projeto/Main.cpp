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
    std::cout << "Criando solução por algoritmo construtivo\n";
    Grafo *solucao = Algoritmos::construtivo(grafo, 2);
    if (solucao == nullptr)
    {
        std::cout << "Solução não encontrada\n";
        return 1;
    }
    std::ofstream output(arquivoSaida);
    solucao->print(output);
    output.close();
    delete grafo;
}