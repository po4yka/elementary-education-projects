#include <iostream>
#include <limits>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>

using Vertex = char;
using INF = struct
{
    float val = std::numeric_limits<float>::max();
};

class Edge
{
public:
    explicit Edge(Vertex vert, float weight_) : next(vert), weight(weight_) { };
    Vertex getNextVertex() const { return next; }
    float getWeight() const { return  weight; }
private:
    Vertex next;
    float weight;
};

class Graph
{
public:
    Graph() = default;
    ~Graph() = default;
    void readGraph();
    void findWay(Vertex sourceVert, Vertex destVert);
private:
    std::map<Vertex, std::vector<Edge>> graph;

    static std::vector<Vertex> reconstruct(std::map<Vertex, Vertex> prevVert, Vertex curr);
    std::vector<Vertex> aStar(Vertex sourceVert, Vertex destVert);
    static float heuristic(Vertex v1, Vertex v2);
};

inline void Graph::readGraph()
{
    Vertex v1, v2;

    float weight;
    while (std::cin >> v1 >> v2 >> weight)
    {
        graph[v1].push_back(Edge(v2, weight));
    }
}

inline std::vector<Vertex> Graph::reconstruct(std::map<Vertex, Vertex> prevVert, Vertex curr)
{
    std::vector<Vertex> totalPath = { curr };
    while(prevVert.find(curr) != prevVert.end())
    {
        curr = prevVert[curr];
        totalPath.insert(totalPath.begin(), curr);
    }

    return totalPath;
}

inline std::vector<Vertex> Graph::aStar(Vertex sourceVert, Vertex destVert)
{
    std::vector<Vertex> closedSet, openSet = {sourceVert};
    std::map<Vertex, Vertex> prevVert;
    std::map<Vertex, INF> gScore, fScore;

    gScore[sourceVert].val = 0;
    fScore[sourceVert].val = heuristic(sourceVert, destVert);
    while (!openSet.empty())
    {
        Vertex current = *std::min_element(openSet.begin(), openSet.end(), [&fScore, destVert](Vertex const &a, Vertex const &b)
        {
            if(fScore[a].val == fScore[b].val)
            {
                return heuristic(a, destVert) < heuristic(b, destVert);
            }
            return fScore[a].val < fScore[b].val;
        });

        if(current == destVert)
        {
            return reconstruct(prevVert, current);
        }

        openSet.erase(std::remove(openSet.begin(), openSet.end(), current), openSet.end());
        closedSet.push_back(current);

        for (auto const &neighbor: graph[current])
        {
            if(std::find(closedSet.begin(), closedSet.end(), neighbor.getNextVertex()) != closedSet.end())
            {
                continue;
            }

            float tentativeGScore = gScore[current].val + neighbor.getWeight();

            if(!(std::find(openSet.begin(), openSet.end(), neighbor.getNextVertex()) != openSet.end()))
            {
                openSet.push_back(neighbor.getNextVertex());
            }
            else if(tentativeGScore >= gScore[neighbor.getNextVertex()].val)
            {
                continue;
            }

            prevVert[neighbor.getNextVertex()] = current;
            gScore[neighbor.getNextVertex()].val = tentativeGScore;
            fScore[neighbor.getNextVertex()].val = gScore[neighbor.getNextVertex()].val + heuristic(neighbor.getNextVertex(), destVert);

        }
    }
}

inline float Graph::heuristic(Vertex v1, Vertex v2)
{
    return static_cast<float>(abs(v1 - v2));
}

inline void Graph::findWay(Vertex sourceVert, Vertex destVert)
{
    std::vector<Vertex> path = aStar(sourceVert, destVert);

    std::string result;
    result.reserve(path.size());
    for (auto const &vertex: path)
    {
        std::cout << vertex;
    }
}

int main()
{
    Vertex sourceVert, destVert;
    std::cin >> sourceVert >> destVert;
    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Incorrect input." << std::endl;
        return 1;
    }

    auto *graph = new Graph();
    graph->readGraph();
    graph->findWay(sourceVert, destVert);
    delete graph;

    return 0;
}