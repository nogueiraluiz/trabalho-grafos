#include "Printer.hpp"

namespace Printer
{

    /**
     * Imprime os vértices para o arquivo definindo o label de cada um como "<id> <(peso)>",
     * no caso de grafos com vértices ponderados.
    */
    void printVerticesPonderados(std::map<int, Vertice*> vertices, std::ofstream &arquivo)
    {
        for (const auto& [id, vertice] : vertices) {
            arquivo << '\t' << id << "[label=" << id << " (" << vertice->peso << ")];\n";
        }
        arquivo << '\n';
    }

    /**
     * Retorna a string que representa uma aresta a ser inserida no arquivo de saída de acordo com as
     * configurações específicadas.
    */
    std::string getRepresentacaoAresta(int id, std::string separador, int idDestino, Aresta &aresta, bool ponderada)
    {
        std::stringstream formatoAresta;
        formatoAresta << id << separador << idDestino;
        if (ponderada) {
            formatoAresta << " [label=" << aresta.peso << "]";
        }
        formatoAresta << ";\n";
        return formatoAresta.str();
    }

    /**
     * Imprime as arestas de um drigrafo.
    */
    void printArestasDirecionadas(std::map<int, Vertice*> vertices, bool ponderadoNasArestas, std::ofstream &arquivo)
    {
        std::string separador = " -> ";
        for (const auto& [id, vertice] : vertices) {
            for (const auto& [idDestino, aresta] : vertice->arestas) {
                arquivo << '\t' << getRepresentacaoAresta(id, separador, idDestino, *aresta, ponderadoNasArestas);
            }
        }
    }

    /**
     * Imprime as arestas de um grafo cuidando para que não haja repetição, visto que na classe Grafo arestas não direcionadas
     * são tratadas como um par de arestas direcionadas com origem e destino invertidas.
    */
    void printArestasNaoDirecionadas(std::map<int, Vertice*> vertices, bool ponderadoNasArestas, std::ofstream &arquivo)
    {
        std::string separador = " -- ";
        std::set<int> impressos;
        for (const auto& [id, vertice] : vertices) {
            if (std::count(impressos.begin(), impressos.end(), id)) {
                continue;
            }
            for (const auto& [idDestino, aresta] : vertice->arestas) {
                arquivo << '\t' << getRepresentacaoAresta(id, separador, idDestino, *aresta, ponderadoNasArestas);
            }
            impressos.insert(id);
        }
    }

    /**
     * Especificação da impressão de um vértice para um arquivo de saída em DOT Language.
    */
    void printGrafo(Grafo &g, std::ofstream &arquivo)
    {
        std::string cabecalho = g.getOpcaoDirecionado() ? "digraph G {\n\n" : "graph G {\n\n";
        arquivo << cabecalho;
        std::map<int, Vertice*> vertices = g.getMapaVertices();
        if (g.getOpcaoVertices())
        {
            printVerticesPonderados(vertices, arquivo);
        }
        if (g.getOpcaoDirecionado()) {
            printArestasDirecionadas(vertices, g.getOpcaoArestas(), arquivo);
        } else {
            printArestasNaoDirecionadas(vertices, g.getOpcaoArestas(), arquivo);
        }
        arquivo << "\n\n}";
    }

    /**
     * Imprime o grafo no console como lista de adjacências.
    */
    void printGrafo(Grafo &g)
    {
        std::map<int, Vertice*> vertices = g.getMapaVertices();
        std::cout << "Número de vértices: " << vertices.size() << '\n';
        for (const auto& [id, vertice] : vertices)
        {
            std::cout << id << " -> { ";
            for (const auto& [idDestino, vertice] : vertice->arestas)
            {
                std::cout << idDestino << " ";
            }
        std::cout << "}\n";
        } 
    }
}
