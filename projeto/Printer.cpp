#include "Printer.hpp"

/**
 * Retorna a string que representa uma aresta a ser inserida no arquivo de saída de acordo com as
 * configurações específicadas.
 */
void Printer::printGrafo(std::vector<Vertice *> &vertices, bool direcionado, bool arestasPonderadas, bool verticesPonderados, std::ofstream &arquivo)
{
    std::string cabecalho = direcionado ? "digraph G {\n\n" : "graph G {\n\n";
    arquivo << cabecalho;
    if (verticesPonderados)
    {
        printVerticesPonderados(vertices, arquivo);
    }
    if (direcionado)
    {
        printArestasDirecionadas(vertices, arestasPonderadas, arquivo);
    }
    else
    {
        printArestasNaoDirecionadas(vertices, arestasPonderadas, arquivo);
    }
    arquivo << "\n}\n\n";
}

void Printer::printGrafo(std::vector<Vertice *> &vertices, bool direcionado, bool arestasPonderadas, bool verticesPonderados)
{
    std::cout << "O grafo possui " << vertices.size() << " vertices\n";
    for (const Vertice *vertice : vertices)
    {
        std::cout << vertice->id << " -> { ";
        Aresta *aresta = vertice->arestas;
        while (aresta != nullptr)
        {
            std::cout << aresta->destino->id << " ";
            aresta = aresta->prox;
        }
        std::cout << "}\n";
    }
}

/**
 * Imprime os vértices para o arquivo definindo o label de cada um como "<id> <(peso)>",
 * no caso de grafos com vértices ponderados.
 */
void Printer::printVerticesPonderados(std::vector<Vertice *> &vertices, std::ofstream &arquivo)
{
    for (const Vertice *vertice : vertices)
    {
        arquivo << '\t' << vertice->id << "[label=" << vertice->id << " (" << vertice->peso << ")];\n";
    }
    arquivo << '\n';
}

std::string Printer::getRepresentacaoAresta(int id, std::string separador, int idDestino, const Aresta &aresta, bool ponderada)
{
    std::stringstream formatoAresta;
    formatoAresta << id << separador << idDestino;
    if (ponderada)
    {
        formatoAresta << " [label=" << aresta.peso << "]";
    }
    formatoAresta << ";\n";
    return formatoAresta.str();
}

void Printer::printArestasDirecionadas(std::vector<Vertice *> &vertices, bool arestasPonderadas, std::ofstream &arquivo)
{
    std::string separador = " -> ";
    for (const Vertice *vertice : vertices)
    {
        Aresta *aresta = vertice->arestas;
        while (aresta != nullptr)
        {
            arquivo << '\t' << getRepresentacaoAresta(vertice->id, separador, aresta->destino->id, *aresta, arestasPonderadas);
            aresta = aresta->prox;
        }
    }
}

void Printer::printArestasNaoDirecionadas(std::vector<Vertice *> &vertices, bool arestasPonderadas, std::ofstream &arquivo)
{
    std::string separador = " -- ";
    std::set<int> impressos;
    for (const Vertice *vertice : vertices)
    {
        Aresta *aresta = vertice->arestas;
        while (aresta != nullptr)
        {
            if (std::count(impressos.begin(), impressos.end(), aresta->destino->id)) 
            {
                aresta = aresta->prox;
                continue; // evita impressão duplicada de uma aresta que tem como uma das extremidades o vértice com tal id
            }
            arquivo << '\t' << getRepresentacaoAresta(vertice->id, separador, aresta->destino->id, *aresta, arestasPonderadas);
            aresta = aresta->prox;
        }
        impressos.insert(vertice->id);
    }
}

/**
 * Imprime apenas as arestas comuns percorridas no caminhamento em profundidade no terminal.
 */
void Printer:: printArvoreCaminhamento(std::vector<Vertice*>& vertices) 
{
    std::cout << "Árvore de caminhamento em profundidade:\n";
    for (Vertice *vertice : vertices) 
    {
        std::cout << vertice->id << " -> { ";
        Aresta *aresta = vertice->arestas;
        while (aresta != nullptr) 
        {
            if (aresta->peso != -1) 
            {
                std::cout << aresta->destino->id << " ";
            }
            aresta = aresta->prox;
        }
        std::cout << "}\n";
    }
}

std::string Printer::getRepresentacaoArestaRetorno(int idOrigem, std::string separador, int idDestino, const Aresta &aresta)
{
    std::stringstream formatoAresta;
    formatoAresta << idOrigem << separador << idDestino << " [style=dashed];\n";
    return formatoAresta.str();
}

/**
 * Imprime as arestas comuns e as arestas de retorno encontradas durante o caminhamento em profundidade
 * tendo como origem o nó raiz.
 * no arquivo de saída.
 */
void Printer:: printArvoreCaminhamento(std::vector<Vertice*>& vertices, std::ofstream &arquivo)
{
    arquivo << "digraph G {\n\n";
    for (Vertice* vertice : vertices)
    {
        Aresta* aresta = vertice->arestas;
        while (aresta != nullptr) {
            if (aresta->peso == -1)
            {
                arquivo << '\t' << getRepresentacaoArestaRetorno(vertice->id, " -> ", aresta->destino->id, *aresta);
            } else
            {
                arquivo << '\t' << getRepresentacaoAresta(vertice->id, " -> ", aresta->destino->id, *aresta, false);
            }
            aresta = aresta->prox;
        }
    }
    arquivo << "\n}\n\n";
}