#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <map>
#include <queue>
#include <exception>

/**
 * https://is.gd/gmjqpa
 */

using Vertex = char;
using Graph = std::map<Vertex, std::map<Vertex, size_t>>;
static size_t INF = std::numeric_limits<size_t>::max();

bool findWayBFS(Graph &graph, Vertex source, Vertex stock, std::map<Vertex, Vertex> &stockPath)
{
    std::vector<Vertex> visitedVertexes = { source };
    std::queue<Vertex> bfsQueue;
    std::map<Vertex, Vertex> prevVert;
    Vertex currVert;

    // В начале очередь состоит из единственной вершины
    bfsQueue.push(source);

    while(!bfsQueue.empty())
    {
        currVert = bfsQueue.front();
        // Удаляем первую вершину в очереди
        bfsQueue.pop();

        // Для всех дуг (u, v), исходящих из вершины u,
        // для которых вершина v ещё не посещена, выполняем следующие шаги:
        for(auto const &neighbors : graph[currVert])
        {
            if (find(visitedVertexes.begin(), visitedVertexes.end(), neighbors.first) == visitedVertexes.end() &&
                neighbors.second > 0)
            {
                // Отмечаем вершину v как посещённую, с родителем u.
                visitedVertexes.push_back(neighbors.first);
                // Добавляем вершину v в конец очереди.
                bfsQueue.push(neighbors.first);
                // Если v = t, выходим из обоих циклов: мы нашли кратчайший путь.
                prevVert[neighbors.first] = currVert;
            }
        }
    }
    stockPath = prevVert;

    return find(visitedVertexes.begin(), visitedVertexes.end(), stock) != visitedVertexes.end();
}

size_t findMaxFlowEKalg(Graph &graph, Vertex source, Vertex stock)
{
    std::map<Vertex, Vertex> stockPath;
    size_t maxFlow = 0;
    Vertex from, where;

    // Находим кратчайший путь из источника в сток. Если такого пути нет, останавливаемся.
    while (findWayBFS(graph, source, stock, stockPath))
    {
        size_t maxWayFlow = INF;
        where = stock;

        // ищем ребро с минимальной пропускной способностью
        while (where != source)
        {
            maxWayFlow = std::min(maxWayFlow, graph[stockPath[where]][where]);
            where = stockPath[where];
        }

        maxFlow += maxWayFlow;
        where = stock;

        while (where != source)
        {
            from = stockPath[where];
            // Для каждого ребра на найденном пути увеличиваем поток на c_{min},
            // а в противоположном ему — уменьшаем на c_{min}
            graph[from][where] -= maxWayFlow;
            graph[where][from] += maxWayFlow;
            where = stockPath[where];
        }
    }

    return maxFlow;
}

int main()
{
    Graph graph{}, residualGraph{};
    Vertex source{}, stock{}, from{}, where{};
    size_t edgeNumber{}, bandwidth{}, maxFlow{};

    try
    {
        std::cin >> edgeNumber >> source >> stock;
        if(std::cin.fail())
        {
            throw std::invalid_argument("Invalid start par.");
        }
        while (edgeNumber--)
        {
            std::cin >> from >> where >> bandwidth;
            if (std::cin.fail())
            {
                throw std::invalid_argument("Invalid edges.");
            }
            graph[from][where] = bandwidth;
        }
    }
    catch(const std::invalid_argument &e)
    {
        std::cerr << e.what();
        std::cin.clear();
    }

    residualGraph = graph;
    maxFlow = findMaxFlowEKalg(residualGraph, source, stock);

    std::cout << maxFlow << std::endl;
    for (auto const &curr : graph)
    {
        for (auto const &neighbor : graph[curr.first])
        {
            int temp = static_cast<int>(neighbor.second - residualGraph[curr.first][neighbor.first]) < 0 ? 0 :
                       static_cast<int>(neighbor.second - residualGraph[curr.first][neighbor.first]);
            std::cout << curr.first << " " << neighbor.first << " " << temp << std::endl;
        }
    }
}