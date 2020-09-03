/**
 * https://habr.com/ru/post/198682/
 * https://docplayer.ru/52553787-Klassicheskie-algoritmy-poiska-obrazca-v-stroke.html
 */

#include <iostream>
#include <map>
#include <vector>
#include <list>

typedef char Vertex;
typedef int Index;

struct BohrVertex
{
    BohrVertex(int parent, char chr) :
            parentNum(parent), parentMoveSymbol(chr), isEndOfShape(false), suffixLink(-1), suffixLinkGood(-1) { }

    std::map<Vertex, Index> nextVertex;     // A, C, G, T, N - i-ый элемент - номер вершины
    std::map<Vertex, Index> moveSymbol;     // Индекс перехода по другим символам алфавита
    bool isEndOfShape{false};               // Флаг конца подстроки <=> явл-ся ли вершина исходной строкой
    Index patternNum{};                     // Индекс родителя
    Index suffixLink{-1};                   // Индекс суффиксальной ссылки набольшего суффикса
    Index suffixLinkGood{};
    Index parentNum{};
    Vertex parentMoveSymbol{};
};

using Bohr = std::vector<BohrVertex*>;
using Pattern = std::vector<std::string>;

class BohrAlg
{
private:
    Bohr bohr;
    Pattern patterns;
    Vertex joker{};
    std::string text;
    std::string input;

    std::map<Vertex, int> alphabet
    {
            { 'A', 0 },
            { 'C', 1 },
            { 'G', 2 },
            { 'T', 3 },
            { 'N', 4 }
    };
public:

    BohrAlg()
    {
        bohr.push_back(new BohrVertex(0, '$'));
    }

    void start()
    {
        std::cin >> text >> input >> joker;

        addToBohr(input);
        find();
    }

    void addToBohr(const std::string& pattern)
    {
        int n{};
        for(auto &chr : pattern)
        {
            if(bohr[n]->nextVertex.find(chr) == bohr[n]->nextVertex.end())
            {
                bohr.push_back(new BohrVertex(n, chr));
                bohr[n]->nextVertex[chr] = bohr.size() - 1;
            }

            n = bohr[n]->nextVertex[chr];
        }

        bohr[n]->isEndOfShape = true;
        patterns.push_back(pattern);
        bohr[n]->patternNum = static_cast<int>(patterns.size()) - 1;
    }

    int getSuffixLink(int node)
    {
        if(bohr[node]->suffixLink == -1)
        {
            if(node == 0 || bohr[node]->parentNum == 0)
            {
                bohr[node]->suffixLink = 0;
            }
            else
            {
                bohr[node]->suffixLink = getAutoMove(getSuffixLink(bohr[node]->parentNum), bohr[node]->parentMoveSymbol);
            }
        }

        return bohr[node]->suffixLink;
    }

    int getAutoMove(int node, char chr)
    {
        if(bohr[node]->moveSymbol.find(chr) == bohr[node]->moveSymbol.end())
        {
            if(bohr[node]->nextVertex.find(chr) != bohr[node]->nextVertex.end())
            {
                bohr[node]->moveSymbol[chr] = bohr[node]->nextVertex[chr];
            }
            else if(bohr[node]->nextVertex.find(joker) != bohr[node]->nextVertex.end())
            {
                bohr[node]->moveSymbol[chr] = bohr[node]->nextVertex[joker];
            }
            else if(node == 0)
            {
                bohr[node]->moveSymbol[chr] = 0;
            }
            else
            {
                bohr[node]->moveSymbol[chr] = getAutoMove(getSuffixLink(node), chr);
            }
        }

        return bohr[node]->moveSymbol[chr];
    }

    void check(int node, int i)
    {
        for(int u = node; u != 0; u = getSuffixLink(u))
        {
            if(bohr[u]->isEndOfShape)
            {
                std::cout << i + 2 - patterns[bohr[u]->patternNum].length() << std::endl;
            }
        }
    }

    void find()
    {
        int u = 0;
        for(int i = 0; i < text.length(); i++)
        {
            u = getAutoMove(u, text[i]);
            check(u, i);
        }
    }
};

int main()
{
    BohrAlg bohr;
    bohr.start();

    return 0;
}