#include <iostream>
#include <fstream>
#include "Grafo.hpp"
#include <string>

void opcoes(Grafo* g, std::ofstream& output)
{
    int opcao = 0;
    while (opcao != 10) {
        std::cout << "Escolha uma opção:\n";
        std::cout << "1 - Fecho transitivo direto\n";
        std::cout << "2 - Fecho transitivo indireto\n";
        std::cout << "3 - Caminho mínimo por Dijkstra\n";
        std::cout << "4 - Caminho mínimo por Floyd\n";
        std::cout << "5 - AGM (Prim)\n";
        std::cout << "6 - AGM (Kruskal)\n";
        std::cout << "7 - Árvore de caminhamento em profundidade\n";
        std::cout << "8 - Análise de excentricidade\n";
        std::cout << "9 - Conjunto dos vértices de articulação\n";
        std::cout << "10 - Sair" << std::endl;
        std::cin >> opcao;
        switch (opcao)
        {
        case 1:
        {
            int idVert;
            std::cout << "Escolha id do vértice que deseja usar a funcionalidade Fecho transitivo direto:";
            std::cin >> idVert;
            Grafo *fecho = g->fechoTransitivoDireto(idVert);
            if (fecho != nullptr)
            {
                int r;
                while (r != 1 || r != 2)
                {
                    std::cout << "Deseja salvar no arquivo? Sim-1 Não-2";
                    std::cin >> r;
                    std::cout << std::endl;
                }
                if (r == 1)
                {
                    g->print(output);
                }
            }
            delete fecho;
            break;
        }
        default:
        {
            std::cout << "Opção inválida\n";
            break;
        }
        }
    }
}

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
    opcoes(g, output);
    input.close();
    output.close();
    return 0;
}
