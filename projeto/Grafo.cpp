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
        liberaMemoriaArestas(vertice->arestas);
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

/**
 * - Retorna true se existir aresta ou arco definidos pelo par (u, v)
 * - Retorna false caso contrário:
 *      - Se u não existir;
 *      - Se v não existir.
 *      - Se u existir, mas não houver aresta definida pelo par (u, v);
 *      - Se u existir, mas o grafo for não-direcionado e não houver aresta definida pelo par (v, u);
 */
bool Grafo::existeAresta(int idVerticeU, int idVerticeV)
{
    Vertice *u = getVertice(idVerticeU);
    if (u == nullptr)
    {
        return false;
    }
    Aresta *aresta = u->arestas;
    while (aresta != nullptr) {
        if (aresta->destino->id == idVerticeV)
        {
            return true;
        }
        aresta = aresta->prox;
    }
    return false; // V não é adjacente a U (ou não existe)
}

/**
 * Retorna o vértice com o id especificado, caso exista.
 * Caso contrário, retorna nullptr.
 */
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

/**
 * Adiciona uma nova adjacência ao vértice U, com destino ao vértice V e peso especificado.
 * Trata os ponteiros para as arestas de U e V, adicionando a nova aresta ao final da lista.
 */
void Grafo::adicionaAdjacencias(int idVerticeU, int idVerticeV, int peso)
{
    Vertice *u = getVertice(idVerticeU);
    Vertice *v = getVertice(idVerticeV);
    Aresta *e = new Aresta;
    e->destino = v;
    e->id_origem = idVerticeU;
    e->peso = peso;
    e->prox = nullptr;
    if (u->arestas == nullptr)
    {
        u->arestas = e;
    }
    else
    {
        Aresta *aresta = u->arestas;
        while (aresta->prox != nullptr)
        {
            aresta = aresta->prox;
        }
        aresta->prox = e;
    }
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
    u->arestas = nullptr;
}

/**
 * Adiciona uma nova aresta ao grafo, caso não exista uma com os vértices especificados.
 * Caso o grafo seja não-direcionado, adiciona a aresta no sentido contrário.
 */
void Grafo::adicionaAresta(int idVerticeU, int idVerticeV, int peso)
{
    if (existeAresta(idVerticeU, idVerticeV))
    {
        return; // já existe aresta definida pelo par
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
 * Remove o arco definido pelo par ordenado (u,v) e, se o grafo for não-direcionado, a aresta definida pelo não ordenado de mesmos vértices.
 */
bool Grafo::removeAresta(int idVerticeU, int idVerticeV)
{
    bool removida = false;
    Vertice *u = getVertice(idVerticeU);
    if (u == nullptr)
    {
        return false;
    }
    Aresta *e = u->arestas;
    if (e == nullptr)
    {
        return false;
    }
    Aresta *anterior = nullptr;
    while (e != nullptr) {
        if (e->destino->id == idVerticeV)
        {
            removida = true;
            break;
        }
        anterior = e;
        e = e->prox;
    }
    if (e == nullptr)
    {
        return false;
    }
    if (e == u->arestas) {
        u->arestas = e->prox;
    } else {
        anterior->prox = e->prox;
    }
    delete e;
    if (direcionado)
    {
        return removida;
    }
    Vertice *v = getVertice(idVerticeV); // v não pode ser nullptr nessa linha
    e = v->arestas;
    anterior = nullptr;
    while (e != nullptr) {
        if (e->destino->id == idVerticeU)
        {
            break;
        }
        anterior = e;
        e = e->prox;
    }
    if (e == v->arestas) {
        v->arestas = e->prox;
    } else {
        anterior->prox = e->prox;
    }
    delete e;
    return removida;
}

/**
 * Método recursivo auxiliar para a funcionalidade de fecho transitivo direto.
 * Busca os sucessores dos sucessores do vértice solicitado.
 */
void Grafo::auxFechoDireto(Vertice *vertice, std::set<int> &fecho, Grafo *grafoFecho)
{
    Aresta *aresta = vertice->arestas;
    while (aresta != nullptr) 
    {
        Vertice *sucessor = aresta->destino;
        aresta = aresta->prox;
        if (fecho.find(sucessor->id) != fecho.end())
        {
            continue;
        }
        fecho.insert(sucessor->id);
        grafoFecho->adicionaAresta(vertice->id, sucessor->id);
        auxFechoDireto(sucessor, fecho, grafoFecho);
    }
}

/**
 * Através de um caminhamento em profundidade, tendo como início o vértice com o id especificado, imprime seu fecho
 * transitivo direto e retorna um grafo que represente tal fecho com os vértices e as arestas que o justificam.
 * - caso o grafo não seja direcionado ou não exista tal vértice no grafo, retorna um nullptr a ser tratado,
 *      visto que a operação não pode ser feita;
 * - caso o vértice exista, mas seu fecho seja vazia, retorna um grafo também vazio, visto que o próprio vértice não faz parte de seu fecho.
 */
Grafo* Grafo::fechoTransitivoDireto(int idVertice)
{
    if (!direcionado)
    {
        std::cout << "O grafo deve ser direcionado\n";
        return nullptr;
    }
    Vertice *v = getVertice(idVertice);
    if (v == nullptr)
    {
        std::cout << "Nao existe no grafo vertice com o id especificado (" << idVertice << ")\n";
        return nullptr;
    }
    Grafo* grafoFecho = new Grafo(direcionado, 0, 0);
    std::set<int> fecho;
    Aresta *aresta = v->arestas;
    while (aresta != nullptr)
    {
        Vertice *sucessor = aresta->destino;
        grafoFecho->adicionaAresta(v->id, sucessor->id);
        fecho.insert(sucessor->id);
        auxFechoDireto(sucessor, fecho, grafoFecho);
        aresta = aresta->prox;
    }
    if (fecho.empty())
    {
        std::cout << "O fecho transitivo direto do vértice é vazio\n";
        return grafoFecho;
    }
    std::cout << "Fecho transitivo direto do vertice " << idVertice << ":\n { ";
    for (int idVerticeFecho : fecho)
    {
        std::cout << idVerticeFecho << " ";
    }
    std::cout << "}\n";
    return grafoFecho;
}

/**
 * Retorna: true se foi adicionado ao menos novo vértice ao fecho.
 */
bool Grafo::auxFechoIndireto(std::set<int>& fecho, std::vector<Vertice*>& naoUtilizados, Grafo *grafoFecho) {
    bool adicionou = false;
    for (std::vector<Vertice*>::iterator it = naoUtilizados.begin(); it != naoUtilizados.end(); it++)
    {
        for (int idVerticeFecho : fecho)
        {
            if (existeAresta((*it)->id, idVerticeFecho))
            {
                fecho.insert((*it)->id);
                grafoFecho->adicionaAresta((*it)->id, idVerticeFecho);
                naoUtilizados.erase(it);
                adicionou = true;
                break;
            }
        }
    }
    return adicionou;
}

/**
 * Através de um caminhamento em profundidade, tendo como início o vértice com o id especificado, imprime seu fecho
 * transitivo indireto e retorna um grafo que represente tal fecho com os vértices e as arestas que o justificam.
 * - caso o grafo não seja direcionado ou não exista tal vértice no grafo, retorna um nullptr a ser tratado,
 *      visto que a operação não pode ser feita;
 * - caso o vértice exista, mas seu fecho seja vazia, retorna um grafo também vazio, visto que o próprio vértice não faz parte de seu fecho.
 */
Grafo* Grafo::fechoTransitivoIndireto(int idVertice)
{
    if (!direcionado)
    {
        std::cout << "O grafo deve ser direcionado\n";
        return nullptr;
    }
    Vertice *u = getVertice(idVertice);
    if (u == nullptr)
    {
        std::cout << "Não existe o vértice definido pelo id " << idVertice << std::endl;
        return nullptr;
    }
    Grafo *grafoFecho = new Grafo(direcionado, 0, 0);
    std::vector<Vertice*> nao_utilizados;
    for (Vertice* v : vertices) 
    {
        if (v->id != u->id)
        { 
            nao_utilizados.push_back(v);
        }
    }
    std::set<int> fecho;
    for (std::vector<Vertice*>::iterator it = nao_utilizados.begin(); it != nao_utilizados.end(); it++)
    {
        Vertice* v = *it;
        if (existeAresta(v->id, idVertice))
        {
            grafoFecho->adicionaAresta(v->id, u->id);
            fecho.insert(v->id);
            nao_utilizados.erase(it);
        }
    }
    if (fecho.empty()) // o vértice é isolado
    {
        std::cout << "O fecho do vértice " << idVertice << " é o conjunto vazio." << std::endl;
        return grafoFecho;
    }
    while (auxFechoIndireto(fecho, nao_utilizados, grafoFecho));
    std::cout << "O fecho transitivo indireto do vértice " << idVertice << " é o conjunto composto pelos vértices:\n{ ";
    for (int id : fecho)
    {
        std::cout << id << ' ';
    }
    grafoFecho->adicionaVertice(u->id);
    std::cout << '}' << std::endl;
    return grafoFecho;
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

void Grafo::inicializaMatrizDistancias(std::vector<std::vector<int>>& distancias, int ordem)
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
        Vertice* u = vertices[i];
        Aresta *aresta = u->arestas;
        while (aresta != nullptr)
        {
            Vertice* v = aresta->destino;
            int j = encontraIndiceVertice(v->id);
            distancias[i][j] = aresta->peso;
            aresta = aresta->prox;
        }
    }
}

void Grafo::atualizaMatrizDistancias(std::vector<std::vector<int>>& distancias, int ordem, int indice)
{
    if (indice == ordem)
        return;
    for (int i = 0; i < ordem; i++)
    {
        if (i == indice) continue; // é necessário?
        for (int j = 0; j < ordem; j++)
        {
            if (j == indice) continue; // é necessário?
            int distanciaAtual = distancias[i][j];
            int distanciaIntermediariaA = distancias[i][indice];
            int distanciaIntermediariaB = distancias[indice][j];
            if (distanciaIntermediariaA == INT_MAX || distanciaIntermediariaB == INT_MAX) continue; // evitando cálculos imprevisíveis
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
    if (!arestasPonderadas) {
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
 * - distanciasParaOutros: vetor contendo as distâncias do vértice para todos os vértices do grafo.
 * obs.: distância 0 indica selfloop e distância INT_MAX indica que não há caminho entre os vértices.
 */
int Grafo::getExcentricidade(const std::vector<int> &distanciasParaOutros)
{
    int excentricidade = INT_MIN;
    for (int distancia : distanciasParaOutros)
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
 * - Diâmetro: maior excentricidade do grafo;
 * - Raio: menor excentricidade do grafo.
 * - Centro: vértice com menor excentricidade, ou seja, vértices que possuem excentricidade igual ao raio;
 * - Periferia: vértice com maior excentricidade, ou seja, vértices que possuem excentricidade igual ao diâmetro.
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

void Grafo::liberaMemoriaArestas(Aresta* inicio)
{
    Aresta* aresta = inicio;
    while (aresta != nullptr)
    {
        Aresta* prox = aresta->prox;
        delete aresta;
        aresta = prox;
    }
}

/**
 * Remove um vértice do grafo, caso exista, tratando de remover as adjacências por ele definidas.
 */
void Grafo::removeVertice(int idVertice)
{
    Vertice *u = getVertice(idVertice);
    if (u == nullptr)
    {
        return; // vértice buscado não existe
    }
    for (Vertice* vertice : vertices)
    {
        if (vertice->id != idVertice)
        {
            removeAresta(vertice->id, idVertice);
        }
    }
    Aresta *aresta = u->arestas;
    liberaMemoriaArestas(aresta);
    std::vector<Vertice*>::iterator it = std::find(vertices.begin(), vertices.end(), u);
    vertices.erase(it);
    delete u;
}
