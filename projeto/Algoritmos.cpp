#include "Algoritmos.hpp"
#include "Grafo.hpp"

int Algoritmos::gapAresta(Aresta *aresta)
{
    return abs(aresta->origem->peso - aresta->destino->peso);
}

/**
 * @brief Coleta as arestas de um grafo.
 *
 * Esta função percorre todos os vértices do grafo, coletando suas arestas e
 * evitando duplicatas (arestas de ida e volta) e mantendo ordem crescente de diferença
 * dos pesos dos vértices que definem cada uma. 
 * 
 * A função mantém a posição de um elemento caso haja empate na diferença de pesos dos vértices.
 * 
 * @return std::list<Aresta*> Lista com as arestas ordenadas
 */
std::list<Aresta *> Algoritmos::coletaArestasOrdenadas(Grafo *grafo)
{
    std::list<Aresta *> arestas;
    for (Vertice *vertice : grafo->vertices)
    {
        Aresta *aresta = vertice->arestas;
        while (aresta != nullptr)
        {
            if (aresta->origem->id < aresta->destino->id) // evita repetir arco de ida e volta
            {
                std::list<Aresta *>::iterator it = arestas.begin();
                while (it != arestas.end() && gapAresta(aresta) >= gapAresta(*it))
                {
                    it++;
                }
                arestas.insert(it, aresta);
            }
            aresta = aresta->prox;
        }
    }
    return arestas;
}

/**
 * @brief Verifica se uma aresta é adjacente a alguma outra na floresta.
 *
 * Esta função percorre todos os componentes da floresta e verifica se a aresta fornecida
 * é adjacente a alguma das arestas presentes em qualquer componente da floresta.
 *
 * @param aresta Ponteiro para a aresta que será verificada.
 * @param floresta Referência para uma matriz de ponteiros de arestas, representando a floresta.
 * @return true Se a aresta fornecida for adjacente a alguma aresta na floresta.
 * @return false Se a aresta fornecida não for adjacente a nenhuma aresta na floresta.
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
 * @brief Preenche a floresta com arestas não adjacentes.
 *
 * Esta função itera sobre as arestas fornecidas e adiciona aquelas que não são adjacentes
 * à floresta. A floresta é representada por um vetor de vetores de ponteiros para Aresta.
 * A função também mantém um conjunto de vértices visitados.
 *
 * @param floresta Referência para uma matriz ponteiros para Aresta, representando a floresta.
 * @param arestas Referência para um vetor de ponteiros para Aresta, representando as arestas disponíveis.
 * @param visitados Referência para um conjunto de inteiros, representando os vértices visitados.
 * @param minimos Referência para um vetor de inteiros, representando os pesos mínimos de cada componente da floresta.
 * @param maximos Referência para um vetor de inteiros, representando os pesos máximos de cada componente da floresta.
 */
void Algoritmos::preencheFloresta(std::vector<std::vector<Aresta *>> &floresta,
        std::list<Aresta *> &arestas,
        std::set<int> &visitados,
        std::vector<int> &minimos,
        std::vector<int> &maximos)
{
    for (int componente = 0; componente < floresta.size(); componente++)
    {
        std::list<Aresta *>::iterator iter = arestas.begin();
        while (iter != arestas.end())
        {
            Aresta *aresta = *iter;
            if (numeroDeVerticesAdjacentes(aresta, visitados) == 0)
            {
                Vertice *u = aresta->origem;
                Vertice *v = aresta->destino;
                int minAresta = std::min(u->peso, v->peso);
                int maxAresta = std::max(u->peso, v->peso);
                minimos[componente] = minAresta;
                maximos[componente] = maxAresta;
                floresta[componente].push_back(aresta);
                visitados.insert(u->id);
                visitados.insert(v->id);
                iter = arestas.erase(iter);
                break;
            }
            ++iter;
        }
    }
}

/**
 * @brief Preenche a floresta com arestas não adjacentes, escolhendo aleatoriamente entre as alfa % melhores arestas.
 * 
 * Esta função itera sobre as arestas fornecidas e adiciona aquelas que não são adjacentes
 * à floresta. A floresta é representada por um vetor de vetores de ponteiros para Aresta.
 * A função também mantém um conjunto de vértices visitados.
 * 
 * @param floresta Referência para uma matriz ponteiros para Aresta, representando a floresta.
 * @param arestas Referência para um vetor de ponteiros para Aresta, representando as arestas disponíveis.
 * @param visitados Referência para um conjunto de inteiros, representando os vértices visitados.
 * @param alfa Porcentagem que torna o algoritmo gulosoRandomizado. 0 < alfa <= 1.
 */
void Algoritmos::preencheFlorestaRandomizado(std::vector<std::vector<Aresta *>> &floresta,
                std::list<Aresta *> &arestas,
                std::set<int> &visitados,
                float alfa,
                std::vector<int> &minimos,
                std::vector<int> &maximos)
{
    for (int componente = 0; componente < floresta.size(); componente++)
    {
        int maxIndex = (int) ((arestas.size() - 1) * alfa);
        int indexSelecionado = geraIndiceAleatorioEntreZeroE(maxIndex);
        std::list<Aresta *>::iterator iter = arestas.begin();
        for (int i = 0; i <= indexSelecionado; i++, ++iter);
        Aresta *selecionada = *iter;
        if (numeroDeVerticesAdjacentes(selecionada, visitados) == 0)
        {
            floresta[componente].push_back(selecionada);
            visitados.insert(selecionada->origem->id);
            visitados.insert(selecionada->destino->id);
            arestas.erase(iter);
        }
    }
}

/**
 * @brief Calcula a quantos vértices já visitados uma aresta é adjacente (0, 1 ou 2).
 *
 * @param aresta Ponteiro para a aresta cujos vértices serão verificados.
 * @param visitados Conjunto de IDs de vértices que já foram visitados.
 * @return int Número de vértices adjacentes à aresta que já foram visitados.
 */
int Algoritmos::numeroDeVerticesAdjacentes(Aresta *aresta, std::set<int> &visitados)
{
    int adjacencias = 0;
    if (visitados.find(aresta->origem->id) != visitados.end())
    {
        adjacencias++;
    }
    if (visitados.find(aresta->destino->id) != visitados.end())
    {
        adjacencias++;
    }
    return adjacencias;
}

bool Algoritmos::saoAdjacentes(Aresta *e, Aresta *aresta)
{
    return e->origem == aresta->origem || e->origem == aresta->destino || e->destino == aresta->origem || e->destino == aresta->destino;
}

int Algoritmos::calculaImpacto(Aresta *candidata,
        std::vector<std::vector<Aresta *>> &floresta,
        std::vector<int> &minimos,
        std::vector<int> &maximos)
{
    int componente = -1;
    for (int i = 0; i < floresta.size(); i++)
    {
        for (Aresta *aresta : floresta[i])
        {
            if (saoAdjacentes(aresta, candidata))
            {
                componente = i;
                break;
            }
        }
    }
    int pesoU = candidata->origem->peso;
    int pesoV = candidata->destino->peso;
    int minAresta = std::min(pesoU, pesoV);
    int maxAresta = std::max(pesoU, pesoV);
    int gapAtual = maximos[componente] - minimos[componente];
    int novoMin = std::min(minimos[componente], minAresta);
    int novoMax = std::max(maximos[componente], maxAresta);
    int novoGap = novoMax - novoMin;
    int impacto = novoGap - gapAtual;
    return impacto;
}

/**
 * @brief Calcula o novo gap (diferença entre o maior e o menor peso) em uma floresta de componentes.
 *
 * Esta função percorre cada componente da floresta e determina o menor e o maior peso das arestas.
 * Se a aresta candidata for adjacente a alguma aresta do componente, seus pesos também são considerados.
 * O gap é a soma das diferenças entre os pesos máximos e mínimos de cada componente.
 *
 * @param floresta Vetor de vetores de ponteiros para Aresta, representando a floresta de componentes.
 * @param candidata Ponteiro para a aresta candidata a ser considerada na atualização do gap.
 * @return O novo gap calculado.
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

int Algoritmos::novoCalculaGap(std::vector<int> &minimos, std::vector<int> &maximos)
{
    int gap = 0;
    for (int componente = 0; componente < minimos.size(); componente++)
    {
        int gapComponente = maximos[componente] - minimos[componente];
        gap += gapComponente;
    }
    return gap;
}

/**
 * @brief Calcula o gap total de uma floresta.
 *
 * Esta função percorre uma floresta, onde cada componente da floresta
 * é um vetor de ponteiros para objetos do tipo Aresta. Para cada componente,
 * a função encontra o peso mínimo e o peso máximo das arestas e calcula a diferença
 * entre esses pesos (gap). O gap total é a soma das diferenças de todos os componentes.
 *
 * @param floresta Referência para um vetor de vetores de ponteiros para objetos do tipo Aresta.
 * @return O somatório dos gaps de cada partição.
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
 * @brief Adiciona uma nova aresta à floresta, escolhendo a aresta que minimiza o gap.
 *
 * Esta função percorre a lista de arestas candidatas e seleciona a aresta que,
 * ao ser adicionada à floresta, minimiza o gap calculado. A aresta selecionada
 * é então removida da lista de candidatas e adicionada à floresta.
 * 
 * Ao longo da iteração, a lista de arestas candidatas é filtrada para remover aquelas que não podem ser adicionadas.
 *
 * @param floresta Referência para o vetor de componentes da floresta, onde cada componente é um vetor de ponteiros para arestas.
 * @param arestas Referência para a lista de arestas candidatas existentes no grafo original que ainda não foram utilizadas.
 * @param visitados Referência para o conjunto de vértices já visitados.
 */
void Algoritmos::adicionaNovaAresta(std::vector<std::vector<Aresta *>> &floresta,
        std::list<Aresta *> &arestas,
        std::set<int> &visitados,
        std::vector<int> &minimos,
        std::vector<int> &maximos)
{
    int impactoMinimo = std::numeric_limits<int>::max();
    std::list<Aresta*>::iterator melhor = arestas.end();
    std::list<Aresta*>::iterator iter = arestas.begin();
    while (iter != arestas.end())
    {
        Aresta *aresta = *iter;
        if (numeroDeVerticesAdjacentes(aresta, visitados) == 2)
        {
            iter = arestas.erase(iter);
            continue;
        }
        if (numeroDeVerticesAdjacentes(aresta, visitados) == 1)
        {
            int impacto = calculaImpacto(aresta, floresta, minimos, maximos);
            if (impacto < impactoMinimo)
            {
                melhor = iter;
                impactoMinimo = impacto;
            }
        }
        iter++;
    }
    Aresta* adicionada = *melhor;
    int componente = -1;
    for (int i = 0; i < floresta.size(); i++)
    {
        for (Aresta *aresta : floresta[i])
        {
            if (saoAdjacentes(aresta, adicionada))
            {
                componente = i;
                break;
            }
        }
    }
    int pesoU = adicionada->origem->peso;
    int pesoV = adicionada->destino->peso;
    int minAresta = std::min(pesoU, pesoV);
    int maxAresta = std::max(pesoU, pesoV);
    minimos[componente] = std::min(minimos[componente], minAresta);
    maximos[componente] = std::max(maximos[componente], maxAresta);
    floresta[componente].push_back(adicionada);
    visitados.insert(adicionada->origem->id);
    visitados.insert(adicionada->destino->id);
    arestas.erase(melhor); // remove aresta utilizada da lista de candidatas
    return;
}

/**
 * @brief Gera um índice aleatório entre 0 e um valor máximo especificado.
 *
 * Esta função utiliza um gerador de números aleatórios para retornar um valor
 * inteiro aleatório entre 0 e o valor máximo fornecido (inclusive).
 *
 * @param max O valor máximo (inclusive) para o índice aleatório gerado.
 * @return Um valor inteiro aleatório entre 0 e o valor máximo especificado.
 */
int Algoritmos::geraIndiceAleatorioEntreZeroE(int max)
{
    if (max == 0)
    {
        return 0;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, max);
    return dis(gen);
}

/**
 * @brief Adiciona uma nova aresta à floresta de forma randomizada.
 *
 * Este método seleciona uma nova aresta a ser adicionada à floresta com base em um critério de aleatoriedade
 * controlado pelo parâmetro alfa. A aresta é escolhida dentre as candidatas que conectam um vértice visitado
 * a um não visitado, minimizando o gap resultante.
 * 
 * Ao longo da iteração, a lista de arestas candidatas é filtrada para remover aquelas que não podem ser adicionadas.
 *
 * @param floresta Referência para a floresta representada como um vetor de vetores de ponteiros para Aresta.
 * @param arestas Referência para a lista de arestas disponíveis para seleção.
 * @param visitados Referência para o conjunto de IDs de vértices já visitados.
 * @param alfa Parâmetro de controle da aleatoriedade na seleção da aresta (0 <= alfa <= 1).
 */
void Algoritmos::adicionaNovaArestaRandomizado(std::vector<std::vector<Aresta *>> &floresta,
                std::list<Aresta *> &arestas,
                std::set<int> &visitados,
                float alfa,
                std::vector<int> &minimos,
                std::vector<int> &maximos)
{
    std::list<Aresta *> candidatas;
    std::list<Aresta *>::iterator iter = arestas.begin();
    std::map<Aresta * , int> impactos;
    while (iter != arestas.end())
    {
        Aresta *aresta = *iter;
        int adjacencias = numeroDeVerticesAdjacentes(aresta, visitados);
        if (adjacencias == 2)
        {
            iter = arestas.erase(iter);
            continue;
        }
        if (adjacencias == 1)
        {
            int impacto = calculaImpacto(aresta, floresta, minimos, maximos);
            impactos[aresta] = impacto;
            std::list<Aresta *>::iterator iterCandidatas = candidatas.begin();
            while (iterCandidatas != candidatas.end())
            {
                Aresta *candidata = *iterCandidatas;
                if (impacto < impactos[candidata])
                {
                    break;
                }
                iterCandidatas++;
            }
            candidatas.insert(iterCandidatas, aresta);
        }
        iter++;
    }
    int maxIndex = (int) ((candidatas.size() - 1) * alfa);
    int indexSelecionado = geraIndiceAleatorioEntreZeroE(maxIndex);
    std::list<Aresta *>::iterator iterSelecionada = candidatas.begin();
    for (int i = 0; i < indexSelecionado; i++)
    {
        ++iterSelecionada;
    }
    Aresta *selecionada = *iterSelecionada;
    int componente = -1;
    for (int i = 0; i < floresta.size(); i++)
    {
        for (Aresta *aresta : floresta[i])
        {
            if (saoAdjacentes(aresta, selecionada))
            {
                componente = i;
                break;
            }
        }
    }
    Vertice *u = selecionada->origem;
    Vertice *v = selecionada->destino;
    int pesoU = u->peso;
    int pesoV = v->peso;
    int minAresta = std::min(u->peso, v->peso);
    int maxAresta = std::max(u->peso, v->peso);
    minimos[componente] = std::min(minimos[componente], minAresta);
    maximos[componente] = std::max(maximos[componente], maxAresta);
    floresta[componente].push_back(selecionada);
    visitados.insert(u->id);
    visitados.insert(v->id);
    arestas.erase(std::find(arestas.begin(), arestas.end(), selecionada));
}

/**
 * @brief Executa o algoritmo construtivo guloso para particionar o grafo em um número especificado de partições.
 *
 * @param grafo Ponteiro para o grafo a ser particionado.
 * @param numeroParticoes Número de partições desejadas.
 * @return Grafo* Ponteiro para o grafo resultante após a aplicação do algoritmo.
 *
 * O algoritmo segue os seguintes passos:
 * 1. Coleta e ordena as arestas do grafo.
 * 2. Inicializa uma floresta com o número de partições especificado.
 * 3. Preenche a floresta adicionando sempre a melhor aresta possível (que minimiza o gap resultante ao ser adicionada).
 * 4. Adiciona novas arestas à floresta até que todos os vértices do grafo original sejam visitados.
 * 5. Calcula o gap da solução encontrada e exibe o resultado.
 * 6. Cria um novo grafo representando a solução e retorna um ponteiro para ele.
 */
Grafo* Algoritmos::gulosoComum(Grafo *grafo, int numeroParticoes)
{
    std::list<Aresta *> arestas = coletaArestasOrdenadas(grafo);
    std::vector<std::vector<Aresta *>> floresta(numeroParticoes);
    std::vector<int> minimos(numeroParticoes);
    std::vector<int> maximos(numeroParticoes);
    for (int i = 0; i < numeroParticoes; i++)
    {
        floresta[i] = std::vector<Aresta *>();
    }
    std::set<int> visitados = std::set<int>();
    preencheFloresta(floresta, arestas, visitados, minimos, maximos);
    while (visitados.size() != grafo->vertices.size())
    {
        adicionaNovaAresta(floresta, arestas, visitados, minimos, maximos);
    }
    int gap = novoCalculaGap(minimos, maximos);
    std::cout << "Somatório dos gaps da solução encontrada = " << gap << '\n';
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

/**
 * @brief Executa o algoritmo guloso randomizado para encontrar uma solução para o problema de particionamento de grafos.
 * 
 * @param grafo Ponteiro para o grafo a ser particionado.
 * @param numeroParticoes Número de partições desejadas.
 * @param alfa Parâmetro de aleatoriedade para o algoritmo guloso randomizado.
 * @return Grafo* Ponteiro para o grafo resultante com a melhor solução encontrada.
 * 
 * O algoritmo realiza 50 iterações, em cada iteração:
 * 1. Coleta as arestas do grafo e as ordena.
 * 2. Inicializa uma floresta com o número de partições especificado de forma randomizada com base no alfa escolhido.
 * 3. Preenche a floresta de forma randomizada com base no parâmetro alfa.
 * 4. Adiciona novas arestas à floresta até que todos os vértices sejam visitados.
 * 5. Calcula o gap da solução encontrada e, se for melhor que o melhor gap encontrado até o momento, atualiza a melhor solução.
 * 
 * Ao final das iterações, o grafo resultante é construído a partir da melhor solução encontrada e retornado.
 */
Grafo *Algoritmos::gulosoRandomizado(Grafo *grafo, int numeroParticoes, float alfa)
{
    int melhorGap = std::numeric_limits<int>::max();
    std::vector<std::vector<Aresta *>> solucao(numeroParticoes);
    for (int i = 0; i < 10; i++) // LEMBRAR DE MULTIPLICAR RESULTADOS DE TEMPO DOS EXPERIMENTOS POR 3
    {
        std::list<Aresta *> arestas = coletaArestasOrdenadas(grafo);
        std::vector<std::vector<Aresta *>> floresta(numeroParticoes);
        std::vector<int> minimos(numeroParticoes);
        std::vector<int> maximos(numeroParticoes);
        for (int j = 0; j < numeroParticoes; j++)
        {
            floresta[j] = std::vector<Aresta *>();
        }
        std::set<int> visitados = std::set<int>();
        preencheFloresta(floresta, arestas, visitados, minimos, maximos);
        while (visitados.size() != grafo->vertices.size())
        {
            adicionaNovaArestaRandomizado(floresta, arestas, visitados, alfa, minimos, maximos);
        }
        int gap = calculaGap(floresta);
        if (gap < melhorGap)
        {
            melhorGap = gap;
            solucao = floresta;
        }
    }
    std::cout << "Somatório dos gaps da melhor solução encontrada = " << melhorGap << '\n';
    Grafo *grafoSolucao = new Grafo(0, 0, 1);
    for (int i = 0; i < solucao.size(); i++)
    {
        for (Aresta *aresta : solucao[i])
        {
            grafoSolucao->adicionaVertice(aresta->origem->id, aresta->origem->peso);
            grafoSolucao->adicionaVertice(aresta->destino->id, aresta->destino->peso);
            grafoSolucao->adicionaAresta(aresta->origem->id, aresta->destino->id);
        }
    }
    return grafoSolucao;
}