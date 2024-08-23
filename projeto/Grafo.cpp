#include <iostream>
#include <vector>
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

void Grafo::removeAresta(int idVerticeU, int idVerticeV)
{
    if (!existeAresta(idVerticeU, idVerticeV))
    {
        return;
    }
    Vertice *u = getVertice(idVerticeU);
    for (Aresta *aresta : u->arestas)
    {
        if (aresta->destino->id == idVerticeV)
        {
            delete aresta;
        }
    }
    if (direcionado)
    {
        return;
    }
    Vertice *v = getVertice(idVerticeV);
    for (Aresta *aresta : v->arestas)
    {
        if (aresta->destino->id == idVerticeU)
        {
            delete aresta;
        }
    }
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
        for (Aresta* e : u->arestas)
        {
            Vertice* v = e->destino;
            int j = encontraIndiceVertice(v->id);
            distancias[i][j] = e->peso;
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
 * Retorna o peso de uma aresta. Se o vértice de origem não existir, retorna -1. Se não existir a aresta, retorna -1.
 */
int Grafo::custo(int idVerticeU, int idVerticeV)
{
    Vertice* u = getVertice(idVerticeU);
    if (u == nullptr) {
        return -1;
    }
    for (Aresta* e : u->arestas)
        if (e->destino->id == idVerticeV)
            return e->peso;
    return INT_MAX;
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
    std::vector<std::vector<int>> distancias;
    inicializaMatrizDistancias(distancias, ordem);
    atualizaMatrizDistancias(distancias, ordem, 0);
    int distanciaUV = distancias[u][v];
    if (distanciaUV == INT_MAX)
        std::cout << "Nao ha qualquer caminho entre " << idVerticeU << " e " << idVerticeV << '\n';
    else
        std::cout << "O caminho minimo entre os vertices " << idVerticeU << " e " << idVerticeV << " eh: " << distanciaUV << '\n';
}

void Grafo::removeVertice(int idVertice)
{
    Vertice *v = getVertice(idVertice);
    if (v == nullptr)
    {
        return; // vértice buscado não existe
    }
    for (Aresta *aresta : v->arestas)
    {
        removeAresta(idVertice, aresta->destino->id);
        removeAresta(aresta->destino->id, idVertice);
    }
    delete v;
}

void Grafo::prim(std::vector<int>& listavertice)
{
    //Fazer função para ver se grafo é fechado (conexo)
    //Ver se vai dar problema qndo receber subárvore
    //Tratamento de erro
    int n=listavertice.size(); int prox[n];
    std::vector<std::vector<int>> F;
    int menorid; int menorpeso=INT_MAX;
    int u; int v;
    for(int i=0; i<n; i++)
    {
        Vertice* a = getVertice(listavertice[i]);
        for (Aresta* e : a->arestas)
        {
            if(e->peso<=menorpeso)
            {
                menorpeso=e->peso; menorid=a->id;
                u=a->id; v=e->destino->id;
            }
        }
    }
    std::vector<int> aux = {u,v}; 
    F.push_back(aux);
    for(int i=1; i<=n; i++)
    {
        if (custo(i,u)<custo(i,v))
        {
            prox[i-1]=u;
        }
        else
        {
            prox[i-1]=v;
        }
    }
    prox[u-1]=0; prox[v-1]=0; int cont=0; int j=1;
    while(cont<n-2)
    {
        int menor2=INT_MAX;
        for(int i=1;i<=n;i++)
        {
            if(prox[i-1]!=0)
            {
                if(custo(i,prox[i-1])<menor2)
                {
                    menor2=custo(i,prox[i-1]);
                    j=i;
                }
            }
        }
        aux={j,prox[j-1]};
        F.push_back(aux);
        prox[j-1]=0;
        for (int i=0; i<n;i++)
        {
            if(prox[i]!=0 && custo(i+1,prox[i])>custo(i+1,j))
            {
                prox[i]=j;
            }
        }
        cont=cont+1;
    }
}

void Grafo::kruskal(std::vector<int>& listavertice)
{
    //Função para ver se grafo é conexo
    //Ver se vai dar problema qndo receber subárvore
    //Fazer tratamento de erro
    int n=listavertice.size();
    std::vector<std::vector<int>> listaaresta;
    std::vector<std::vector<int>> F;
    std::vector<int> aux;
    int u; int v;
    for(int i=0; i<n; i++)
    {
        Vertice* a = getVertice(listavertice[i]);
        for (Aresta* e : a->arestas)
        {
            if(a->id<e->destino->id)
            {
                u=a->id; v=e->destino->id;
                aux={u,v};
                listaaresta.push_back(aux);
            }
        }
    }
    int temp1=0; int temp2=0; int size=listaaresta.size();
    for(int i=size-1;i>1;i--) //receba o BubbleSort
    {
        for(int j=0;j<i;j++)
        {
            temp1=listaaresta[j][0]; temp2=listaaresta[j][1];
            if(custo(listaaresta[j][0],listaaresta[j][1])>custo(listaaresta[j+1][0],listaaresta[j+1][1]))
            {
                listaaresta[j][0]=listaaresta[j+1][0]; listaaresta[j][1]=listaaresta[j+1][1];
                listaaresta[j+1][0]=temp1; listaaresta[j+1][1]=temp2;
            }
        }
    }


    for(int i=0;i<listaaresta.size();i++)
    {
        for(int j=0;j<2;j++)
        {
            std::cout<<listaaresta[i][j]<<std::endl;
        }
    }
}