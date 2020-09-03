#include <iostream>
#include <limits>
#include <vector>
#include <cmath>
#include <cassert>
#include <map>

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
std::vector<N> greedyAlg(const AdjacencyMatrix<T, N>& adjacencyMatrix, N sourceName, N destName)
{
    static T INF = std::numeric_limits<T>::max();

    size_t vertCount = adjacencyMatrix.getVertCount();
    size_t sourceIndex = adjacencyMatrix.getVertIndexByName(sourceName);
    size_t destIndex = adjacencyMatrix.getVertIndexByName(destName);
    assert(sourceIndex < adjacencyMatrix.getVertCount() && destIndex < adjacencyMatrix.getVertCount());

    // Array of visited peaks (index is equal to the number of peaks)
    std::vector<bool> visited(vertCount, false);
    std::vector<size_t> path;

    // current vert
    size_t currVertIndex = sourceIndex;
    bool final = false;

    for (size_t i = 0; i < vertCount && !final; ++i)
    {
        T minWeight = INF;
        size_t newCurr = currVertIndex;
        for (size_t k = 0; k < vertCount; ++k)
        {
            if(!visited[k] && adjacencyMatrix.connectionCheck(currVertIndex, k) && adjacencyMatrix.getEdgeWeight(currVertIndex, k) <= minWeight)
            {
                if(adjacencyMatrix.getEdgeWeight(currVertIndex, k) == minWeight)
                {
                    if(adjacencyMatrix.getVertNameByIndex(k) > adjacencyMatrix.getVertNameByIndex(newCurr))
                        continue;
                }
                newCurr = k;
                minWeight = adjacencyMatrix.getEdgeWeight(currVertIndex, k);
            }
        }
        visited[newCurr] = true;

        // if there's no way next go back by the way
        if(newCurr == currVertIndex && currVertIndex != 0)
        {
            currVertIndex = path.back();
            path.pop_back();
            i--;
            continue;
        }

        path.push_back(currVertIndex);
        currVertIndex = newCurr;
        // Reached final Vert
        if(currVertIndex == destIndex)
        {
            final = true;
            path.push_back(currVertIndex);
        }
    }

    std::vector<N> pathRes;
    for(auto &curr : path)
    {
        pathRes.push_back(adjacencyMatrix.getVertNameByIndex(curr));
    }
    return pathRes;
}

int main()
{
    char sourceVertex, destVertex;

    std::cin >> sourceVertex >> destVertex;

    AdjacencyMatrix<float, char> adjacencyMatrix;

    char tempSource, tempVert;
    float tempWight;
    while(std::cin >> tempSource >> tempVert >> tempWight)
    {
        adjacencyMatrix.addEdge(tempSource, tempVert, tempWight);
    }

    std::vector<char> path = greedyAlg(adjacencyMatrix, sourceVertex, destVertex);
    for (auto vertex : path)
    {
        std::cout << vertex;
    }

    return 0;
}