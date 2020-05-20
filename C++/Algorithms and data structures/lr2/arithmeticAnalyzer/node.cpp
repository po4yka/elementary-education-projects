#include "node.h"
#include "basicheaders.h"
#include "basicqtheader.h"

/**
 * @brief Node::Node : по умолчанию создаём пустой узел
 */
Node::Node()
{
    next = nullptr;
    isAtom = 0;
    content.expr = nullptr;
    content.character = '\0';
    content.number = 0;
}

Node::Node(int number): Node()
{
    isAtom = 1;
    content.number = number;
}

Node::Node(char sign): Node()
{
    isAtom = 2;
    content.character = sign;
}

Node::Node(bool flag, char var): Node()
{
    /** передаём, просто чтобы вызвать
     * нужный конструктор
     */
    Q_UNUSED(flag);
    isAtom = 3;
    content.character = var;
}

Node::Node(Node* expr): Node()
{
    content.expr = expr;
}

int Node::getAtomValue()
{
    return isAtom;
}

int Node::getContNumb()
{
    return content.number;
}

char Node::getContChar()
{
    return content.character;
}

Node* Node::getNext()
{
    return next;
}

void Node::setNext(Node* next)
{
    this->next = next;
}

void Node::setContExprNext(Node* next)
{
    this->content.expr = next;
}

Node* Node::getContExprVal()
{
    return this->content.expr;
}
