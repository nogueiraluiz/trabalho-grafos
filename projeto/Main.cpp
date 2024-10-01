#include <iostream>
#include <string>
#include "Opcoes.hpp"

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Número insuficiente de argumentos\n";
        return 1;
    }
    if (std::stoi(argv[1]) == 1)
    {
        std::string arquivoInstancia = argv[2];
        std::string arquivoSaida = argv[3];
        int opcaoAlgoritmo = std::stoi(argv[4]);
        if (opcaoAlgoritmo < 0 || opcaoAlgoritmo > 2)
        {
            std::cout << "Opção inválida para algoritmo utilizado na constução de solução\n";
            return 1;
        }
        float alfa = argc == 6 ? std::stof(argv[5]) : 0;
        Opcoes::MGGPP(arquivoInstancia, arquivoSaida, opcaoAlgoritmo, alfa);
    } else if (std::stoi(argv[1]) == 2)
    {
        std::string arquivoInstancia = argv[2];
        std::string arquivoSaida = argv[3];
        int direcionado = std::stoi(argv[4]);
        int arestasPonderadas = std::stoi(argv[5]);
        int verticesPonderados = std::stoi(argv[6]);
        Opcoes::operacoesBasicas(arquivoInstancia, arquivoSaida, direcionado, arestasPonderadas, verticesPonderados);
    } else
    {
        std::cout << "Opção inválida de funcionalidade\n";
        return 1;
    }
}