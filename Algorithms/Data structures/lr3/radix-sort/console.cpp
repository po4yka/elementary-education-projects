#include "console.h"

Console::Console()
{
}

void Console::consoleExec()
{
    if(DEBUG) std::cout << "console exec" << std::endl;
    std::vector<int32_t> inpArr;
    int tempInt;
    QString tempStr;
    bool sortFlag, stepFlag = false, consoleMode = true;
    std::cout << "Which type of radix sort do you want to use?" << std::endl;
    std::cout << "for LSD type - 1, for MSD - 2" << std::endl;
    std::cin >> tempInt;
    std::cin.ignore();
    tempInt == 1 ? sortFlag = true : sortFlag = false;

    std::cout << "Input array of 32-bit sign numbers for sort:" << std::endl;
    std::cout << "Remark: separate numbers by space." << std::endl;
    std::string str;
    std::getline(std::cin, str);
    QString input(str.c_str());
    QTextStream stream(&input);

    foreach (QString i,QString(stream.readAll()).split(QRegExp("[ \n]"),QString::SkipEmptyParts))
    {
        inpArr.push_back(static_cast<int32_t>(i.toInt()));
    }

    radixSort *workSort = new radixSort(sortFlag, stepFlag, consoleMode);
    workSort->writeData(inpArr);
    workSort->mainSortFunc();

    //workSort->resPrint();
}
