#include <iostream>
#include <vector>
#include <string>

class SquareDivision
{
private:
    size_t size{};
    std::vector<std::vector<size_t>> squareDivArr;
    std::vector<std::vector<size_t>> optimalSquareDivArr;
    size_t currDivCount{};
    size_t optimalDivCount{};
    size_t compression{};

public:
    explicit SquareDivision(size_t &size_) : size(size_), optimalDivCount(size * size)
    {
        // speed improve
        squareDivArr.resize(size);

        for (auto i = size_; i > 0; --i)
        {
            if (size % i == 0 && size != i)
            {
                size /= i;
                compression = i;
                break;
            }
        }

        for (auto &i : squareDivArr)
        {
            i.resize(size);
            for (auto j = 0; j < size; ++j)
                i.push_back(0);
        }
        optimalSquareDivArr = squareDivArr;
    }

    void createOptimalDivision()
    {
        prepareDivSetup();

        updateOptimalDivision(size / 2, size / 2 + 1);
        printOptimalDivString();
    }

private:

    static bool checkForSimpleSolve(size_t n)
    {
        return n % 2 == 0 || n % 3 == 0 || n % 5 == 0;
    }

    void insertDivision(size_t x, size_t y, size_t size_)
    {
        for (auto i = x; i < x + size_; ++i)
            for (auto j = y; j < y + size_; ++j)
            {
                squareDivArr[i][j] = size_;
            }
        currDivCount++;
    }

    void removeDivision(size_t x, size_t y, size_t size_, bool optimal)
    {
        for (auto i = x; i < x + size_; ++i)
            for (auto j = y; j < y + size_; ++j)
            {
                if (optimal)
                    optimalSquareDivArr[i][j] = 0;
                else
                    squareDivArr[i][j] = 0;
            }
        if (!optimal)
            currDivCount--;
    }

    bool checkDivision(size_t x, size_t y, size_t size_) {
        if (x + size_ > size || y + size_ > size)
            return false;

        for (auto i = x; i < x + size_; i++)
        {
            for (auto j = y; j < y + size_; j++)
            {
                if (squareDivArr[i][j] != 0)
                    return false;
            }
        }

        return true;
    }

    bool findEmpty(size_t &x, size_t &y) {
        while (squareDivArr[x][y] != 0) {
            if (y == size - 1)
            {
                if (x == size - 1)
                {
                    return false;
                }
                else
                {
                    x++;
                    y = size / 2;
                    continue;
                }
            }
            y++;
        }
        return true;
    }

    void prepareDivSetup()
    {
        size_t halfSize = size / 2;
        if (size % 2 == 0)
        {
            insertDivision(0, 0, halfSize);
            insertDivision(0, halfSize, halfSize);
            insertDivision(halfSize, 0, halfSize);
            insertDivision(halfSize, halfSize, halfSize);
            currDivCount = 4;
            optimalDivCount = currDivCount;
            optimalSquareDivArr = squareDivArr;
        }
        else
        {
            insertDivision(0, 0, halfSize + 1);
            insertDivision(0, halfSize + 1, halfSize);
            insertDivision(halfSize + 1, 0, halfSize);
        }
    }

    void updateOptimalDivision(size_t x, size_t y)
    {
        if (currDivCount >= optimalDivCount)
            return;
        for (auto n = size / 2; n > 0; --n)
        {
            if (checkDivision(x, y, n))
            {
                insertDivision(x, y, n);
                size_t copyX = x;
                size_t copyY = y;
                if (findEmpty(copyX, copyY))
                {
                    updateOptimalDivision(copyX, copyY);
                }
                else
                {
                    if (currDivCount < optimalDivCount)
                    {
                        optimalSquareDivArr = squareDivArr;
                        optimalDivCount = currDivCount;
                    }
                    removeDivision(x, y, n, false);
                    return;
                }
                removeDivision(x, y, n, false);
            }
        }
    }

    void printOptimalDivString()
    {
        std::cout<< std::to_string(optimalDivCount) << std::endl;

        for (unsigned i = 0; i < size; ++i)
        {
            for (unsigned j = 0; j < size; ++j)
            {
                if (optimalSquareDivArr[i][j] != 0)
                {
                    std::cout << std::to_string(i * compression + 1) + " " +
                                 std::to_string(j * compression + 1) + " ";
                    std:: cout << std::to_string(optimalSquareDivArr[i][j] * compression) << std::endl;
                    removeDivision(i, j, optimalSquareDivArr[i][j], true);
                }
            }
        }
    }
};

int main()
{
    size_t size = 0;
    std::cin >> size;

    auto *cutter = new SquareDivision(size);
    cutter->createOptimalDivision();
    delete cutter;

    return 0;
}