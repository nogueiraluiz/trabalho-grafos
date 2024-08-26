#include <iostream>
#include <fstream>
#include "Grafo.hpp"
#include "Printer.hpp"
#include <string>

/**
 * Função utilitária para salvar um grafo em um arquivo de saída caso seja essa
 * a vontade do usuário.
 */
void salvar(Grafo *g, std::ofstream &output, bool profundidade = false)
{
    if (g == nullptr)
    {
        return;
    }
    int opcao = -1;
    while (opcao != 1 && opcao != 2)
    {
        std::cout << "Deseja salvar o grafo no arquivo de saída? Sim-1 Não-2 ";
        std::cin >> opcao;
        std::cout << std::endl;
    }
    if (opcao == 1)
    {
        if (profundidade)
        {
            Printer::printArvoreCaminhamento(g->vertices, output);
        }
        else
        {
            g->print(output);
        }
    }
    delete g;
}

/**
 * Função utilitária para ler um subconjunto de vértices para utilização nas funcionalidades de AGM.
 */
std::vector<int> lerSubconjunto()
{
    std::vector<int> subconjunto;
    int n;
    do
    {
        std::cout << "Digite os id dos vertices que voce deseja na arvore geradora mínima:\n Digite -1 para parar\n";
        std::cin >> n;
        if (n != -1)
        {
            subconjunto.push_back(n);
        }
    } while (n != -1);
    return subconjunto;
}

/**
 * Função utilitária para exibir as opções disponíveis para o usuário.
 */
void opcoes(Grafo *g, std::ofstream &output)
{
    int opcao = 0;
    while (opcao != 10)
    {
        std::cout << "Escolha uma opcao (Digite somente numeos):\n";
        std::cout << "1 - Fecho transitivo direto\n";
        std::cout << "2 - Fecho transitivo indireto\n";
        std::cout << "3 - Caminho minimo por Dijkstra\n";
        std::cout << "4 - Caminho minimo por Floyd\n";
        std::cout << "5 - AGM (Prim)\n";
        std::cout << "6 - AGM (Kruskal)\n";
        std::cout << "7 - Arvore de caminhamento em profundidade\n";
        std::cout << "8 - Analise de excentricidade\n";
        std::cout << "9 - Conjunto dos vértices de articulacao\n";
        std::cout << "10 - Sair" << std::endl;
        std::cin >> opcao;
        switch (opcao)
        {
        case 1:
        {
            int idVertice;
            std::cout << "Escolha id do vertice que deseja usar a funcionalidade Fecho transitivo direto: ";
            std::cin >> idVertice;
            Grafo *fecho = g->fechoTransitivoDireto(idVertice);
            salvar(fecho, output);
            break;
        }
        case 2:
        {
            int idVertice;
            std::cout << "Escolha id do vertice que deseja usar a funcionalidade Fecho transitivo indireto: ";
            std::cin >> idVertice;
            Grafo *fecho = g->fechoTransitivoIndireto(idVertice);
            std::cout << std::endl;
            salvar(fecho, output);
            break;
        }
        case 3:
        {
            int idInicio;
            int idFinal;
            std::cout << "Escolha id do vertice para ser o início do caminho minimo: ";
            std::cin >> idInicio;
            std::cout << std::endl;
            std::cout << "Escolha id do vertice para ser o destino do caminho minimo: ";
            std::cin >> idFinal;
            std::cout << std::endl;
            Grafo *fecho = g->caminhoMinimoDijkstra(idInicio, idFinal);
            std::cout << std::endl;
            salvar(fecho, output);
            break;
        }
        case 4:
        {
            int idInicio;
            int idFinal;
            std::cout << "Escolha id do vértice para ser o início do caminho mínimo: ";
            std::cin >> idInicio;
            std::cout << std::endl;
            std::cout << "Escolha id do vértice para ser o destino do caminho mínimo: ";
            std::cin >> idFinal;
            std::cout << std::endl;
            Grafo *fecho = g->caminhoMinimoFloyd(idInicio, idFinal);
            salvar(fecho, output);
            break;
        }
        case 5:
        {
            std::vector<int> subconjunto = lerSubconjunto();
            Grafo *fecho = g->arvoreGeradoraMinimaPrim(subconjunto);
            salvar(fecho, output);
            break;
        }
        case 6:
        {
            std::vector<int> subconjunto = lerSubconjunto();
            Grafo *fecho = g->arvoreGeradoraMinimaKruskal(subconjunto);
            salvar(fecho, output);
            break;
        }
        case 7:
        {
            std::cout << "Digite o id do vertice para iniciar o caminhamento em profundidade:\n";
            int idVertice;
            std::cin >> idVertice;
            Grafo *fecho = g->caminhamentoProfundidade(idVertice);
            salvar(fecho, output, true);
            break;
        }
        case 8:
        {
            g->analiseExcentricidade();
            break;
        }
        case 9:
        {
            Grafo *articulacoes = g->verticesDeArticulacao();
            salvar(articulacoes, output);
        }
        case 10:
        {
            std::cout << "Saindo!\n";
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
    if (argc < 6)
    {
        std::cout << "Número insuficiente de argumentos\n";
        return 1;
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
    g->print();
    g->print(output);
    opcoes(g, output);
    input.close();
    output.close();
    return 0;
}
