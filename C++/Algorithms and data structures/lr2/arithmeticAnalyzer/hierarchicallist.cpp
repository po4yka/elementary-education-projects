#include "hierarchicallist.h"
#include "basicheaders.h"
#include "basicqtheader.h"

HierarchicalList::HierarchicalList()
{
    head = nullptr;
    currentPtr = nullptr;
    expression = new ExpStr();
}

HierarchicalList::HierarchicalList(std::string str): \
    HierarchicalList()
{
    expression = new ExpStr(str);
}

HierarchicalList::HierarchicalList(std::string str, std::string val): \
    HierarchicalList()
{
    expression = new ExpStr(str);
    varValues = makeValDict(val);
}

HierarchicalList::~HierarchicalList()
{
    head = nullptr;
    currentPtr = nullptr;
    /** тут память, отведённая на вектор сама очиститься,
     *  expression также будет очищен после уничтожения
     * экземпляра класса, т.к. память выделена в стеке.
     * delete не работает, т.к. это не указатели.
     */
}

void HierarchicalList::globalProblemMes(std::string message)
{
    QFile log("../log.txt");
    log.open(QIODevice::Append | QIODevice::Text);
    QTextStream logStream(&log);

    std:: cout << '\n' << message << '\n';
    logStream << '\n' << QString::fromStdString(message) << '\n';
    QMessageBox::warning(0, "Mistake", QString::fromStdString(message));

    log.close();
}

void HierarchicalList::connectWithPrev(Node* prevEl, Node* nextEl)
{
    if (!prevEl) {
        head = nextEl;
    } else {
        if (prevEl->getAtomValue()) {
            prevEl->setNext(nextEl);
        } else if (!prevEl->getContExprVal()) {
            prevEl->setContExprNext(nextEl);
        } else {
            prevEl->setNext(nextEl);
        }
    }
}

int HierarchicalList::readNumber(int& currentSymbNumb)
{
    unsigned int beginPos = currentSymbNumb;
    while (expression->getSymbByIndex(currentSymbNumb) >= '0' && \
           expression->getSymbByIndex(currentSymbNumb) <= '9') {
        currentSymbNumb++;
    }

    std::string tmpStrNumb = expression->getSubStr(beginPos, currentSymbNumb);
    tmpStrNumb[tmpStrNumb.length() + 1] = '\0';

    return std::stoi(tmpStrNumb);
}

std::map<char, int> HierarchicalList::makeValDict(std::string varInputLine)
{
    std::map<char, int> valDict;
    const std::regex reg(R"(\s*\(\s*([a-z])\s*([1-9][0-9]*)\s*\)\s*)");
    std::smatch match;
    for (std::sregex_iterator i = std::sregex_iterator( \
             varInputLine.begin(), varInputLine.end(), reg); \
         i != std::sregex_iterator(); ++i) {
        std::smatch match = *i;
        valDict.insert(std::pair<char, int>(match[1].str()[0], \
                       std::stoi(match[2].str().c_str())));
    }

    return valDict;
}

void HierarchicalList::printLogMess(std::string message)
{
    QFile log("../log.txt");
    log.open(QIODevice::Append | QIODevice::Text);
    QTextStream logStream(&log);

    std::cout << message;
    logStream << QString::fromStdString(message);

    log.close();
}

void HierarchicalList::printLogMess(std::string message, char character)
{
    QFile log("../log.txt");
    log.open(QIODevice::Append | QIODevice::Text);
    QTextStream logStream(&log);

    std::cout << message << character << '\n';
    logStream << QString::fromStdString(message) << character << '\n';

    log.close();
}

void HierarchicalList::printLogMess(std::string message, int numb)
{
    QFile log("../log.txt");
    log.open(QIODevice::Append | QIODevice::Text);
    QTextStream logStream(&log);

    std::cout << message << numb << '\n';
    logStream << QString::fromStdString(message) << numb << '\n';

    log.close();
}

void HierarchicalList::printLogMess(std::string message, double numb)
{
    QFile log("../log.txt");
    log.open(QIODevice::Append | QIODevice::Text);
    QTextStream logStream(&log);

    std::cout << message << numb << '\n';
    logStream << QString::fromStdString(message) << numb << '\n';

    log.close();
}

bool HierarchicalList::printList(Node* head, int deep, int nodeNumber)
{
    bool isCorrect = 1;
    bool zeroFound = false;
    QFile log("../log.txt");
    log.open(QIODevice::Append | QIODevice::Text);
    QTextStream logStream(&log);
    for (int i = 0; i < deep; i++) {
        std::cout << "   ";
        logStream << QString::fromStdString("   ");
    }
    if (nodeNumber) {
        std::cout << "Node: " << nodeNumber << " ";
        logStream << QString::fromStdString("Node: ") << nodeNumber << \
                    QString::fromStdString(" ");
    }
    Node* ListHead = nullptr;
    Node* curList = nullptr;
    Node* ptr = head;
    static int NN = 1;
    int minNN = NN;
    while (ptr) {
        if (ptr->getAtomValue() == 1) {
            std::cout << ptr->getContNumb() << " ";
            logStream << ptr->getContNumb() << QString::fromStdString(" ");
            if (!ptr->getContNumb() && ptr != head) zeroFound = true;
            if (!ptr->getNext()) isCorrect = 0;
        } else if (ptr->getAtomValue() == 2) {
            std::cout << ptr->getContChar() << " ";
            logStream << ptr->getContChar() << QString::fromStdString(" ");
            if (zeroFound && ptr->getContChar() == '/') isCorrect = 0;
            if (ptr->getNext()) isCorrect = 0;
        } else {
            std::cout << "(Node №) -> " << NN;
            logStream << QString::fromStdString("(Node №) -> ") << NN++;
            Node* tmp = new Node();
            tmp->setContExprNext(ptr);
            if (!curList)
                ListHead = tmp;
            else
                curList->setContExprNext(tmp);
            curList = tmp;
        }
        ptr = ptr->getNext();
    }
    std::cout << "NULL";
    logStream << QString::fromStdString("NULL");
    if (zeroFound) {
        std::cout << "Zero is found!\n";
        logStream << QString::fromStdString("Zero was found!\n");
    }
    std::cout << "\n\n";
    logStream << QString::fromStdString("\n\n");
    curList = ListHead;
    bool isCorrectRec = 1;
    log.close();
    while (curList) {
        isCorrectRec &= printList(curList->getContExprVal()->getContExprVal(), \
                                  deep + 1, minNN);
        curList = curList->getNext();
        minNN++;
    }
    curList = ListHead;
    while(curList) {
        ListHead = curList;
        curList = curList->getNext();
        delete curList;
    }
    return isCorrect && isCorrectRec;
}

void HierarchicalList::exprDelEmptBr()
{
    expression->deleteEmptyBrackets();
}

int HierarchicalList::exprLightCheck()
{
    if (expression->checkExpCorrectionLight())
        return 1;
    else
        return 0;
}

void HierarchicalList::setCurrNodeToHead()
{
    currentPtr = head;
}

void HierarchicalList::setNewHead(Node *head)
{
    this->head = head;
}

Node* HierarchicalList::getHead()
{
    return head;
}

Node* HierarchicalList::makeHierarchichalList(int deep, int& currPosStr)
{
    Node* currHead = nullptr;

    while(expression->getStrLength() != currPosStr) {
        if (expression->getSymbByIndex(currPosStr) == '(')
        {
            currPosStr++;
            if (DEBUG) {printLogMess("New node was created!\n");};
            Node* tmp = currentPtr;
            Node* newNode = new Node(makeHierarchichalList(deep++, \
                                                           currPosStr));

            if (!currHead)
                currHead = newNode;
            currentPtr = tmp;
            connectWithPrev(currentPtr, newNode);
            stackNodes.push(newNode);
            currentPtr = newNode;
        } else if ((expression->allowableSignes.find(expression->\
                   getSymbByIndex(currPosStr)) != std::string::npos))
        {
            if (DEBUG) {printLogMess("Sign is found: ", \
                                     expression->getSymbByIndex(currPosStr));}
            // случай отрицательного числа
            if (expression->getSymbByIndex(currPosStr) == '-' && \
                    isdigit(expression->getSymbByIndex(currPosStr+1)))
            {
                        currPosStr++;
                        int tmpNumber = readNumber(currPosStr);
                        tmpNumber *= -1;
                        Node* newNode = new Node(tmpNumber);
                        if (!currHead)
                            currHead = newNode;
                        if (DEBUG) { printLogMess("Number is found: ", tmpNumber);}
                        connectWithPrev(currentPtr, newNode);
                        currentPtr = newNode;
            } else
            {
                Node* newNode = new Node(expression->getSymbByIndex(currPosStr));
                if (!currHead)
                    currHead = newNode;
                currPosStr++;
                connectWithPrev(currentPtr, newNode);
                currentPtr = newNode;
            }
        } else if (isdigit(expression->getSymbByIndex(currPosStr)))
        {
            int tmpNumber = readNumber(currPosStr);
            if (DEBUG) { printLogMess("Number is found: ", tmpNumber);}
            Node* newNode = new Node(tmpNumber);
            if (!currHead)
                currHead = newNode;
            connectWithPrev(currentPtr, newNode);
            currentPtr = newNode;
        } else if (isalpha(expression->getSymbByIndex(currPosStr)))
        {
            if (DEBUG) { printLogMess("Variable is found: ", \
                         expression->getSymbByIndex(currPosStr));}
            Node* newNode = new Node(true, expression->getSymbByIndex(currPosStr));
            if (!currHead)
                currHead = newNode;
            currPosStr++;
            connectWithPrev(currentPtr, newNode);
            currentPtr = newNode;
        } else if (expression->getSymbByIndex(currPosStr) == ')') {
            if (DEBUG) {printLogMess("The end of the current node!\n");}
            currPosStr++;
            return currHead;
        } else currPosStr++;
    }
    if (expression->getSymbByIndex(currPosStr) == ')') {
        if (DEBUG) {printLogMess("The end of the current node!\n");}
        currentPtr->setNext(nullptr);
        currPosStr++;
        return currHead;
    }

    return currHead;
}

// 0 - узел; 1 - число; 2 - знак; 3 - переменная  //

double HierarchicalList::checkExprCorrectionAndCount(bool& check)
{

    Stack<double> resStack;
    double a = 0;
    double b = 0;

    while(currentPtr) {
        if (currentPtr->getAtomValue() == 1) {
            resStack.push(currentPtr->getContNumb());
            currentPtr = currentPtr->getNext();
        } else
        if (currentPtr->getAtomValue() == 2) {
            if (resStack.getTop() != 2) {
                    check = false;
                    return 0;
                }
            switch(currentPtr->getContChar()) {
                case '-':
                    a = resStack.pop();
                    b = resStack.pop();
                    resStack.push(a - b);
                    break;
                case '+':
                    a = resStack.pop();
                    b = resStack.pop();
                    resStack.push(a + b);
                    break;
                case '*':
                    a = resStack.pop();
                    b = resStack.pop();
                    resStack.push(a * b);
                    break;
                case '/':
                    a = resStack.pop();
                    b = resStack.pop();
                    if (b == 0) {
                        check = false;
                        return 0;
                    }
                    resStack.push(((a * 1.0) / b));
                    break;
            }
            currentPtr = currentPtr->getNext();
        } else
        if (currentPtr->getAtomValue() == 3) {
            if (varValues.find(currentPtr->getContChar()) == varValues.end()) {
                globalProblemMes("Error: variable with no value was found!\n");
                check = false;
                return 0;
            } else {
                resStack.push(varValues[currentPtr->getContChar()]);
                currentPtr = currentPtr->getNext();
            }
        } else
        if (currentPtr->getAtomValue() == 0)
        {
            Node* tmpPtr = currentPtr;
            currentPtr = currentPtr->getContExprVal();
            double tmp = checkExprCorrectionAndCount(check);
            if (check == false) {
                return 0;
            }
            resStack.push(tmp);
            currentPtr = tmpPtr->getNext();
        }
    }

    if (check == false || resStack.getStackSize() != 1)
        return 0;
    else
        return resStack.pop();
}
