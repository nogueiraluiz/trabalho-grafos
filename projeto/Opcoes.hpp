#include <string>
#include <iostream>
#include <fstream>
#include "Algoritmos.hpp"
#include "Grafo.hpp"
#include "Reader.hpp"
#include "Printer.hpp"

namespace Opcoes
{
    void MGGPP(std::string &arquivoInstancia, 
            std::string &arquivoSaida, 
            int opcaoAlgoritmo,
            float alfa);
    void operacoesBasicas(std::string &arquivoInstancia,
            std::string &arquivoSaida, 
            int direcionado, 
            int arestasPonderadas, 
            int verticesPonderados);
    void salvarResultado(Grafo *g, std::ofstream &output, bool profundidade);
    std::vector<int> lerSubconjunto();
    void opcoes(Grafo *g, std::ofstream &output);
} // namespace Options
