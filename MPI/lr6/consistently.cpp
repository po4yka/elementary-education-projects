#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

#define SIZE 1300

size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

std::vector<std::vector<int>> read_matrix(std::string name)
{
    std::ifstream matrix_stream;
    matrix_stream.open("./" + name + "_" + std::to_string(SIZE) + ".txt" /*,ios_base::app*/);

    std::vector<std::vector<int>> matrix(SIZE, std::vector<int>(SIZE, 0));
    
    std::string line;
    int i = 0;
    while (!matrix_stream.eof())
    {
        getline(matrix_stream, line);
        if (line.length() == 0) {
            break;
        }
        std::vector<std::string> numbers;
        split(line, numbers, ' ');
        for (int j = 0; j < SIZE; ++j)
        {
            int numb = std::stoi(numbers[j]);
            matrix[i][j] = std::stoi(numbers[j]);
        }
        i++;
    }

    matrix_stream.close();

    return matrix;
}

int main()
{
    std::vector<std::vector<int>> A, B;
    std::vector<std::vector<int>> C(SIZE, std::vector<int>(SIZE, 0));

    auto start = std::chrono::high_resolution_clock::now();

    A = read_matrix("A");
    B = read_matrix("B");

    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            C[i][j] = 0;
            for (int k = 0; k < SIZE; ++k)
                C[i][j] += A[i][k] * A[k][j];
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Time: " + std::to_string(duration.count() * 1e-6) + "s" << std::endl;

    return 0;
}