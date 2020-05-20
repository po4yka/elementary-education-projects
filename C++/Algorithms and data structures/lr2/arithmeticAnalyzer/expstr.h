#ifndef EXPSTR_H
#define EXPSTR_H

#include "basicheaders.h"

class ExpStr
{
private:
    std::string str;

public:
    ExpStr();
    ExpStr(std::string expression);
    ~ExpStr();
    void deleteEmptyBrackets();
    static const std::string allowableSignes;
    int checkExpCorrectionLight();  // 0 - верное, 1 - нет
    inline char getSymbByIndex(int index)
    {
        return str[index];
    }
    inline int getStrLength()
    {
        return str.length();
    }
    std::string getSubStr(int beginPos, int CurrentPos);
};

#endif // EXPSTR_H
