#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include "limits.h"
#include "Grafo.hpp"
#include "Printer.hpp"

/**
 * Construtor que instancia um grafo de acordo com as arestas definidas em um arquivo .dat lido como argumento para a execução.
 */
Grafo::Grafo(std::ifstream &arquivoInstancia, bool direcionado, bool arestasPonderadas, bool verticesPonderados)
{
    this->direcionado = direcionado;
    this->arestasPonderadas = arestasPonderadas;
    this->verticesPonderados = verticesPonderados;
    std::string linha;
    getline(arquivoInstancia, linha);
    while (getline(arquivoInstancia, linha))
    {
        std::stringstream stream(linha);
        std::string item;
        std::vector<int> itens;
        while (getline(stream, item, ' '))
        {
            itens.push_back(std::stoi(item));
        }
        int idVerticeU = itens[0];
        int idVerticeV = itens[1];
        int pesoAresta = itens[2];
        adicionaAresta(idVerticeU, idVerticeV, pesoAresta);
    }
}

/**
 * Construtor mais básico, não define quaisquer elementos dos conjuntos de vértices e de arestas.
 */
Grafo::Grafo(bool direcionado, bool verticesPonderados, bool arestasPonderadas)
{
    this->direcionado = direcionado;
    this->arestasPonderadas = arestasPonderadas;
    this->verticesPonderados = verticesPonderados;
}

Grafo::~Grafo()
{
    for (Vertice *vertice : vertices)
    {
        for (Aresta *aresta : vertice->arestas)
        {
            delete aresta;
        }
        delete vertice;
    }
}

/**
 * Método para impressão de um grafo diretamente para o console, seguindo a visão de sua lista de adjacências.
 */
void Grafo::print()
{
    Printer::printGrafo(vertices, direcionado, arestasPonderadas, verticesPonderados);
}

/**
 * Método público para escrever o grafo num arquivo especificado pela referência
 * à ofstream passada como parâmetro.
 */
void Grafo::print(std::ofstream &output)
{
    Printer::printGrafo(vertices, direcionado, arestasPonderadas, verticesPonderados, output);
}

bool Grafo::existeAresta(int idVerticeU, int idVerticeV)
{
    Vertice *u = getVertice(idVerticeU);
    if (u == nullptr)
    {
        return false; // não existe vértice V
    }
    for (Aresta *aresta : u->arestas)
    {
        if (aresta->destino->id == idVerticeV)
        {
            return true;
        }
    }
    return false; // V não é adjacente a U (ou não existe)
}

Vertice *Grafo::getVertice(int idAlvo)
{
    for (Vertice *vertice : vertices)
    {
        if (vertice->id == idAlvo)
        {
            return vertice;
        }
    }
    return nullptr;
}

void Grafo::adicionaAdjacencias(int idVerticeU, int idVerticeV, int peso)
{
    Vertice *u = getVertice(idVerticeU);
    Vertice *v = getVertice(idVerticeV);
    Aresta *e = new Aresta;
    e->destino = v;
    e->peso = peso;
    u->arestas.push_back(e);
}

/**
 * Adiciona um novo vértice ao grafo, caso não exista um com o id especificado.
 */
void Grafo::adicionaVertice(int idVertice, int peso)
{
    if (getVertice(idVertice) != nullptr)
    {
        return; // já existe o vértice com o id especificado
    }
    Vertice *u = new Vertice;
    u->id = idVertice;
    u->peso = peso;
    vertices.push_back(u);
}

void Grafo::adicionaAresta(int idVerticeU, int idVerticeV, int peso)
{
    if (existeAresta(idVerticeU, idVerticeV))
    {
        return; // já existe aresta pelo par
    }
    adicionaVertice(idVerticeU);
    adicionaVertice(idVerticeV);
    adicionaAdjacencias(idVerticeU, idVerticeV, peso);
    if (!direcionado)
    {
        adicionaAdjacencias(idVerticeV, idVerticeU, peso);
    }
}

/**
 * Remove a aresta da(s) lista(s) de arestas e libera a memória alocada.
 */
void Grafo::removeAresta(int idVerticeU, int idVerticeV)
{
    if (!existeAresta(idVerticeU, idVerticeV))
    {
        return;
    }
    Vertice *u = getVertice(idVerticeU);
    Aresta *e;
    for (Aresta *aresta : u->arestas)
    {
        if (aresta->destino->id == idVerticeV)
        {
            e = aresta;
        }
    }
    std::list<Aresta *>::iterator it = std::find(u->arestas.begin(), u->arestas.end(), e);
    u->arestas.erase(it);
    delete e;
    if (direcionado)
    {
        return;
    }
    Vertice *v = getVertice(idVerticeV);
    for (Aresta *aresta : v->arestas)
    {
        if (aresta->destino->id == idVerticeU)
        {
            e = aresta;
        }
    }
    it = std::find(v->arestas.begin(), v->arestas.end(), e);
    v->arestas.erase(it);
    delete e;
}

/**
 * Método recursivo auxiliar para a funcionalidade de fecho transitivo direto.
 * Busca os sucessores dos sucessores do vértice solicitado.
 */
void Grafo::auxFechoDireto(Vertice *vertice, std::set<int> &fecho)
{
    for (Aresta *aresta : vertice->arestas)
    {
        Vertice *sucessor = aresta->destino;
        fecho.insert(sucessor->id);
        auxFechoDireto(sucessor, fecho);
    }
}

/**
 * Imprime na tela o fecho transitivo direto de um vértice no grafo dado seu id.
 * - caso o grafo não seja direcionado, retorna, visto que a operação não pode ser feita;
 * - caso o vértice não existe no conjunto de vértices, retorna;
 */
void Grafo::fechoTransitivoDireto(int idVertice)
{
    if (!direcionado)
    {
        std::cout << "O grafo deve ser direcionado\n";
        return;
    }
    Vertice *v = getVertice(idVertice);
    if (v == nullptr)
    {
        std::cout << "Nao existe no grafo vertice com o id especificado (" << idVertice << ")\n";
        return;
    }
    std::set<int> fecho;
    for (Aresta *aresta : v->arestas)
    {
        Vertice *sucessor = aresta->destino;
        fecho.insert(sucessor->id);
        auxFechoDireto(sucessor, fecho);
    }
    if (fecho.empty())
    {
        std::cout << "O fecho transitivo direto do vértice é vazio\n";
        return;
    }
    std::cout << "Fecho transitivo direto do vertice " << idVertice << ":\n { ";
    for (int idVerticeFecho : fecho)
    {
        std::cout << idVerticeFecho << " ";
    }
    std::cout << "}\n";
}

int Grafo::encontraIndiceVertice(int id)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i]->id == id)
        {
            return i;
        }
    }
    return -1;
}

void Grafo::inicializaMatrizDistancias(std::vector<std::vector<int>> &distancias, int ordem)
{
    for (int i = 0; i < ordem; i++)
    {
        std::vector<int> linhaInicial;
        for (int j = 0; j < ordem; j++)
        {
            if (i == j)
            {
                linhaInicial.push_back(0);
            }
            else
            {
                linhaInicial.push_back(INT_MAX);
            }
        }
        distancias.push_back(linhaInicial);
    }
    for (int i = 0; i < ordem; i++)
    {
        Vertice *u = vertices[i];
        for (Aresta *e : u->arestas)
        {
            Vertice *v = e->destino;
            int j = encontraIndiceVertice(v->id);
            distancias[i][j] = e->peso;
        }
    }
}

void Grafo::atualizaMatrizDistancias(std::vector<std::vector<int>> &distancias, int ordem, int indice)
{
    if (indice == ordem)
        return;
    for (int i = 0; i < ordem; i++)
    {
        if (i == indice)
            continue; // é necessário?
        for (int j = 0; j < ordem; j++)
        {
            if (j == indice)
                continue; // é necessário?
            int distanciaAtual = distancias[i][j];
            int distanciaIntermediariaA = distancias[i][indice];
            int distanciaIntermediariaB = distancias[indice][j];
            if (distanciaIntermediariaA == INT_MAX || distanciaIntermediariaB == INT_MAX)
                continue; // evitando cálculos imprevisíveis
            int novaDistancia = distanciaIntermediariaA + distanciaIntermediariaB;
            distanciaAtual = std::min(distanciaAtual, novaDistancia);
            distancias[i][j] = distanciaAtual;
        }
    }
    atualizaMatrizDistancias(distancias, ordem, indice + 1);
}

/**
 * Retorna a matriz representativa dos caminhos mínimos entre quaisquer vértices do grafo.
 * distancias(i,j) = INT_MAX -> não existe qualquer caminho entre os dois vértices;
 * distancias(i,j) = 0 -> caminho não é permitido (self-loop), só ocorre quando i = j.
 */
std::vector<std::vector<int>> Grafo::getMatrizDistancias()
{
    std::vector<std::vector<int>> distancias;
    inicializaMatrizDistancias(distancias, vertices.size());
    atualizaMatrizDistancias(distancias, vertices.size(), 0);
    return distancias;
}

void Grafo::caminhoMinimoFloyd(int idVerticeU, int idVerticeV)
{
    if (!arestasPonderadas)
    {
        std::cout << "As operacoes de caminho minimo nao sao permitidas para grafos sem ponderacao nas arestas" << std::endl;
        return;
    }
    int u = encontraIndiceVertice(idVerticeU);
    if (u == -1)
    {
        std::cout << "Nao existe vertice de id " << idVerticeU << std::endl;
        return;
    }
    int v = encontraIndiceVertice(idVerticeV);
    if (v == -1)
    {
        std::cout << "Nao existe vertice de id " << idVerticeV << std::endl;
        return;
    }
    int ordem = vertices.size();
    std::vector<std::vector<int>> distancias = getMatrizDistancias();
    int distanciaUV = distancias[u][v];
    if (distanciaUV == INT_MAX)
    {
        std::cout << "Nao ha qualquer caminho entre " << idVerticeU << " e " << idVerticeV << '\n';
    }
    else
    {
        std::cout << "O caminho minimo entre os vertices " << idVerticeU << " e " << idVerticeV << " eh: " << distanciaUV << '\n';
    }
}

/**
 * parâmetros:
 * - distanciasVertice: vetor contendo as distâncias do vértice para todos os vértices do grafo.
 */
int Grafo::getExcentricidade(const std::vector<int> &distanciasVertice)
{
    int excentricidade = INT_MIN;
    for (int distancia : distanciasVertice)
    {
        if (distancia != 0 && distancia != INT_MAX && distancia > excentricidade)
        {
            excentricidade = distancia;
        }
    }
    return excentricidade;
}

/**
 * Analisa um grafo e os caminhos entre seus vértices em busca de seu centro, periferia, diâmetro e raio.
 */
void Grafo::analiseExcentricidade()
{
    if (!arestasPonderadas)
    {
        std::cout << "Operacao nao permitida para grafos com arestas nao ponderadas" << std::endl;
        return;
    }
    std::map<int, std::vector<int>> excentricidades;
    int raio = INT_MAX;
    int diametro = INT_MIN;
    auto distancias = getMatrizDistancias();
    for (int i = 0; i < distancias.size(); i++)
    {
        int e = getExcentricidade(distancias[i]);
        if (e == INT_MIN)
        {
            continue;
        }
        if (raio > e)
        {
            raio = e;
        }
        if (diametro < e)
        {
            diametro = e;
        }
        excentricidades[e].push_back(i);
    }
    if (raio == INT_MAX)
    {
        std::cout << "Nao ha caminho de um vertice para qualquer outro vertice no grafo" << std::endl;
        return;
    }
    auto centro = excentricidades[raio];
    auto periferia = excentricidades[diametro];
    std::cout << "O raio do grafo eh " << raio << " e seu centro eh composto pelos vertices {";
    for (int i = 0; i < centro.size(); i++)
    {
        std::cout << ' ' << vertices[i]->id << ' ';
    }
    std::cout << "}\n";
    std::cout << "O diametro do grafo eh " << diametro << " e sua periferia eh composta pelos vertices {";
    for (int i = 0; i < periferia.size(); i++)
    {
        std::cout << ' ' << vertices[i]->id << ' ';
    }
    std::cout << '}' << std::endl;
}

void Grafo::preencheMapaInicialProfundidade(std::map<Vertice *, int> &mapa)
{
    for (Vertice *vertice : vertices)
    {
        mapa[vertice] = 0;
    }
}

/**
 * Método auxiliar que percorre o grafo em profundidade recursivamente.
 * u: vértice visitado
 * entrada: mapa que guarda o tempo de entrada de cada vértice
 */
void Grafo::caminhaProfundidade(Vertice *u, std::map<Vertice *, int> &entrada, std::map<Vertice *, int> &saida,
                                std::map<Vertice *, int> &cor, int &tempo)
{
    std::cout << "Visitando: " + u->id + '\n';
    entrada[u] = ++tempo;
    cor[u] = 1;
    for (Aresta *aresta : u->arestas)
    {
        Vertice *v = aresta->destino;
        std::cout << "Adjacente: " + v->id + '\n';
        caminhaProfundidade(v, entrada, saida, cor, tempo);
    }
    cor[u] = 2;
    saida[u] = ++tempo;
    std::cout << "Visitado: " + u->id + '\n';

}

void Grafo::caminhamentoProfundidade(int idVerticeInicio)
{
    if (vertices.size() == 0)
    {
        std::cout << "Nao ha vertices no grafo" << std::endl;
        return;
    }
    Vertice *inicial = getVertice(idVerticeInicio);
    if (inicial = nullptr)
    {
        std::cout << "O vertice especificado nao existe" << std::endl;
        return;
    }
    std::cout << "Iniciando o caminhamento pelo vértice " + inicial->id + '\n';
    int tempo = 0;
    std::map<Vertice *, int> tempoEntrada;
    std::map<Vertice *, int> tempoSaida;
    std::map<Vertice *, int> cor;
    preencheMapaInicialProfundidade(tempoEntrada);
    preencheMapaInicialProfundidade(tempoSaida);
    preencheMapaInicialProfundidade(cor);
    int& cronometro = tempo;
    caminhaProfundidade(inicial, tempoEntrada, tempoSaida, cor, cronometro);
    // perguntar ao tutor como deve ser feita a impressão desse troço
}

void Grafo::liberaMemoriaArestas(std::list<Aresta *> &arestas)
{
    for (Aresta *aresta : arestas)
    {
        delete aresta;
    }
}

void Grafo::removeVertice(int idVertice)
{
    Vertice *u = getVertice(idVertice);
    if (u == nullptr)
    {
        return; // vértice buscado não existe
    }
    std::list<Aresta *> &arestas = u->arestas;
    for (Vertice *vertice : vertices)
    {
        removeAresta(vertice->id, idVertice);
    }
    liberaMemoriaArestas(arestas);
    std::vector<Vertice *>::iterator it = std::find(vertices.begin(), vertices.end(), u);
    vertices.erase(it);
    delete u;
}
