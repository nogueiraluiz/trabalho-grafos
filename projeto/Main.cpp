#include <iostream>
#include <fstream>
#include <chrono>
#include "Reader.hpp"
#include "Grafo.hpp"
#include "Algoritmos.hpp"

int main(int argc, char *argv[])
{
    char* arquivoInstancia = argv[1];
    std::cout << "Arquivo de instância: " << arquivoInstancia << std::endl;
    char* arquivoSaida = argv[2];
    std::ifstream input(arquivoInstancia);
    int particoes = Reader::getNumeroDeParticoes(input);
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
    auto start = std::chrono::high_resolution_clock::now();
    Grafo *solucao = Algoritmos::gulosoRandomizado(grafo, particoes, 0.5f);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Tempo de execução: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
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