#include "Algoritmos.hpp"
#include "Grafo.hpp"

int Algoritmos::gapAresta(Aresta *aresta)
{
    return abs(aresta->origem->peso - aresta->destino->peso);
}

std::vector<Aresta *> Algoritmos::coletaEOrdenaArestas(Grafo *grafo)
{
    std::vector<Aresta *> arestas;
    for (Vertice *vertice : grafo->vertices)
    {
        Aresta *aresta = vertice->arestas;
        while (aresta != nullptr)
        {
            if (aresta->origem->id > aresta->destino->id)
            {
                // evita repetições (arco de "ida" e de "volta")
                arestas.push_back(aresta);
            }
            aresta = aresta->prox;
        }
    }
    std::sort(arestas.begin(), arestas.end(), [](Aresta *a, Aresta *b)
              { return gapAresta(a) < gapAresta(b); });
    return arestas;
}

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

void Algoritmos::preencheFloresta(std::vector<std::vector<Aresta *>> &floresta, std::vector<Aresta *> &arestas,
                                  std::set<Vertice *> &visitados)
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
                visitados.insert(aresta->origem);
                visitados.insert(aresta->destino);
                arestas.erase(iter);
                break;
            }
            ++iter;
        }
        componente++;
    }
}

int Algoritmos::numeroDeAdjacencias(Aresta *aresta, std::vector<std::vector<Aresta *>> &particoes)
{
    int adjacencias = 0;
    for (int i = 0; i < particoes.size(); i++)
    {
        for (Aresta *e : particoes[i])
        {
            if (saoAdjacentes(e, aresta))
            {
                adjacencias++;
            }
        }
    }
    return adjacencias;
}

bool Algoritmos::saoAdjacentes(Aresta *e, Aresta *aresta)
{
    return e->origem == aresta->origem || e->origem == aresta->destino || e->destino == aresta->origem || e->destino == aresta->destino;
}

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
            if (pesoU < pesoMin)
            {
                pesoMin = pesoU;
            }
            if (pesoV < pesoMin)
            {
                pesoMin = pesoV;
            }
            candidataEntra = saoAdjacentes(candidata, aresta);
        }
        if (candidataEntra)
        {
            int pesoU = candidata->origem->peso;
            int pesoV = candidata->destino->peso;
            if (pesoU < pesoMin)
            {
                pesoMin = pesoU;
            }
            if (pesoV < pesoMin)
            {
                pesoMin = pesoV;
            }
            if (pesoU > pesoMax)
            {
                pesoMax = pesoU;
            }
            if (pesoV > pesoMax)
            {
                pesoMax = pesoV;
            }
        }
        gap += pesoMax - pesoMin;
    }
    return gap;
};

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
            if (pesoU < pesoMin)
            {
                pesoMin = pesoU;
            }
            if (pesoV < pesoMin)
            {
                pesoMin = pesoV;
            }
        }
        gap += pesoMax - pesoMin;
    }
    return gap;
}

void Algoritmos::adicionaNovaAresta(std::vector<std::vector<Aresta *>> &floresta,
                                    std::vector<Aresta *> &arestas,
                                    std::set<Vertice *> visitados)
{
    std::cout << "Adicionando nova aresta\n";
    int min = std::numeric_limits<int>::max();
    Aresta *arestaMin = nullptr;
    std::map<Aresta *, int> gapsAtualizados;
    std::vector<Aresta *>::iterator iter = arestas.begin();
    while (iter != arestas.end())
    {
        Aresta *aresta = *iter;
        int adjacencias = numeroDeAdjacencias(aresta, floresta);
        if (adjacencias == 2)
        {
            std::cout << "Aresta que não pode ser utilizada removida\n";
            iter = arestas.erase(iter); // remove da lista de candidatas a aresta que não pode ser utilizada
            ++iter;
            continue;
        }
        if (adjacencias == 0)
        {
            ++iter;
            continue;
        }
        std::cout << "Aresta candidata: " << aresta->origem->id << " -> " << aresta->destino->id << std::endl;
        int novoGap = calculaNovoGap(floresta, aresta);
        if (novoGap < min)
        {
            std::cout << "Nova melhor aresta encontrada\n";
            arestaMin = aresta;
            min = novoGap;
        }
        gapsAtualizados[aresta] = novoGap;
        ++iter;
    }
    if (arestaMin == nullptr)
    {
        return;
    }
    std::vector<Aresta *>::iterator it = std::find(arestas.begin(), arestas.end(), arestaMin);
    arestas.erase(it); // remove aresta utilizada da lista de candidatas
    for (std::vector<Aresta *> &componente : floresta)
    {
        for (Aresta *aresta : componente)
        {
            if (saoAdjacentes(aresta, arestaMin))
            {
                componente.push_back(arestaMin); // nesse ponto, qualquer aresta a ser adicionada só é adjacente a um único vértice
                visitados.insert(arestaMin->origem);
                visitados.insert(arestaMin->destino);
                return;
            }
        }
    }
}

Grafo *Algoritmos::construtivo(Grafo *grafo, int numeroParticoes)
{
    std::vector<Aresta *> arestas = coletaEOrdenaArestas(grafo);
    std::vector<std::vector<Aresta *>> floresta(numeroParticoes);
    for (int i = 0; i < numeroParticoes; i++)
    {
        floresta[i] = std::vector<Aresta *>();
    }
    std::set<Vertice *> visitados = std::set<Vertice *>();
    preencheFloresta(floresta, arestas, visitados);
    while (visitados.size() < grafo->vertices.size())
    {
        if (arestas.size() == 0)
        {
            return nullptr;
        }
        adicionaNovaAresta(floresta, arestas, visitados);
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
