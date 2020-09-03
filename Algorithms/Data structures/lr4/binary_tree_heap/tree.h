#ifndef TREE_H
#define TREE_H

#include "basicheaders.h"
#include "customvector.h"

namespace lrstruct {
    struct Data {
        int val;
        int level;
        int pos;
        // position for drawing
        int posX{0};
        int posY{0};
        bool new_node{false};
    };

    class Node : public QObject
    {
        Q_OBJECT

    public:
        // cuz public links is not a problem
        Node *_left = nullptr, *_right = nullptr;
        static bool isStepByStepMode;
        void setStepByStepFlag();

        const int DrawingSize{34};
        const QPoint start_pos{260,30};

        static int left_nodes;
        static int right_nodes;
        Data _data;
        int _depth;

        bool contain(int number);
        int getSmallestNumberBsd();
        int getLargestNumberBsd();
        int findMaxNode(Node*);
        int findMinNode(Node*);
        int findDepth(Node*);
        void getSortedVector(lrstruct::Vector<int>& a);
        bool isSymetrical();

        static int convertBracketRepresentationToBinTree(std::string, Node*&);
        lrstruct::Vector<Data> setPositionOfNodesForVisualization();
        lrstruct::Vector<QPoint> getVectorOfLinesBetwenNodes();

        static bool stepLoopSwitcher;
        static void loopLatency();

        Node(Data NewData);
        ~Node();

    public slots:
        static bool isValidBst(Node*);
        static bool isValidBinHeap(Node*);
        static void changeLoopStepSwitcherState();

    signals:
        void drawCurrNode(lrstruct::Node*, int);

    private:
        void returnDataVector(lrstruct::Vector<Data>& vector);
        void loopForLinesConnection(lrstruct::Vector<QPoint>& vec);
        void setPos(int& pos);
        void setLevel(int& level);
        void symetricalChecking(lrstruct::Vector<int>& vec);
        void setDataForDrawing(Node& root);

        static int get_next_num(std::string &s, int &i, int&);
        // Depth-first search for bracket representation
        static int dfs(std::string&, int&, Node*&);
    };
}


#endif // TREE_H
