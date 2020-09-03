/**
 * https://habr.com/ru/post/198682/
 */

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

#define LETTER_COUNT 5

// #define DEBUG

typedef char Vertex;
typedef int Index;

struct BohrVertex
{
    Index nextVertex[LETTER_COUNT]{};       // A, C, G, T, N - i-ый элемент - номер вершины,
                                            // в которую осуществляется переход по символу с № i в алфавите
    Index patternNum{};                     // Номер строки образа
    bool isEndOfShape{false};               // Флаг конца подстроки <=> явл-ся ли вершина исходной строкой
    Index parentNum{};                      // Индекс родителя
    Index suffixLink{-1};                   // Индекс суффиксальной ссылки набольшего суффикса
    Index moveSymbol[LETTER_COUNT]{};       // Индекс перехода по другим символам алфавита
    Vertex parentMoveSymbol;                // Индекс символа, по которому осуществляется переход от родителя
                                            // к текущей вершине

    BohrVertex(size_t parentNum_, Vertex parentMoveSymbol_) : parentNum (parentNum_), parentMoveSymbol(parentMoveSymbol_)
    {
        std::fill_n(nextVertex, LETTER_COUNT, -1);
        std::fill_n(moveSymbol, LETTER_COUNT, -1);
    }
};

class BohrAlg
{
private:
    std::vector<BohrVertex> bohr;
    std::vector<std::string> pattern;
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
        bohr.emplace_back(0,0);    // Инициализируем корень
    }

    void add(std::string &str)
    {
        int n{};
        for(char &i : str)
        {
            int ch;
            if(alphabet.find(i) != alphabet.end())
            {
                ch = alphabet[i];
            }
            else
            {
                ch = 5;
            }

            if(bohr[n].nextVertex[ch] == -1)
            {
                bohr.emplace_back(n, ch);
                bohr[n].nextVertex[ch] = bohr.size() - 1;
            }

            n = bohr[n].nextVertex[ch];
        }

        bohr[n].isEndOfShape = true;
        pattern.push_back(str);
        bohr[n].patternNum = static_cast<int>(pattern.size()) - 1;
    }

    /**
     * Суффиксальная ссылка вершина v указатель на вершину u, такую что строка u - наибольший
     * cобственный суффикс строки v, или, если такой вершины нет в боре, то указатель на корень
     * @param vert
     * @return
     */
    int getSuffix(Index vert)
    {
#ifdef DEBUG
        std::cout << "New vert in suffix: " << vert << std::endl;
#endif
        if(bohr[vert].suffixLink == -1)
        {
            if(vert == 0 || bohr[vert].parentNum == 0)
            {
                bohr[vert].suffixLink = 0;
            }
            else
            {
                bohr[vert].suffixLink = getAutoMove(getSuffix(bohr[vert].parentNum), bohr[vert].parentMoveSymbol);
            }
        }

#ifdef DEBUG
        std::cout << "Suffix: " << bohr[vert].suffixLink << std::endl;
#endif
        return bohr[vert].suffixLink;
    }

    /**
     * Если из текущей вершины есть ребро c символом ch, то пройдем по нему,
     * в обратном случаем пройдем по суффиксной ссылке и запустимся рекурсивно от новой вершины.
     * @param vert
     * @param ch
     * @return
     */
    int getAutoMove(int vert, int ch)
    {
        if (bohr[vert].moveSymbol[ch] == -1)
        {
            if (bohr[vert].nextVertex[ch] != -1)
            {
                bohr[vert].moveSymbol[ch] = bohr[vert].nextVertex[ch];
            }
            else
            {
                if (vert == 0)
                {
                    bohr[vert].moveSymbol[ch] = 0;
                }
                else
                {
                    bohr[vert].moveSymbol[ch] = getAutoMove(getSuffix(vert), ch);
                }
            }
        }
#ifdef DEBUG
        std::cout << "getAutoMove: " << bohr[vert].moveSymbol[ch] << std::endl;
#endif
        return bohr[vert].moveSymbol[ch];
    }

    void find(const std::string &str)
    {
        int u{};
        for(int i = 0; i < str.length(); ++i)
        {
            int ch;
            if(alphabet.find(str[i]) != alphabet.end())
            {
                ch = alphabet[str[i]];
            }
            else
            {
                ch = 5;
            }
#ifdef DEBUG
        std::cout << "Call functions: u = " << u << "; ch = " << ch << std::endl;
#endif
            u = getAutoMove(u, ch);
            print(u, i + 1);
        }
    }

    void print(int vert, int i)
    {
        for(int u = vert; u != 0; u = getSuffix(u))
        {
            if(bohr[u].isEndOfShape)
            {
                std::cout << i - pattern[bohr[u].patternNum].length() + 1 << " " << bohr[u].patternNum + 1 << std::endl;
            }
        }
#ifdef DEBUG
      std::cout << "Print" << std::endl;
#endif
    }
};

int main()
{
    BohrAlg solution;
    std::string text, keyword;
    std::cin >> text;
    int n;
    std::cin >> n;
    for(int i = 0; i < n; i++)
    {
        std::cin >> keyword;
        solution.add(keyword);
    }
    solution.find(text);

    return 0;
}