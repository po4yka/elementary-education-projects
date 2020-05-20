#include "basicheaders.h"
#include "tree.h"
#include "customvector.h"

namespace lrstruct {
    void Node::changeLoopStepSwitcherState() {
            stepLoopSwitcher = false;
    }

    void Node::setStepByStepFlag() {
        isStepByStepMode = true;
    }

    void Node::loopLatency() {
        qDebug() << "Loop latency for step-by-step started" << endl;
        for( ; ; ) {
            QApplication::processEvents();
            if(stepLoopSwitcher == false) break;
        }
        stepLoopSwitcher = true;
    }

    bool Node::isValidBst(Node* root) {
        if(isStepByStepMode) { stepLoopSwitcher = true; }
        qDebug() << "Checking for valid BST started" << endl;
        if(!root) return true;

        std::stack<Node*> nodeStack;
        Node* current = root;

        int curr_val, pre_val = INT_MIN;
        int first_compare = 1;          // in first compare

        while(!nodeStack.empty() || current) {
            if(current) {
                qDebug() << "Val: " << current->_data.val << " Mode: " << isStepByStepMode << endl;
            }
            if(current) {
                if(isStepByStepMode == true)
                    emit root->drawCurrNode(current, 1);
                nodeStack.push(current);
                current = current->_left;
            } else {
                current = nodeStack.top();
                if(isStepByStepMode == true)
                    emit root->drawCurrNode(current, 2);
                curr_val = current->_data.val;

                if(curr_val <= pre_val && first_compare == 0) {
                    if(isStepByStepMode == true)
                        emit root->drawCurrNode(current, 3);
                    root->changeLoopStepSwitcherState();
                    return false;
                }

                first_compare = 0;      // after first compare
                pre_val = curr_val;
                nodeStack.pop();
                current = current->_right;
            }
            if(isStepByStepMode == true)
                loopLatency();
        }

        qDebug() << "End of checking for valid BST tree" << endl;
        root->changeLoopStepSwitcherState();
        return true;
    }

    bool Node::isValidBinHeap(Node* root) {
        // create an empty queue and enqueue root node
        std::queue<Node*> queue;
        queue.push(root);

        // take a boolean flag which becomes true when an empty left
        // or right child is seen for a node
        bool nullseen = false;

        // run till queue is not empty
        while (queue.size())
        {
            // process front node in the queue
            Node* current = queue.front();
            queue.pop();
            if(isStepByStepMode == true)
                emit root->drawCurrNode(current, 1);

            // left child is non-empty
            if (current->_left)
            {
                if(isStepByStepMode == true)
                    loopLatency();
                // if either heap-property is violated
                if (nullseen || current->_left->_data.val >= current->_data.val) {
                    if(isStepByStepMode == true)
                        emit root->drawCurrNode(current->_left, 3);
                    root->changeLoopStepSwitcherState();
                    return false;
                }

                // enqueue left child
                queue.push(current->_left);
            }
            // left child is empty
            else {
                nullseen = true;
            }

            // right child is non-empty
            if (current->_right)
            {
                // if either heap-property is violated
                if (nullseen || current->_right->_data.val >= current->_data.val) {
                    if(isStepByStepMode == true)
                        emit root->drawCurrNode(current->_right, 3);
                    root->changeLoopStepSwitcherState();
                    return false;
                }

                // enqueue left child
                queue.push(current->_right);
            }
            // right child is empty
            else {
                nullseen = true;
            }

            if(isStepByStepMode == true)
                loopLatency();
        }

        // we reach here only when the given binary tree is a min-heap
        root->changeLoopStepSwitcherState();
        return true;
    }
}
