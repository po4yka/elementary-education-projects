#include <iostream>
#include <limits>
#include <vector>
#include <cmath>
#include <cassert>
#include <map>

#define DEBUG

/**
 * Using Dijkstra’s Algorithm as one of the Greedy Algorithm
 */

/**
 *
 * @tparam T Type of weight
 * @tparam N Type of vertex
 */
template <typename T, typename N>
class AdjacencyMatrix
{
public:
    AdjacencyMatrix() = default;

    void addEdge(N sourceVert, N destVert, T weight = T());
    bool connectionCheck(size_t sourceIndex, size_t destIndex) const;
    T getEdgeWeight(size_t vSourceIndex, size_t vDestIndex) const;
    size_t getVertCount() const;
    size_t getVertIndexByName(N name) const;
    N getVertNameByIndex(size_t index) const;

    // TODO：Using iterator to visit inner element
    std::vector<size_t> getAdjacencyVertices(size_t vVertexIndex) const;

private:
    std::vector<std::vector<T>> matrix{};
    size_t vertCount{};
    // The fastest way is to save two maps, one for each direction
    std::map<N, size_t> verticesNames{};
    std::map<size_t , N> verticesNamesForPath{};
};

template <typename T, typename N>
void AdjacencyMatrix<T, N>::addEdge(N sourceVert, N destVert, T weight)
{
    auto itSource = verticesNames.find(sourceVert);
    auto itDest = verticesNames.find(destVert);

    // Source vertex has not yet been added
    if(itSource == verticesNames.end())
    {
        verticesNames.insert(std::make_pair(sourceVert, vertCount));
        verticesNamesForPath.insert(std::make_pair(vertCount++, sourceVert));
        matrix.resize(vertCount);
        for(auto &curr : matrix)
        {
            curr.resize(vertCount);
        }
    }
    // Destination vertex has not yet been added
    if(itDest == verticesNames.end())
    {
        verticesNames.insert(std::make_pair(destVert, vertCount));
        verticesNamesForPath.insert(std::make_pair(vertCount++, destVert));
        matrix.resize(vertCount);
        for(auto &curr : matrix)
        {
            curr.resize(vertCount);
        }
    }

    // Get indexes of vertexes
    size_t vSourceIndex = verticesNames[sourceVert];
    size_t vDestIndex = verticesNames[destVert];

    // NOTE：Vertex X to X`s self default zero
    if (vSourceIndex != vDestIndex)
    {
        matrix[vSourceIndex][vDestIndex] = weight;

        // Cause oriented graph
        //matrix[vDestIndex][vSourceIndex] = weight;
    }
}

template <typename T, typename N>
inline bool AdjacencyMatrix<T, N>::connectionCheck(size_t sourceIndex, size_t destIndex) const
{
    assert(sourceIndex < vertCount && destIndex < vertCount);
    return matrix[sourceIndex][destIndex] != 0;
}

template <typename T, typename N>
inline T AdjacencyMatrix<T, N>::getEdgeWeight(size_t vSourceIndex, size_t vDestIndex) const
{
    assert(vSourceIndex < vertCount && vDestIndex < vertCount);
    return matrix[vSourceIndex][vDestIndex];
}

template <typename T, typename N>
std::vector<size_t> AdjacencyMatrix<T, N>::getAdjacencyVertices(size_t vVertexIndex) const
{
    assert(vVertexIndex < vertCount);

    std::vector<size_t> AdjacencyVertices;
    for (size_t i = 0; i < vertCount; ++i)
    {
        if (matrix[vVertexIndex][i] != 0)
        {
            AdjacencyVertices.push_back(i);
        }
    }

    return AdjacencyVertices;
}

template<typename T, typename N>
size_t AdjacencyMatrix<T, N>::getVertCount() const
{
    return vertCount;
}

template<typename T, typename N>
size_t AdjacencyMatrix<T, N>::getVertIndexByName(N name) const
{
    return verticesNames.at(name);
}

template<typename T, typename N>
N AdjacencyMatrix<T, N>::getVertNameByIndex(size_t index) const
{
    return verticesNamesForPath.at(index);
}

template <typename T, typename N>
T dijkstra(const AdjacencyMatrix<T, N>& adjacencyMatrix, N sourceName, N destName, std::vector<N>& path)
{
    size_t sourceIndex = adjacencyMatrix.getVertIndexByName(sourceName);
    size_t destIndex = adjacencyMatrix.getVertIndexByName(destName);
    assert(sourceIndex < adjacencyMatrix.getVertCount() && destIndex < adjacencyMatrix.getVertCount() && path.empty());

    std::vector<T> dest(adjacencyMatrix.getVertCount(), 0);
    std::vector<size_t> prevVertices(adjacencyMatrix.getVertCount(), 0);

    dijkstra(adjacencyMatrix, sourceIndex, prevVertices, dest);

    std::vector<size_t> tempPath;
    for (size_t i = destIndex; i != sourceIndex; i = prevVertices[i])
    {
        tempPath.push_back(i);
    }
    tempPath.push_back(sourceIndex);

    for(auto iter = tempPath.rbegin(); iter != tempPath.rend(); ++iter)
    {
        path.push_back(adjacencyMatrix.getVertNameByIndex(*iter));
    }

    return dest[destIndex];
}

template <typename T, typename N>
void dijkstra(const AdjacencyMatrix<T, N>& adjacencyMatrix, size_t sourceIndex, std::vector<size_t> &prevVertices, std::vector<T> &dest)
{
    size_t vertCount = adjacencyMatrix.getVertCount();

    assert(sourceIndex < adjacencyMatrix.getVertCount());

    static T INF = std::numeric_limits<T>::max();

    // Array of visited peaks (index is equal to the number of peaks)
    std::vector<bool> visited(vertCount, false);

    // (Step 1) Init by infinite values
    for (size_t i = 0; i < vertCount; ++i)
    {
        prevVertices[i] = sourceIndex;
        dest[i] = INF;
    }
    dest[sourceIndex] = T();

    // Sort through all the vertices
    for (size_t i = 0; i < vertCount; ++i)
    {
        T minWeight = INF;
        size_t minWeightIndex = 0;

        // Find the peak with the minimum distance to it,
        // in the first step it will be a start node
        for (size_t k = 0; k < vertCount; ++k)
        {
            if (!visited[k] && dest[k] <= minWeight)
            {
                minWeightIndex = k;
                minWeight = dest[k];
            }
        }
        // mark the selected vertex as passed
        visited[minWeightIndex] = true;

        // loop in which we give all the vertices adjacent
        // to the selected path weight to it
        for (size_t k = 0; k < vertCount; ++k)
        {
            // If this vertex is not passed and it is adjacent to the selected one and if the sum of the weight
            // of the selected vertex and edge to the current one is less than the current weight at the moment,
            // then we change the weight value of the current vertex.
            #ifdef DEBUG
                std::cout << "Curr: " << minWeightIndex << "; dist: " << k << ". Visited: " << visited[k] << "; Connected: " <<
                    adjacencyMatrix.connectionCheck(minWeightIndex, k) << "; Dist: " << (minWeight + adjacencyMatrix.getEdgeWeight(minWeightIndex, k) < dest[k]) << std::endl;
                if(adjacencyMatrix.connectionCheck(minWeightIndex, k))
                {
                    std::cout << "Curr dist: " << dest[k] << "; Available dist: " << minWeight + adjacencyMatrix.getEdgeWeight(minWeightIndex, k) << std::endl;
                }
            #endif
            if (!visited[k] && adjacencyMatrix.connectionCheck(minWeightIndex, k)
                && minWeight + adjacencyMatrix.getEdgeWeight(minWeightIndex, k) < dest[k])
            {
                dest[k] = minWeight + adjacencyMatrix.getEdgeWeight(minWeightIndex, k);
                // remember the ancestor of the vertex K
                prevVertices[k] = minWeightIndex;
            }
        }
        #ifdef DEBUG
            std::cout << std::endl;
        #endif
    }
}

int main()
{
    char sourceVertex, destVertex;
#ifndef DEBUG
    std::cin >> sourceVertex >> destVertex;
#else
    sourceVertex = 'a';
    destVertex = 'e';
#endif
    AdjacencyMatrix<float, char> adjacencyMatrix;

#ifndef DEBUG
    char tempSource, tempVert;
    float tempWight;
    while(std::cin >> tempSource >> tempVert >> tempWight)
    {
        adjacencyMatrix.addEdge(tempSource, tempVert, tempWight);
    }
#else
    adjacencyMatrix.addEdge('a', 'b', 3.0);
    adjacencyMatrix.addEdge('b', 'c', 1.0);
    adjacencyMatrix.addEdge('c', 'd', 1.0);
    adjacencyMatrix.addEdge('a', 'd', 5.0);
    adjacencyMatrix.addEdge('d', 'e', 1.0);
#endif

    std::vector<char> path;

    int smallestWeight = dijkstra(adjacencyMatrix, sourceVertex, destVertex, path);

    for (auto vertex : path)
    {
        std::cout << vertex;
    }
}