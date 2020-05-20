#include "HashTable.h"

void makeLogMessage(std::string message) {
    std::fstream fs;
    fs.open (LOG_FILE_WAY, std::fstream::in | std::fstream::out | std::fstream::app);
    fs << message << "\n";
    fs.close();
}

void makeLogMessage(std::string message, std::string el) {
    std::fstream fs;
    fs.open (LOG_FILE_WAY, std::fstream::in | std::fstream::out | std::fstream::app);
    fs << message << el << "\n";
    fs.close();
}

void makeLogMessage(std::string message, size_t maxSize, size_t cSize) {
    std::fstream fs;
    fs.open (LOG_FILE_WAY, std::fstream::in | std::fstream::out | std::fstream::app);
    fs << "\n" << message << "\n";
    fs << "New max size of hash is: " << maxSize << ", current size is: " << cSize << "\n\n";
    fs.close();
}
