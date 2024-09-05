#include "Reader.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

Grafo *Reader::readGrafo(std::ifstream &arquivoInstancia)
{
    Grafo *grafo = new Grafo(0, 0, 1);
    std::string cabecalhoVertices = "set V";
    std::string cabecalhoPesos = "param w";
    std::string cabecalhoArestas = "set E";
    std::string linha;
    while(getline(arquivoInstancia, linha))
    {
        if (linha.find(cabecalhoVertices) != std::string::npos)
        {
            readVertices(arquivoInstancia, grafo);
        } 
        else if (linha.find(cabecalhoPesos) != std::string::npos)
        {
            readPesos(arquivoInstancia, grafo);
        } 
        else if (linha.find(cabecalhoArestas) != std::string::npos)
        {
            readArestas(arquivoInstancia, grafo);
        }
    }
    return grafo;
}

void Reader::readVertices(std::ifstream &arquivoInstancia, Grafo *grafo)
{
    std::string linha;
    while (getline(arquivoInstancia, linha))
    {
        if (linha.find(";") != std::string::npos)
        {
            break;
        }
        std::stringstream stream(linha);
        std::string item;
        while (getline(stream, item, ' '))
        {
            if (item.size() > 0)
            {
                int id = std::stoi(item);
                grafo->adicionaVertice(id);
            }
        }
    }
}

void Reader::readPesos(std::ifstream &arquivoInstancia, Grafo *grafo)
{
    std::string linha;
    while (getline(arquivoInstancia, linha))
    {
        if (linha.find(";") != std::string::npos)
        {
            break;
        }
        std::stringstream stream(linha);
        std::string item;
        std::vector<int> infoLinha;
        while (getline(stream, item, ' '))
        {
            if (item.size() > 0) 
            {
                infoLinha.push_back(std::stoi(item));
            }
        }
        grafo->getVertice(infoLinha[0])->peso = infoLinha[1];
    }
}

void Reader::readArestas(std::ifstream &arquivoInstancia, Grafo *grafo)
{   
    std::string linha;
    while (getline(arquivoInstancia, linha))
    {
        if (linha.find(";") != std::string::npos)
        {
            break;
        }
        if (linha.length() == 0)
        {
            continue;
        }
        std::stringstream stream(linha);
        std::string aresta;
        while (getline(stream, aresta, ' '))
        {
            if (aresta.size() > 0 && aresta.find(",") != std::string::npos)
            {
                aresta.erase(0, 1);
                aresta.erase(aresta.size() - 1, 1);
                std::stringstream streamAresta(aresta);
                std::string itemAresta;
                std::vector<int> infoAresta;
                while (getline(streamAresta, itemAresta, ','))
                {
                    infoAresta.push_back(std::stoi(itemAresta));
                }
                grafo->adicionaAresta(infoAresta[0], infoAresta[1]);
            }
        }
    }
}
