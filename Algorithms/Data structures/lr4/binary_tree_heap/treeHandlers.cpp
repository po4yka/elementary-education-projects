#include "basicheaders.h"
#include "tree.h"
#include "customvector.h"

namespace lrstruct {
    bool Node::stepLoopSwitcher = false;
    bool Node::isStepByStepMode = false;

    int Node::findDepth(Node* root) {
        if(root == nullptr) {
            return 0;
        }

        if(root->_left == nullptr && root->_right == nullptr) {
            return 1;
        }
        else {
            int l = findDepth(root->_left);
            int r = findDepth(root->_right);

            _depth = (1 + ((l > r) ? l : r));
            return _depth;
            //T O(n) S O(n)
        }
    }

    int Node::findMinNode(Node* root) {
        // Base case
        if (root == nullptr)
            return INT_MAX;

        // Return minimum of 3 values:
        // 1) Root's data 2) Min in Left Subtree
        // 3) Min in right subtree
        int res = root->_data.val;
        int lres = findMinNode(root->_left);
        int rres = findMinNode(root->_right);
        if (lres < res)
            res = lres;
        if (rres < res)
            res = rres;
        return res;
    }

    int Node::findMaxNode(Node* root) {
        // Base case
        if (root == nullptr)
            return INT_MIN;

        // Return maximum of 3 values:
        // 1) Root's data 2) Max in Left Subtree
        // 3) Max in right subtree
        int res = root->_data.val;
        int lres = findMaxNode(root->_left);
        int rres = findMaxNode(root->_right);
        if (lres > res)
            res = lres;
        if (rres > res)
            res = rres;
        return res;
    }

    int Node::getLargestNumberBsd() {
        if (_right == nullptr) {
            return _data.val;
        }
        else {
            _right->getLargestNumberBsd();
        }

        // never happen
        return false;
    }

    int Node::getSmallestNumberBsd() {
        if (_left == nullptr) {
            return _data.val;
        }
        else {
            _left->getSmallestNumberBsd();
        }

        // never happen
        return false;
    }

    bool Node::isSymetrical() {
        lrstruct::Vector <int> left_side;
        lrstruct::Vector <int> right_side;
        if(_left == nullptr || _right == nullptr)
            return false;
        _left->symetricalChecking(left_side);
        _right->symetricalChecking(right_side);

        if (left_side.size() != right_side.size()) {
            return false;
        }

        for (int i = 0; static_cast<size_t>(i) < left_side.size(); i++) {
            if (left_side[i] != right_side[i]) {
                return false;
            }
        }

        return true;

    }

    void Node::symetricalChecking(lrstruct::Vector<int>& vec) {
        if (_left != nullptr && _right != nullptr) {
            vec.push_back(0);
            _left->symetricalChecking(vec);
            _right->symetricalChecking(vec);
        }
        else if (_left != nullptr) {
            vec.push_back(-1);
            _left->symetricalChecking(vec);
        }
        else if (_right != nullptr) {
            vec.push_back(1);
            _right->symetricalChecking(vec);
        }
        else {
            vec.push_back(2);
        }
    }

    int Node::get_next_num(std::string &s, int &i, int& numb) {
        qDebug() << "get_next_num() " << "for i = " << i << endl;
        int start = i;
        while (i != static_cast<int>(s.size()) && \
               s[static_cast<unsigned long>(i)] != '(' && s[static_cast<unsigned long>(i)] != ')')
            i++;

        try {
            numb =  stoi(s.substr(static_cast<unsigned long>(start), static_cast<unsigned long>(i) - \
                                 static_cast<unsigned long>(start)));
        }  catch (...) {
            qDebug() << "Invalid input string!" << endl;
            return 1;
        }

        return 0;
    }

    int Node::dfs(std::string &s, int &i, Node*& root) {
        qDebug() << "dfs started " << "for i = " << i << endl;
        // base case
        int size = static_cast<int>(s.size());
        if (i >= size || s[static_cast<unsigned long>(i)] == ')') {
            // NULL is OK
            root = nullptr;
            return 0;
        }

        if(s[static_cast<unsigned long>(i)] == '(') i++;

        if(s[static_cast<unsigned long>(i)] == '#') {
            root = nullptr;
            i++;
            return 0;
        }

        int num;
        if(get_next_num(s, i, num))
            // something went wrong
            return 1;
        lrstruct::Data _data;
        _data.val = num;
        Node* temp_root = new Node(_data);

        int check;
        Node* left;
        check = dfs(s, i, left);
        if(check == 1)
            return 1;
        Node* right;
        check = dfs(s, i, right);
        if(check == 1)
            return 1;

        temp_root->_left = left;
        temp_root->_right = right;

        if (s[static_cast<unsigned long>(i)] == ')') i++;

        root = temp_root;

        // everything OK
        return 0;
    }

    /**
     * @brief Node::convertBracketRepresentationToBinTree
     * @param inputStr
     * Input string with bracket representation of binary tree
     * @return
     */
    int Node::convertBracketRepresentationToBinTree(std::string inputStr, Node*& root) {
        qDebug() << "convertBracketRepresentationToBinTree()" << endl;
        int i = 0;
        int check = dfs(inputStr, i, root);
        if(check == 1)
            return 1;

        return 0;
    }
}
