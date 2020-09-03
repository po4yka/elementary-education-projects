#include <iostream>
#include <string>
#include <vector>
#include <memory>

class KmpAlg {
    std::string textString;
    std::string word;

    std::unique_ptr<std::size_t[]> partialMatchTable;
    std::vector<std::size_t> matching;
    bool finishedMatching{};

    // generate π
    void generatePartialMatchTable() {
        partialMatchTable = std::make_unique<std::size_t[]>(word.length());
        std::size_t const wordLength = word.length();
        std::size_t j = 0;

        for(std::size_t i = 1; i < wordLength; i++) {
            while(j && word[i] != word[j]) {
                j = partialMatchTable[j - 1];
            }
            if(word[i] == word[j]) {
                partialMatchTable[i] = ++j;
            }
        }
    }

    void kmp() {
        if(finishedMatching) return;

        std::size_t const textLength = textString.length();
        std::size_t const wordLength = word.length();
        std::size_t j = 0;

        for(std::size_t i = 0; i < textLength; ++i) {
            while(j && textString[i] != word[j]) {
                j = partialMatchTable[j - 1];
            }
            if(textString[i] == word[j]) {
                if(j == wordLength - 1) {
                    matching.push_back(i - wordLength + 1);
                    j = partialMatchTable[j];
                    return;
                }
                else j++;
            }
        }
        finishedMatching = true;
    }

public:
    KmpAlg() = default;
    ~KmpAlg() = default;
    explicit KmpAlg(std::string textString) : textString(std::move(textString)), finishedMatching(false) {}
    KmpAlg(std::string textString, std::string word) : textString(std::move(textString)), word(std::move(word)), finishedMatching(false) {
        generatePartialMatchTable();
    }

    [[nodiscard]] std::string getText() const { return textString; }
    [[nodiscard]] std::string getWord() const { return word; }

    void setText(std::string txt) {
        textString = std::move(txt);
        finishedMatching = false;
    }

    void setWord(std::string wrd) {
        word = std::move(wrd);
        generatePartialMatchTable();
        finishedMatching = false;
    }

    [[nodiscard]] std::vector<std::size_t> const &getMatching() {
        kmp();
        return matching;
    }
};

int main() {
    std::string str1, str2;
    std::cin >> str1 >> str2;

    str1.append(str1);

    KmpAlg kmp(str1, str2);
    auto const &matching = kmp.getMatching();

    if(matching.empty()) {
        std::cout << "-1" << std:: endl;
    }
    else {
        std::cout << matching[0] << std::endl;
    }

    return 0;
}