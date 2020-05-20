#include "basicheaders.h"
#include "radixSort.h"
#include "console.h"
#include "partitioncustom.h"

radixSort::radixSort(bool sortFlag, bool stepFlag, bool consoleMode)
{
    this->stepFlag = stepFlag;
    this->sortFlag = sortFlag;
    this->consoleMode = consoleMode;
    this->printResFlag = false;

    lsb = 0;

    operationsCounter = 0;
}

//  Least significant digit radix sort
void radixSort::lsd_radix_sort()
{
    operationsCounter++;
    QFile log("../log.txt");
    log.open(QIODevice::Append);
    QTextStream logStream(&log);

    if(lsb == ITERCOUNT) {sendDeactivateNextStepBut(); return;}

    if (stepFlag == true)
    {
        logStream << "Step " << lsb << ": " << endl;
        if(consoleMode == false) emit strToPrintInWindow("Step " + QString::number(lsb) + ": \n");
        for (auto i : inpArr)
        {
            logStream << i << ' ';
            if(consoleMode == false) emit strToPrintInWindow(QString::number(i) + " ");
        }
        logStream << endl;
        if(consoleMode == false) emit strToPrintInWindow("\n");
    }
    stablePartitionCustom(inpArr.begin(), inpArr.end(), radix_test(lsb), operationsCounter);
    ++lsb;
    log.close();
}

// Most significant digit radix sort (recursive)
void radixSort::msd_radix_sort(std::vector<int32_t>::iterator first, \
                               std::vector<int32_t>::iterator last, int msb = 31)
{
    if (first != last && msb >= 0)
    {
        operationsCounter++;
        std::vector<int32_t>::iterator mid = partitionCustom(first, last, \
                                             radix_test(msb), operationsCounter);
        msb--; // decrement most-significant-bit
        msd_radix_sort(first, mid, msb); // sort left partition
        msd_radix_sort(mid, last, msb); // sort right partition
    }
}

// write input array to radix sort class object
void radixSort::writeData(std::vector<int32_t> inpArr)
{
    this->inpArr.reserve(inpArr.size());
    std::copy(inpArr.begin(), inpArr.end(), std::back_inserter(this->inpArr));
}

void radixSort::mainSortFunc()
{
    if(DEBUG) qDebug() << "mainSortFunc implements" << endl;
    QFile log("../log.txt");
    log.open(QIODevice::Append);
    QTextStream logStream(&log);

    if (sortFlag == true)
    {
        if(stepFlag == true)
        {
            if (lsb == 1)
            {
                logStream << "LSD radix sort process:" << endl;
                if(consoleMode == false) emit strToPrintInWindow("LSD radix sort process:\n");
            }
        } else
        {
            for(int i = 0; i < ITERCOUNT; ++i)
                lsd_radix_sort();
        }

        lsd_radix_sort();
    } else
    {
        msd_radix_sort(inpArr.begin(), inpArr.end());
    }

    if ((lsb == ITERCOUNT || sortFlag == false) && printResFlag == false)
    {
        resPrint();
        printResFlag = true;
    }

    log.close();

    QFile assessment("../assessment.txt");
    assessment.open(QIODevice::Append);
    QTextStream assessmentStream(&assessment);

    QString temp;
    temp = sortFlag ? " LSD " : " MSD ";
    assessmentStream << "For sorting of " << inpArr.size() << " elements array ";
    assessmentStream << "by " << temp << "sort ";
    assessmentStream << "was made " << operationsCounter << " operations." << endl << endl;

    assessment.close();
}

void radixSort::implementForFile(QString fileName)
{
    QFile file(fileName);
    std::vector<int32_t> inpArr;

    // creating and naming log file
    QFile log("../log.txt");
    log.open(QFile::WriteOnly|QFile::Truncate);
    QTextStream logStream(&log);
    logStream << "************\n";
    logStream << "* LOG FILE *\n";
    logStream << "************\n";
    log.close();

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        foreach (QString i,QString(stream.readAll()).split(QRegExp("[\r\n]"),QString::SkipEmptyParts))
        {
            inpArr.push_back(static_cast<int32_t>(i.toInt()));
        }
    }
    file.close();

    writeData(inpArr);
    mainSortFunc();
}

void radixSort::implementForLine(std::vector<int32_t> inpArr)
{
    // creating and naming log file
    QFile log("../log.txt");
    log.open(QFile::WriteOnly|QFile::Truncate);
    QTextStream logStream(&log);
    logStream << "************\n";
    logStream << "* LOG FILE *\n";
    logStream << "************\n";
    log.close();

    writeData(inpArr);
    mainSortFunc();
}

void radixSort::resPrint()
{
    if(DEBUG) std::cout << "Result printing, lsb = " << lsb << std::endl;
    QFile log("../log.txt");
    log.open(QIODevice::Append);
    QTextStream logStream(&log);
    logStream << "Results of radix sort:" << endl;
    if(consoleMode == false) emit strToPrintInWindow("Results of radix sort:\n");
    else std::cout << "Results of radix sort:" << std:: endl;
    for (auto i : inpArr)
    {
        logStream << i << endl;
        if(consoleMode == false) emit strToPrintInWindow(QString::number(i) + "\n");
        else std::cout << i << std:: endl;
    }
}

void radixSort::callWorkSortFunc()
{
    mainSortFunc();
}

radixSort::~radixSort()
{
}
