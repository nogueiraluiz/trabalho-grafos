#include "Printer.hpp"

/**
 * Retorna a string que representa uma aresta a ser inserida no arquivo de saída de acordo com as
 * configurações específicadas.
*/
void Printer::printGrafo(std::list<Vertice *> &vertices, bool direcionado, bool arestasPonderadas, bool verticesPonderados, std::ofstream &arquivo)
{
    std::string cabecalho = direcionado ? "digraph G {\n\n" : "graph G {\n\n";
    arquivo << cabecalho;
    if (verticesPonderados) {
        printVerticesPonderados(vertices, arquivo);
    }
    if (direcionado) {
        printArestasDirecionadas(vertices, arestasPonderadas, arquivo);
    } else {
        printArestasNaoDirecionadas(vertices, arestasPonderadas, arquivo);
    }
    arquivo << "\n\n}";
}

void Printer::printGrafo(std::list<Vertice *> &vertices, bool direcionado, bool arestasPonderadas, bool verticesPonderados)
{
    std::cout << "O grafo possui " << vertices.size() << " vertices\n";
    for (const Vertice* vertice : vertices)
    {
        std::cout << vertice->id << " -> { ";
        for (const Aresta* aresta : vertice->arestas)
        {
            std::cout << aresta->destino->id << " "; 
        }
        std::cout << "}\n";

    }
}

/**
 * Imprime os vértices para o arquivo definindo o label de cada um como "<id> <(peso)>",
 * no caso de grafos com vértices ponderados.
*/
void Printer::printVerticesPonderados(std::list<Vertice *> &vertices, std::ofstream &arquivo)
{
    for (const Vertice* vertice : vertices)
    {
        arquivo << '\t' << vertice->id << "[label=" << vertice->id << " (" << vertice->peso << ")];\n"; 
    }
    arquivo << '\n';
}

std::string Printer::getRepresentacaoAresta(int id, std::string separador, int idDestino, const Aresta &aresta, bool ponderada)
{
    std::stringstream formatoAresta;
    formatoAresta << id << separador << idDestino;
    if (ponderada) {
        formatoAresta << " [label=" << aresta.peso << "]";
    }
    formatoAresta << ";\n";
    return formatoAresta.str();
}

void Printer::printArestasDirecionadas(std::list<Vertice *> &vertices, bool arestasPonderadas, std::ofstream &arquivo)
{
    std::string separador = " -> ";
    for (const Vertice* vertice : vertices)
    {
        for (const Aresta* aresta : vertice->arestas)
        {
            arquivo << '\t' << getRepresentacaoAresta(vertice->id, separador, aresta->destino->id, *aresta, arestasPonderadas);
        }
    }
}

void Printer::printArestasNaoDirecionadas(std::list<Vertice *> &vertices, bool arestasPonderadas, std::ofstream &arquivo)
{
    std::string separador = " -- ";
    std::set<int> impressos;
    for (const Vertice* vertice : vertices)
    {
        if (std::count(impressos.begin(), impressos.end(), vertice->id)) {
            continue; // evita impressão duplicada de uma aresta que tem como uma das extremidades o vértice com tal id
        }
        for (const Aresta* aresta : vertice->arestas)
        {
            arquivo << '\t' << getRepresentacaoAresta(vertice->id, separador, aresta->destino->id, *aresta, arestasPonderadas);
            impressos.insert(aresta->destino->id);
        }
    }
}
