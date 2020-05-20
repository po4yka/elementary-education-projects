#ifndef TEST_H
#define TEST_H

#include "allheaders.h"
#include "customvector.h"
#include "HashTable.h"

class Test {

    public:
        static void makeHashTable(lrstruct::Vector<std::string>, lrstruct::HashTable<std::string>*);
        static void findElement(lrstruct::HashTable<std::string>*, std::string);
};

#endif // TEST_H
