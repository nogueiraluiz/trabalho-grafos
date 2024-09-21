#include "Algoritmos.hpp"
#include "Grafo.hpp"

int Algoritmos::gapAresta(Aresta *aresta) // PRONTO
{
    return abs(aresta->origem->peso - aresta->destino->peso);
}

std::vector<Aresta *> Algoritmos::coletaEOrdenaArestas(Grafo *grafo) // PRONTO
{
    std::vector<Aresta *> arestas;
    for (Vertice *vertice : grafo->vertices)
    {
        Aresta *aresta = vertice->arestas;
        while (aresta != nullptr)
        {
            if (aresta->origem->id < aresta->destino->id) // evita repetir arco de ida e volta
            {
                arestas.push_back(aresta);
            }
            aresta = aresta->prox;
        }
    }
    std::sort(arestas.begin(), arestas.end(), [](Aresta *a, Aresta *b)
              { return gapAresta(a) < gapAresta(b); });
    return arestas;
}

/**
 * Verifica se uma aresta é adjacente a alguma aresta na floresta.
 */
bool Algoritmos::isAdjacenteAFloresta(Aresta *aresta, std::vector<std::vector<Aresta *>> &floresta)
{
    for (std::vector<Aresta *> &componente : floresta)
    {
        for (Aresta *e : componente)
        {
            if (saoAdjacentes(e, aresta))
            {
                return true;
            }
        }
    }
    return false;
}

/**
 * Preenche a floresta com uma aresta em cada árvore.
 */
void Algoritmos::preencheFloresta(std::vector<std::vector<Aresta *>> &floresta, std::vector<Aresta *> &arestas,
                                  std::set<int> &visitados) // PRONTO
{
    int componente = 0;
    while (componente != floresta.size())
    {
        std::vector<Aresta *>::iterator iter = arestas.begin();
        while (iter != arestas.end())
        {
            Aresta *aresta = *iter;
            bool adjacente = isAdjacenteAFloresta(aresta, floresta);
            if (!adjacente)
            {
                floresta[componente].push_back(aresta);
                visitados.insert(aresta->origem->id);
                visitados.insert(aresta->destino->id);
                arestas.erase(iter);
                break;
            }
            ++iter;
        }
        componente++;
    }
}

/**
 * Calcula o número de adjacências de uma aresta em um conjunto de partições.
 */
int Algoritmos::numeroDeAdjacencias(Aresta *aresta, std::vector<std::vector<Aresta *>> &particoes)
{
    int adjacencias = 0;
    for (int i = 0; i < particoes.size(); i++)
    {
        for (Aresta *e : particoes[i])
        {
            if (adjacencias == 2)
            {
                return adjacencias;
            }
            if (saoAdjacentes(e, aresta))
            {
                adjacencias++;
                continue;
            }
        }
    }
    return adjacencias;
}

bool Algoritmos::saoAdjacentes(Aresta *e, Aresta *aresta)
{
    return e->origem == aresta->origem || e->origem == aresta->destino || e->destino == aresta->origem || e->destino == aresta->destino;
}

/**
 * Calcula o novo gap para uma floresta de componentes após a adição de uma aresta candidata.
 */
int Algoritmos::calculaNovoGap(std::vector<std::vector<Aresta *>> &floresta, Aresta *candidata)
{
    int gap = 0;
    for (std::vector<Aresta *> &componente : floresta)
    {
        int pesoMin = std::numeric_limits<int>::max();
        int pesoMax = std::numeric_limits<int>::min();
        bool candidataEntra = false;
        for (Aresta *aresta : componente)
        {
            int pesoU = aresta->origem->peso;
            int pesoV = aresta->destino->peso;
            int minAresta = std::min(pesoU, pesoV);
            int maxAresta = std::max(pesoU, pesoV);
            if (minAresta < pesoMin)
            {
                pesoMin = minAresta;
            }
            if (maxAresta > pesoMax)
            {
                pesoMax = maxAresta;
            }
            candidataEntra = saoAdjacentes(candidata, aresta);
        }
        if (candidataEntra)
        {
            int pesoU = candidata->origem->peso;
            int pesoV = candidata->destino->peso;
            int minAresta = std::min(pesoU, pesoV);
            int maxAresta = std::max(pesoU, pesoV);
            if (minAresta < pesoMin)
            {
                pesoMin = minAresta;
            }
            if (maxAresta > pesoMax)
            {
                pesoMax = maxAresta;
            }
        }
        gap += pesoMax - pesoMin;
    }
    return gap;
};

/**
 * Calcula o somatório dos gaps das partições.
 */
int Algoritmos::calculaGap(std::vector<std::vector<Aresta *>> &floresta)
{
    int gap = 0;
    for (int i = 0; i < floresta.size(); i++)
    {
        int pesoMin = std::numeric_limits<int>::max();
        int pesoMax = std::numeric_limits<int>::min();
        for (Aresta *aresta : floresta[i])
        {
            int pesoU = aresta->origem->peso;
            int pesoV = aresta->destino->peso;
            int minAresta = std::min(pesoU, pesoV);
            int maxAresta = std::max(pesoU, pesoV);
            if (minAresta < pesoMin)
            {
                pesoMin = minAresta;
            }
            if (maxAresta > pesoMax)
            {
                pesoMax = maxAresta;
            }
        }
        gap += pesoMax - pesoMin;
    }
    return gap;
}

/**
 * Percorre a lista de arestas candidatas, removendo aquelas que não podem ser utilizadas
 * (ou seja, aquelas que são adjacentes a dois vértices da solução que está sendo construída).
 * Em seguida, calcula o novo gap para cada aresta e seleciona a aresta que, ao ser adicionada, resultaria no menor
 * somatório de gaps possível. A aresta selecionada é removida da lista de candidatas e adicionada
 * à partição correspondente na floresta. Os vértices que definem a aresta adicionada são marcados como visitados
 * (embora efetivamente apenas um seja marcado, pois a aresta adicionada é adjacente a um único vértice novo).
 * são marcados como visitados.
 */
void Algoritmos::adicionaNovaAresta(std::vector<std::vector<Aresta *>> &floresta,
                                    std::vector<Aresta *> &arestas,
                                    std::set<int> &visitados)
{
    std::cout << "Adicionando nova aresta" << std::endl;
    std::cout << "------------\n";
    std::cout << "Arestas existentes:\n";
    for (Aresta *aresta : arestas)
    {
        std::cout << "Aresta: " << aresta->origem->id << " " << aresta->destino->id << std::endl;
    }
    int min = std::numeric_limits<int>::max();
    Aresta *arestaMin = nullptr;
    std::map<Aresta *, int> gapsAtualizados;
    std::vector<Aresta *>::iterator iter = arestas.begin();
    while (iter != arestas.end())
    {
        if (arestas.size() == 0)
        {
            std::cout << "Nenhuma aresta candidata!!!!!" << std::endl;
        }
        Aresta *aresta = *iter;
        std::cout << "Checando aresta: " << aresta->origem->id << " " << aresta->destino->id << std::endl;
        int adjacencias = numeroDeAdjacencias(aresta, floresta);
        if (adjacencias == 2)
        {
            std::cout << "Aresta descartada" << std::endl;
            arestas.erase(iter); // não pode ser utilizada ou não provoca alteração na solução
        }
        else if (adjacencias == 0)
        {
            std::cout << "Aresta guardada para depois" << std::endl;  // não pode ser utilizada no momento, mas pode ser útil posteriormente
        }
        else
        {
            std::cout << "Aresta candidata" << std::endl;
            int novoGap = calculaNovoGap(floresta, aresta);
            if (novoGap < min)
            {
                arestaMin = aresta;
                min = novoGap;
            }
            gapsAtualizados[aresta] = novoGap;
        }
        iter++;
    }
    if (arestaMin == nullptr)
    {
        return;
    }
    std::cout << "Aresta selecionada: " << arestaMin->origem->id << " " << arestaMin->destino->id << std::endl;
    std::vector<Aresta *>::iterator it = std::find(arestas.begin(), arestas.end(), arestaMin);
    arestas.erase(it); // remove aresta utilizada da lista de candidatas
    for (std::vector<Aresta *> &componente : floresta)
    {
        for (Aresta *aresta : componente)
        {
            if (saoAdjacentes(aresta, arestaMin))
            {
                int pre = visitados.size();
                componente.push_back(arestaMin); // nesse ponto, qualquer aresta a ser adicionada só é adjacente a um único vértice
                visitados.insert(arestaMin->origem->id);
                visitados.insert(arestaMin->destino->id);
                int pos = visitados.size();
                if (pos - pre != 1)
                {
                    std::cout << "!!!\n";
                    std::cout << "ERRO: Adicionou mais de um vértice" << std::endl;
                    std::cout << "!!!\n";
                }
                return;
            }
        }
    }
}

/**
 * Algoritmo construtivo guloso para o MGGPP.
 */
Grafo *Algoritmos::construtivo(Grafo *grafo, int numeroParticoes)
{
    std::vector<Aresta *> arestas = coletaEOrdenaArestas(grafo);
    if (arestas.size() < numeroParticoes)
    {
        return nullptr;
    }
    std::cout << "------------\n";
    std::cout << "Arestas existentes:\n";
    for (Aresta *aresta : arestas)
    {
        std::cout << "Aresta: " << aresta->origem->id << " " << aresta->destino->id << std::endl;
    }
    std::cout << "------------\n";
    std::vector<std::vector<Aresta *>> floresta(numeroParticoes);
    for (int i = 0; i < numeroParticoes; i++)
    {
        floresta[i] = std::vector<Aresta *>();
    }
    std::set<int> visitados = std::set<int>();
    preencheFloresta(floresta, arestas, visitados);
    std::cout << "Floresta criada e preenchida com as seguintes arestas:\n";
    for (std::vector<Aresta *> &componente : floresta)
    {
        for (Aresta *aresta : componente)
        {
            std::cout << "Aresta: " << aresta->origem->id << " " << aresta->destino->id << std::endl;
        }
    }
    std::cout << "------------\n";
    std::cout << "Arestas existentes:\n";
    for (Aresta *aresta : arestas)
    {
        std::cout << "Aresta: " << aresta->origem->id << " " << aresta->destino->id << std::endl;
    }
    std::cout << "------------\nVISITADOS:\n";
    for (int vertice : visitados)
    {
        std::cout << "Visitados: " << vertice << std::endl;
    }
    while (visitados.size() != grafo->vertices.size())
    {
        adicionaNovaAresta(floresta, arestas, visitados);
        std::cout << "------------\nVISITADOS:\n";
        if (visitados.size() == 96) {
            std::cout << "Chegou: " << visitados.size() << std::endl;
        }
        for (int vertice : visitados)
        {
            std::cout << "Visitados: " << vertice << std::endl;
        }
    }
    int gap = calculaGap(floresta);
    std::cout << "Somatório dos gaps da solução encontrada = " << gap << std::endl;
    Grafo *solucao = new Grafo(0, 0, 1);
    for (int i = 0; i < floresta.size(); i++)
    {
        for (Aresta *aresta : floresta[i])
        {
            solucao->adicionaVertice(aresta->origem->id, aresta->origem->peso);
            solucao->adicionaVertice(aresta->destino->id, aresta->destino->peso);
            solucao->adicionaAresta(aresta->origem->id, aresta->destino->id);
        }
    }
    return solucao;
}
