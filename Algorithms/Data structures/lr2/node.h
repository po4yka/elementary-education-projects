#ifndef NODE_H
#define NODE_H


class Node
{
public:
    Node();
    Node(char sign);
    Node(int number);
    Node(bool flag, char var);
    Node(Node* expr);
    int getAtomValue();
    int getContNumb();
    char getContChar();
    Node* getNext();
    void setNext(Node* next);
    void setContExprNext(Node* next);
    Node* getContExprVal();

private:
    int isAtom;  // 0 - узел; 1 - число; 2 - знак; 3 - переменная
    Node* next;
    union {
        Node* expr;
        char character;
        int number;
    } content;
};

#endif // NODE_H
