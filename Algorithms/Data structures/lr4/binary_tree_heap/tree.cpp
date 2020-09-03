#include "basicheaders.h"
#include "tree.h"
#include "customvector.h"

namespace lrstruct
{
    int Node::left_nodes = 0;
    int Node::right_nodes = 0;

    Node::Node(Data NewData) {
        _data = NewData;
        _depth = 0;
    }

    Node::~Node() {
        delete _left;
        delete _right;
    }

    /**
     * @brief Node::contain
     * Check contation of number in tree by value.
     * @param number
     * Number which contain needed to check.
     * @return
     */
    bool Node::contain(int number) {
        if (number < _data.val) {
            if (_left == nullptr) {
                return false;
            }
            else {
                _left->contain(number);
            }
        }
        else if (number > _data.val) {
            if (_right == nullptr) {
                return false;
            }
            else {
                _right->contain(number);
            }
        }
        else {
            return true;
        }

        // never happen
        return false;
    }

    void Node::getSortedVector(lrstruct::Vector<int>& a) {
        if (_left != nullptr) {
            _left->getSortedVector(a);
        }
        a.push_back(_data.val);

        if (_right != nullptr) {
            _right->getSortedVector(a);
        }
    }

    /**
     * @brief Node::loopForLinesConnection
     * Recursion function for drawing connection between two nodes.
     * @param vec
     */
    void Node::loopForLinesConnection(lrstruct::Vector<QPoint>& vec) {

        if(_left != nullptr) {
            QPoint point(_data.posX,_data.posY);
            vec.push_back(point);
            _left->loopForLinesConnection(vec);
        }

        if(_right != nullptr) {
            QPoint point(_data.posX,_data.posY);
            vec.push_back(point);
            _right->loopForLinesConnection(vec);
        }

        QPoint point(_data.posX,_data.posY);
        vec.push_back(point);
    }

    /**
     * @brief Node::returnDataVector
     *
     * @param vector
     */
    void Node::returnDataVector(lrstruct::Vector<Data>& vector) {
        if(_left != nullptr) {
            _left->returnDataVector(vector);
        }

        vector.push_back(_data);

        if(_right != nullptr) {
            _right->returnDataVector(vector);
        }
    }

    // Mostly for drawing

    /**
     * @brief Node::setPos
     * Set positions of nodes.
     * @param pos
     */
    void Node::setPos(int &pos) {
        if(_left != nullptr) {
            _left->setPos(pos);
        }
        _data.pos = pos;
        pos++;
        if(_right != nullptr) {
            _right->setPos(pos);
        }
    }

    void Node::setLevel(int& level) {
        _data.level = level;
        level++;
        if(_left != nullptr) {
            _left->setLevel(level);
        }
        if(_right != nullptr) {
            _right->setLevel(level);
        }
        level--;
    }

    void Node::setDataForDrawing(Node& root) {
        if(_left != nullptr){
            _left->setDataForDrawing(root);
        }

        _data.posY = (_data.level* root.DrawingSize)+root.start_pos.y();
        if(_data.pos < root._data.pos) {
            _data.posX = root.start_pos.x() - ((root._data.pos - _data.pos)*root.DrawingSize);
        }
        else if(_data.pos > root._data.pos) {
            _data.posX = root.start_pos.x() + ((_data.pos - root._data.pos)*root.DrawingSize);
        }
        else {
            _data.posX = root.start_pos.x();
        }

        if(_right != nullptr) {
            _right->setDataForDrawing(root);
        }
    }

    lrstruct::Vector<Data> Node::setPositionOfNodesForVisualization() {
        int level{0},pos{1};
        this->setLevel(level);
        this->setPos(pos);
        this->setDataForDrawing(*this);
        lrstruct::Vector<Data> vec;
        this->returnDataVector(vec);

        return vec;

    }

    lrstruct::Vector<QPoint> Node::getVectorOfLinesBetwenNodes() {
        lrstruct::Vector<QPoint> vec;
        this->loopForLinesConnection(vec);
        return vec;
    }
}
