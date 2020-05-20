#ifndef HIERARCHICALLIST_H
#define HIERARCHICALLIST_H

#define DEBUG 1

#include "basicheaders.h"
#include "basicqtheader.h"
#include "stackList.h"
#include "node.h"
#include "expstr.h"

class HierarchicalList
{
private:
    Stack<Node*> stackNodes;    // стек с узлами списка
    Node *head;
    Node *currentPtr;           // текущий рабочий элемент
    ExpStr *expression;         // арифметическое выражение
    std::map<char, int> varValues;

public:
    HierarchicalList();
    HierarchicalList(std::string str);
    HierarchicalList(std::string str, std::string vars);
    ~HierarchicalList();

    /**
     * @brief globalProblemMes : вывод информации об ошибке
     * @param message : сообщение с информацией об ошибке
     */
    static void globalProblemMes(std::string message);

    /**
     * @brief connectWithPrev :
     * 1. Если прерыдущего элемента не сущ., то
     * новый элемент становится головой списка;
     * 2. Если слудеющий элемент является символом
     * или числом, то записываем новый элемент на
     * тот же "уровень" в иерархическом списке.
     * 3. Если у предыдущего элемента нет "ответвления"
     * в выражение - другой "уровень", то пишем в него,
     * иначе пишем в next.
     * @param prevEl : предыдущий элемент списка
     * @param nextEl : новый элемент списка
     */
    void connectWithPrev(Node* prevEl, Node* nextEl);

    std::map<char, int> makeValDict(std::string varInputLine);

    int readNumber(int& currentSymbNumb);

    Node* makeHierarchichalList(int deep, int& currPos);

    void exprDelEmptBr();

    int exprLightCheck();  // 0 - верное, 1 - нет

    void printLogMess(std::string message);

    void printLogMess(std::string message, char character);

    void printLogMess(std::string message, int numb);

    void printLogMess(std::string message, double numb);

    bool printList(Node* head, int deep, int nodeNumber);

    void setNewHead(Node* head);

    Node* getHead();

    double checkExprCorrectionAndCount(bool& check);

    void setCurrNodeToHead();
};

#endif // HIERARCHICALLIST_H
