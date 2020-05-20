#include "basicheaders.h"
#include "basicqtheader.h"
#include "hierarchicallist.h"

void mainWorkFunc(std::string expression, std::string varValues)
{
    HierarchicalList* expList = new HierarchicalList(expression, varValues);

    expList->exprDelEmptBr();
    if (expList->exprLightCheck())
        return;

    int currPos = 0;
    expList->setNewHead(expList->makeHierarchichalList(0, currPos));
    if(!expList->getHead()) {
        expList->globalProblemMes("Error: not enought numbers!\n");
        return;
    }

    expList->setCurrNodeToHead();
    bool check = true;
    double res = expList->checkExprCorrectionAndCount(check);
    if (check == true) {
        expList->printLogMess("Expression correct and res is: ", res);
        std::stringstream resStr;
        resStr << "Expression correct and res is: " << res;
        QMessageBox::information(0,\
                                 "Success", \
                                 QString::fromStdString(resStr.str()));
    } else {
        expList->globalProblemMes("Something wrong with expression!\n");
    }
}
