#ifndef RADIX_TEST_H
#define RADIX_TEST_H

#include "basicheaders.h"

#define ITERCOUNT 32

//  Radix sort comparator for 32-bit two's complement integers
class radixSort : public QObject
{
    Q_OBJECT

    private:
        std::vector<int32_t> inpArr;
        bool sortFlag;
        bool stepFlag;
        bool consoleMode;
        bool printResFlag;

        void lsd_radix_sort();
        void msd_radix_sort(std::vector<int>::iterator, std::vector<int>::iterator, int);
        void printResFile();

        int lsb;

        int operationsCounter;

    public:
        explicit radixSort(QObject *parent = nullptr);
        radixSort(bool, bool, bool);
        ~radixSort();

        void mainSortFunc();
        void implementForFile(QString);
        void implementForLine(std::vector<int32_t>);

        void writeData(std::vector<int32_t>);
        void resPrint();
    signals:
        void sendDeactivateNextStepBut();
        void strToPrintInWindow(QString);
    public slots:
        void callWorkSortFunc();

};

#endif // RADIX_TEST_H
