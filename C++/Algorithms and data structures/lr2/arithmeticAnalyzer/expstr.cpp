#include "expstr.h"
#include "hierarchicallist.h"

const std::string ExpStr::allowableSignes = "+-*/";

ExpStr::ExpStr()
{
    str.clear();
    str = "\0";
}

ExpStr::ExpStr(std::string str): ExpStr()
{
    this->str = str;
}

ExpStr::~ExpStr()
{
    str = nullptr;
}

void ExpStr::deleteEmptyBrackets()
{
    static const std::regex rgx_pattern("\\(\\s*\\)");
    std::string res;
    res.reserve(str.size());
    std::regex_replace(std::back_insert_iterator<std::string>(res), \
                       str.cbegin(), str.cend(), rgx_pattern, "");
    str = res;
}

int ExpStr::checkExpCorrectionLight()
{
    if (str[0] == '\0') {
        HierarchicalList::globalProblemMes("The expression is empty!\n");
        return 1;
    }

    int openBrackets = 0, closeBrackets = 0;
    for (std::string::size_type i = 0; i < str.length(); i++) {
        if (str[i] == '(') {
            openBrackets++;
            continue;
        }
        if (str[i] == ')'){
            closeBrackets++;
            continue;
        }
        if (allowableSignes.find(str[i]) == std::string::npos && \
                !isdigit(str[i]) && !isalpha(str[i]) && str[i] != ' ') {
            HierarchicalList::globalProblemMes("Error: Incorrect input symbol!");
            return 1;
        }
    }

    if(openBrackets != closeBrackets) {
        HierarchicalList::globalProblemMes("ERROR: Invalid number of brackets!");
        return 1;
    }

    return 0;
}

std::string ExpStr::getSubStr(int beginPos, int CurrentPos)
{
    return str.substr(beginPos, \
                      CurrentPos - beginPos);
}
