#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>
#include "Grafo.hpp"
#include "Printer.hpp"

const int INF = std::numeric_limits<int>::max();
const int MIN = std::numeric_limits<int>::min();

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
                linhaInicial.push_back(INF);
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
            if (distanciaIntermediariaA == INF || distanciaIntermediariaB == INF) continue; // evitando cálculos imprevisíveis
            int novaDistancia = distanciaIntermediariaA + distanciaIntermediariaB;
            distanciaAtual = std::min(distanciaAtual, novaDistancia);
            distancias[i][j] = distanciaAtual;
        }
    }
    atualizaMatrizDistancias(distancias, ordem, indice + 1);
}

/**
 * Retorna a matriz representativa dos caminhos mínimos entre quaisquer vértices do grafo.
 * distancias(i,j) = INF -> não existe qualquer caminho entre os dois vértices;
 * distancias(i,j) = 0 -> caminho não é permitido (self-loop), só ocorre quando i = j.
 */
std::vector<std::vector<int>> Grafo::getMatrizDistancias()
{
    std::vector<std::vector<int>> distancias;
    inicializaMatrizDistancias(distancias, vertices.size());
    atualizaMatrizDistancias(distancias, vertices.size(), 0);
    return distancias;
}

void Grafo::inicializaMatrizesFloyd(std::vector<std::vector<int>>& distancias, std::vector<std::vector<int>>& proximos, int ordem)
{
    for (int i = 0; i < ordem; i++)
    {
        std::vector<int> linhaDistancias;
        std::vector<int> linhaProximos;
        for (int j = 0; j < ordem; j++)
        {
            if (i == j)
            {
                linhaDistancias.push_back(0);
                linhaProximos.push_back(j);
            }
            else
            {
                linhaDistancias.push_back(INF);
                linhaProximos.push_back(-1);
            } 
        }
        distancias.push_back(linhaDistancias);
        proximos.push_back(linhaProximos);
    }
    for (int i = 0; i < ordem; i++)
    {
        Vertice* u = vertices[i];
        Aresta *aresta = u->arestas;
        while (aresta != nullptr)
        {
            Vertice* v = aresta->destino;
            int j = encontraIndiceVertice(v->id);
            distancias[i][j] = aresta->peso; // custo é o peso da aresta
            proximos[i][j] = j; // o próximo é o vértice de destino da aresta
            aresta = aresta->prox;
        }
    }
}

void Grafo::atualizaMatrizesFloyd(std::vector<std::vector<int>>& distancias, std::vector<std::vector<int>>& proximos, int ordem, int indice)
{
    if (indice == ordem)
    {
        return;
    }
    for (int i = 0; i < ordem; i++)
    {
        if (i == indice) 
        {
            continue; // é necessário?
        }
        for (int j = 0; j < ordem; j++)
        {
            if (j == indice) 
            {
                continue; // é necessário?
            }
            int distanciaAtual = distancias[i][j];
            if (distancias[i][indice] == INF || distancias[indice][j] == INF)
            {
                continue; // evitando cálculos imprevisíveis e irrelevantes
            }
            int novaDistancia = distancias[i][indice] + distancias[indice][j];
            if (novaDistancia < distanciaAtual)
            {
                distancias[i][j] = novaDistancia;
                proximos[i][j] = proximos[i][indice];
            }
        }
    }
    atualizaMatrizesFloyd(distancias, proximos, ordem, indice + 1);
}

/**
 * Calcula o caminho mínimo entre dois vértices do grafo.
 * - Caso o grafo não possua arestas ponderadas, retorna um nullptr
 * - Caso um ou ambos os vértices não exista, retorna um nullptr
 * - Caso não exista caminho entre os vértices, retorna um grafo vazio
 * - Caso exista caminho, retorna um grafo com as arestas que compõem o caminho mínimo
 */
Grafo* Grafo::caminhoMinimoFloyd(int idVerticeU, int idVerticeV)
{
    if (!arestasPonderadas)
    {
        std::cout << "As operacoes de caminho minimo nao sao permitidas para grafos sem ponderacao nas arestas" << std::endl;
        return nullptr;
    }
    int u = encontraIndiceVertice(idVerticeU);
    if (u == -1)
    {
        std::cout << "Nao existe vertice de id " << idVerticeU << std::endl;
        return nullptr;
    }
    int v = encontraIndiceVertice(idVerticeV);
    if (v == -1)
    {
        std::cout << "Nao existe vertice de id " << idVerticeV << std::endl;
        return nullptr;
    }
    std::cout << "Calculando caminho mínimo entre os vértices " << idVerticeU << ":" << u << " e " << idVerticeV << ":" << v << '\n';
    int ordem = vertices.size();
    std::vector<std::vector<int>> distancias; // controla o caminho mínimo entre i e j
    std::vector<std::vector<int>> proximos; // controla qual é o próximo no caminho mínimo de i a j
    inicializaMatrizesFloyd(distancias, proximos, vertices.size());
    atualizaMatrizesFloyd(distancias, proximos, vertices.size(), 0);
    int distanciaUV = distancias[u][v];
    if (distanciaUV == INF)
    {
        std::cout << "Nao ha qualquer caminho entre " << idVerticeU << " e " << idVerticeV << '\n';
        return new Grafo(direcionado, 0, 0);
    }
    std::cout << "O custo do caminho minimo entre os vertices " << idVerticeU << " e " << idVerticeV << " é: " << distanciaUV << '\n';
    std::vector<int> caminho = {vertices[u]->id};
    while (u != v)
    {
        u = proximos[u][v];
        caminho.push_back(vertices[u]->id);
    }
    Grafo* grafoCaminho = new Grafo(direcionado, 0, 0);
    for (int i = 0; i < caminho.size() - 1; i++) {
        grafoCaminho->adicionaAresta(caminho[i], caminho[i + 1]);
    }
    return grafoCaminho;
}

/**
 * parâmetros:
 * - distanciasParaOutros: vetor contendo as distâncias do vértice para todos os vértices do grafo.
 * obs.: distância 0 indica selfloop e distância INF indica que não há caminho entre os vértices.
 */
int Grafo::getExcentricidade(const std::vector<int> &distanciasParaOutros)
{
    int excentricidade = MIN;
    for (int distancia : distanciasParaOutros)
    {
        if (distancia != 0 && distancia != INF && distancia > excentricidade)
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
    int raio = INF;
    int diametro = MIN;
    auto distancias = getMatrizDistancias();
    for (int i = 0; i < distancias.size(); i++)
    {
        int e = getExcentricidade(distancias[i]);
        if (e == MIN)
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
    if (raio == INF)
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
 * Método auxiliar que percorre o grafo em profundidade recursivamente.
 * Parâmetros:
 * - u: vértice sendo visitado
 * - pai: vértice que antecede u na busca
 * - cor: mapa que indica se o vértice foi visitado (2), se não (0) ou se está em processo de visita (1).
 * - arvore: grafo que representa a árvore de caminhamento em profundidade.
 */
void Grafo::caminhaProfundidade(Vertice *u, std::map<Vertice *, int> &cor, Grafo* arvore)
{
    cor[u] = 1;
    Aresta *e = u->arestas;
    while (e != nullptr)
    {
        Vertice *v = e->destino;
        if (cor[v] == 0)
        {
            arvore->adicionaAresta(u->id, v->id);
            caminhaProfundidade(v, cor, arvore);
        } else if (cor[v] == 1)
        {
            arvore->adicionaAresta(u->id, v->id, -1);
        }
        e = e->prox;
    }
    cor[u] = 2;
}

/**
 * Imprime no terminal e retorna como um grafo a árvore de caminhamento em
 * profundidade do grafo partindo do vértice especificado.
 * - Caso o grafo seja vazio, retorna um nullptr;
 * - Caso o vértice especificado não exista, retorna um nullptr;
 * Obs.: a árvore de caminhamento em profundidade é um grafo que representa as arestas percorridas
 * e usa arestas com peso -1 para indicar arestas de retorno.
 * - Aresta de retorno: aresta que liga um vértice a um ancestral na árvore de caminhamento em profundidade.
 */
Grafo* Grafo::caminhamentoProfundidade(int idVerticeInicio)
{
    if (vertices.empty())
    {
        std::cout << "Nao há vertices no grafo" << std::endl;
        return nullptr;
    }
    Vertice *inicial = getVertice(idVerticeInicio);
    if (inicial == nullptr)
    {
        std::cout << "O vértice especificado não existe" << std::endl;
        return nullptr;
    }
    Grafo *arvore = new Grafo(1, 0, 0); // deve ser direcionando para representar a árvore corretamente com as arestas de retorno
    arvore->adicionaVertice(inicial->id);
    int tempo = 0;
    std::map<Vertice *, int> cor;
    for (Vertice *v : vertices)
    {
        cor[v] = 0;
    }
    caminhaProfundidade(inicial, cor, arvore);
    Printer::printArvoreCaminhamento(vertices);
    return arvore;
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

/**
 * Método auxiliar para a busca em profundidade de uma componente conexa.
 * O método é recursivo e, a cada chamada, adiciona um vértice ao conjunto que representa a componente conexa.
 * Parâmetros:
 * - v: vértice de início da busca;
 * - usados: mapa que indica quais vértices já foram visitados;
 * - componente: conjunto que armazena os vértices da componente conexa, inicialmente, contem apenas o vértice da primeira chamada.
 */
void Grafo::buscaProfundidadeComponente(Vertice *v, std::map<Vertice*, bool>& usados, std::set<Vertice*>& componente)
{
    Aresta *aresta = v->arestas;
    while (aresta != nullptr)
    {
        Vertice *u = aresta->destino;
        if (!usados[u])
        {
            usados[u] = true;
            componente.insert(u);
            buscaProfundidadeComponente(u, usados, componente);
        }
        aresta = aresta->prox;
    }
}

/**
 * Encontra e retorna a componente conexa do grafo que contém o vértice passado como argumento.
 * Parâmetros:
 * - v: vértice de início da busca;
 * - usados: mapa que indica quais vértices já foram visitados.
 */
std::set<Vertice*> Grafo::buscaComponente(Vertice *v, std::map<Vertice*, bool>& usados)
{
    usados[v] = true;
    std::set<Vertice*> componente;
    componente.insert(v);
    buscaProfundidadeComponente(v, usados, componente);
    return componente;
}

/**
 * Retorna um set de set de vértices, onde cada set interno representa uma componente conexa do grafo.
 */
std::set<std::set<Vertice*>> Grafo::getComponentesConexas()
{
    std::set<std::set<Vertice*>> componentes;
    std::map<Vertice*, bool> usados;
    for (Vertice* v : vertices)
    {
        usados[v] = false;
    }
    for (Vertice* v : vertices)
    {
        if (!usados[v])
        {
            std::set<Vertice*> componente = buscaComponente(v, usados);
            componentes.insert(componente);
        }
    }
    return componentes;
}

/**
 * Através de uma busca em profundidade, encontra quais são os vértices de articulação
 * da componente conexa que contém o vértice passado como argumento.
 * Parâmetros:
 * - articulacoes: conjunto que armazena os vértices de articulação;
 * - v: vértice de início da busca;
 * - pai: vértice pai do vértice de início da busca;
 * - tempoEntrada: mapa que armazena o tempo de entrada de cada vértice na busca;
 * - minimo: mapa que armazena o menor tempo de entrada dos vértices alcançáveis por cada vértice;
 * - visitado: mapa que indica quais vértices já foram visitados;
 * - cronometro: contador que armazena o tempo de entrada dos vértices.
 */
void Grafo::buscaProfundidadeArticulacoes(std::set<int> &articulacoes, Vertice *v, Vertice *pai,
        std::map<int, int> &tempoEntrada, std::map<int, int> &minimo, std::map<int, bool> visitado, int &cronometro)
{
    visitado[v->id] = true;
    cronometro++;
    tempoEntrada[v->id] = cronometro;
    minimo[v->id] = cronometro;
    int filhos = 0;
    Aresta *aresta = v->arestas;
    while (aresta != nullptr)
    {
        Vertice *u = aresta->destino;
        if (u == pai)
        {
            aresta = aresta->prox;
            continue;
        }
        if (visitado[u->id])
        {
            minimo[v->id] = std::min(minimo[v->id], tempoEntrada[u->id]);
        }
        else
        {
            buscaProfundidadeArticulacoes(articulacoes, u, v, tempoEntrada, minimo, visitado, cronometro);
            minimo[v->id] = std::min(minimo[v->id], minimo[u->id]);
            if (minimo[u->id] >= tempoEntrada[v->id] && pai != nullptr)
            {
                articulacoes.insert(v->id);
            }
            filhos++;
        }
        aresta = aresta->prox;
    }
}

/**
 * Método auxiliar para a busca dos vértices de articulação de uma componente conexa.
 * Parâmetros:
 * - v: vértice de início da busca;
 * - componente: conjunto que armazena os vértices da componente conexa.
 * Retorna um set de inteiros, com os IDs dos vértices de articulação.
 */
std::set<int> Grafo::encontraArticulacoesComponente(Vertice *v, std::set<Vertice*> componente)
{
    int cronometro = 0;
    std::set<int> articulacoes;
    std::map<int, int> tempoEntrada, minimo;
    std::map<int, bool> visitado;
    for (Vertice* v : componente) // inicializações
    {
        tempoEntrada[v->id] = -1;
        minimo[v->id] = -1;
        visitado[v->id] = false;
    }
    buscaProfundidadeArticulacoes(articulacoes, v, nullptr, tempoEntrada, minimo, visitado, cronometro);
    return articulacoes;
}

/**
 * Retorna um grafo com os vértices de articulação do grafo original.
 * - Caso o grafo não possua vértices, retorna um nullptr.
 */
Grafo* Grafo::verticesDeArticulacao()
{
    if (vertices.empty())
    {
        std::cout << "O grafo nao possui vertices" << std::endl;
        return nullptr;
    }
    Grafo *grafoArticulacoes = new Grafo(direcionado, 0, 0);
    std::set<std::set<Vertice*>> componentes = getComponentesConexas();
    std::cout << "Componentes conexas do grafo: " << componentes.size() << std::endl;
    for (std::set<std::set<Vertice*>>::iterator it = componentes.begin(); it != componentes.end(); it++)
    {
        std::set<Vertice*> componente = *it;
        int cronometro = 0;

        Vertice *v = *componente.begin();
        std::cout << "Componente conexa comecando em " << v->id << std::endl;
        std::set<int> articulacoes = encontraArticulacoesComponente(v, componente);
        for (int id : articulacoes)
        {
            grafoArticulacoes->adicionaVertice(id);
        }
    }
    return grafoArticulacoes;
}
  
bool Grafo::auxDijkstra() {
    for (auto v : vertices) {
        if (v->aberto && v->distancia != std::numeric_limits<int>::max()) {
            return true;
        }
    }
    return false;
}

std::list<int> Grafo::listaAdjacentes(int idVertice) {
    std::list<int> lista;
    for (auto a : getVertice(idVertice)->arestas) {
        lista.push_front(a->destino->id);
    }
    return lista;
}

void Grafo::Dijkstra(int idVertice) {
    if (!direcionado)
    {
        std::cout << "O grafo deve ser direcionado\n";
        return;
    }
    if(!arestasPonderadas)
    {
        std::cout << "O grafo ter arestas ponderadas\n";
        return;
    }
    Vertice *v = getVertice(idVertice);
    if (v == nullptr)
    {
        std::cout << "Nao existe no grafo vertice com o id especificado (" << idVertice << ")\n";
        return;
    }
    for(auto a: vertices){
        for (auto ares : a->arestas) {
            if(a->peso < 0){
                std::cout << "O peso das sarestas não pode ser negativo\n";
                return;
            }
        }
    }
    v->distancia = 0; // Distância do vértice de origem para ele mesmo é 0

    while (auxDijkstra()) {
        // Pegar o vértice com a menor distância
        Vertice* verticeAtual = nullptr;
        int menorDistancia = std::numeric_limits<int>::max();
        for (auto vert : vertices) {
            if (vert->aberto && vert->distancia < menorDistancia) {
                menorDistancia = vert->distancia;
                verticeAtual = vert;
            }
        }

        // Verificar se encontrou um vértice válido
        if (verticeAtual == nullptr) {
            break;
        }

        verticeAtual->aberto = false; // Marcar o vértice atual como fechado
        std::cout << verticeAtual->id << " distancia: " << verticeAtual->distancia << "\n";
        
        // Atualizar as distâncias dos vértices adjacentes
        for(auto a: vertices){
            for (auto ares : a->arestas) {
            Vertice* destino = ares->destino;
            if (destino->aberto && verticeAtual->distancia + ares->peso < destino->distancia) {
                destino->distancia = a->distancia + ares->peso;
            }
        }
        }
    }
}
    
