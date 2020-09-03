#include <iostream>
#include <limits>
#include <vector>
#include <cmath>
#include <cassert>
#include <map>
#include <algorithm>
#include <queue>
#include <initializer_list>

#define DEBUG

using Path = std::string;

template <typename W>
struct pathCost
{
    W edgeWeight{};
    size_t heuristicWeight{};
    W getSum() { return edgeWeight + static_cast<W>(heuristicWeight); }
};

/**
 *
 * @tparam L label type
 * @tparam W weight type
 */
template <typename L, typename W>
class Vertex
{
private:
    L label{};
    std::map<Vertex*, pathCost<W>> neighbours;
    bool isClosed{};
    Vertex *previous = nullptr;
    W pathLength = std::numeric_limits<W>::infinity();
    // FIXME: delete it
    W heuristicValue = std::numeric_limits<W>::infinity();

public:
    explicit Vertex(W label_) : label(label_) { };
    Path getReconstructedPath()
    {
        Path path;
        for (Vertex *current = this; current != nullptr; current = current->previous)
        {
            path += current->label;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
    W getPathLength() const { return pathLength; }
    void setPathLength(W length) { pathLength = length; }
    bool getClosedState() const { return isClosed; }
    void setClosedState() { isClosed = true; }
    L getLabel() const { return label; }
    std::map<Vertex*, pathCost<W>> getNeighbours() const { return neighbours; }
    void setPreviousVertex(Vertex* &vertex) { previous = vertex; }
    W getHeuristicValueToOtherVert(const Vertex* vert)
    {
        return abs(static_cast<W>(label - vert->getLabel()));
    }
    void setCosts(Vertex* vertex, W vert)
    {
        neighbours[vertex].edgeWeight = vert;
        neighbours[vertex].heuristicWeight = static_cast<size_t>(abs(label - vertex->getLabel()));
    }
};

template <typename L, typename W>
class VertexWithPriority
{
private:
    Vertex<L, W> *vertex{};
    // pathLength + heuristicFunction
    W priority = std::numeric_limits<W>::signaling_NaN();

public:
    VertexWithPriority(Vertex<L, W>* vertex_, W priority_) : vertex(std::move(vertex_)), priority(priority_) { };

    // for std::priority_queue sort working
    bool operator< (const VertexWithPriority &other) const
    {
        return this->priority > other.priority;
    }
    Vertex<L, W>* getCurrVertex() const { return vertex; }
};

/*
void printPriorityQueue(std::priority_queue<VertexWithPriority> queue)
{
    while (!queue.empty())
    {
        auto top = queue.top();
        std::cout << "    " << top.vertex->label
             << ": length = " << top.vertex->pathLength
             << ", path = " << top.vertex->getReconstructedPath()
             << ", priority = " << top.priority
             << ", is closed = " << (top.vertex->isClosed ? "true" : "false")
             << std::endl;
        queue.pop();
    }
}
*/

template <typename L, typename W>
Path aStarAlgorithm(Vertex<L, W> *sourceVertex, L destVertexLabel)
{
    std::priority_queue<VertexWithPriority<L, W>> openVertices;
    sourceVertex->setPathLength(0);
    // push to queue source start vertex: 0 length & 0 priority
    openVertices.push({sourceVertex, 0});

    #ifndef DEBUG
        std::cout << "  Current priority queue:" << std::endl;
        printPriorityQueue(openVertices);
    #endif

    while (!openVertices.empty())
    {
        // get vertex with a minimum priority
        auto currentVertex = openVertices.top().getCurrVertex();
        openVertices.pop();

        // if it closed vertex go to the next
        if (currentVertex->getClosedState())
        {
            continue;
        }

        #ifndef DEBUG
            std::cout << "Starting to work with min-priority non-closed vertex: " << currentVertex->label << std::endl;
        #endif

        // path to dist was successfully found
        if (currentVertex->getLabel() == destVertexLabel)
        {

            #ifndef DEBUG
                    std::cout << "  End is reached." << std::endl;
            #endif

            return currentVertex->getReconstructedPath();
        }

        #ifndef DEBUG
            std::cout << "  Updating neighbors:" << std::endl;
        #endif

        #ifndef DEBUG
            bool someNeighborIsUpdated = false;
        #endif

        // go thought neighbors of current vertex std::map<Vertex*, W>
        for (const auto &neighborEdge : currentVertex->getNeighbours())
        {
            // current neighbor vertex
            Vertex<L, W> *edgeEndVertex = neighborEdge.first;

            if (edgeEndVertex->getClosedState())
                continue;

            // current weight to it
            W edgeLength = neighborEdge.second.edgeWeight;
            if (edgeEndVertex->getPathLength() > currentVertex->getPathLength() + edgeLength)
            {
                #ifndef DEBUG
                    someNeighborIsUpdated = true;
                #endif

                edgeEndVertex->setPathLength(currentVertex->getPathLength() + edgeLength);
                edgeEndVertex->setPreviousVertex(currentVertex);
                W priority = edgeEndVertex->getPathLength() + edgeEndVertex->heuristicValue;

                #ifndef DEBUG
                    std::cout << "    " << edgeEndVertex->getLabel()
                                          << ": length = " << edgeEndVertex->getPathLength()
                                          << ", path = " << edgeEndVertex->getReconstructedPath()
                                          << ", priority = " << priority
                                          << std::endl;
                #endif

                openVertices.push({edgeEndVertex, priority});
            }
        }

        #ifndef DEBUG
            if (someNeighborIsUpdated)
            {
                std::cout << "  Current priority queue:" << std::endl;
                printPriorityQueue(openVertices);
            }
            else
            {
                std::cout << "    Nothing to update." << std::endl;
            }
        #endif

        currentVertex->setClosedState();

        #ifndef DEBUG
                    std::cout << "Vertex " << currentVertex->label << " is closed now." << std::endl;
        #endif
    }

    throw std::runtime_error("No path exists!");
}

int main()
{
    char sourceVertex, destVertex;
    std::cin >> sourceVertex >> destVertex;

    std::map<char, Vertex<char, float>*> vertices;

    while (true)
    {
        char edgeStart, edgeEnd;
        float edgeLength;
        std::cin >> edgeStart;
        if (std::cin.eof())
        {
            break;
        }
        std::cin >> edgeEnd >> edgeLength;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Incorrect input." << std::endl;
            return 1;
        }

        auto endVertexPtr = vertices[edgeEnd];
        if (endVertexPtr == nullptr)
            endVertexPtr = vertices[edgeEnd] = new Vertex<char, float>(edgeEnd);

        auto startVertexPtr = vertices[edgeStart];
        if (startVertexPtr == nullptr)
            startVertexPtr = vertices[edgeStart] = new Vertex<char, float>(edgeStart);

        // set weight for neighbors
        startVertexPtr->setCosts(endVertexPtr, edgeLength);
    }

    auto path = aStarAlgorithm(vertices[sourceVertex], destVertex);

    #ifdef DEBUG
        std::cout << "Ultimate path: ";
    #endif

    std::cout << path << std::endl;

    return 0;
}