#ifndef CONSOLE_H
#define CONSOLE_H

#include "basicheaders.h"
#include "radixSort.h"

#define DEBUG 0

class Console
{
    private:
        radixSort *workSort;
    public:
        Console();
        void consoleExec();
};

#endif // CONSOLE_H
