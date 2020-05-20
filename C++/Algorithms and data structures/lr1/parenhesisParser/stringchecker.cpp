#include "basicheaders.h"
#include "universalqtheader.h"

// Parentheses
bool Brackets(std::string str, int & pos, QTextStream & log);
// Square brackets
bool Square(std::string str, int & pos, QTextStream & log);
// Round brackets
bool Round(std::string str, int & pos, QTextStream & log);
// Go to the next line character
void step(int & pos, QTextStream & log);
// errors
void Error(const std::string & mistake, std::string str, \
          int pos, QTextStream & log);

int checkString(std::string inputStr, QTextStream & log) {
    int pos = 0;
    std::string successStr = inputStr;
    if (!Brackets(inputStr, pos, log)) {
        successStr = "The result is true: " + successStr + \
                " - correct\n";
        std::cout << successStr;
        log << QString::fromStdString(successStr);
        QMessageBox::information(0, "Success", \
                                 QString::fromStdString(successStr));
    }
    pos = 0;

    return 0;
}

bool Brackets(std::string str, int & pos, QTextStream & log) {
    log << "Brackets func!\n";
    if ((str[pos]=='[') || (str[pos]=='B')) {
        if (Square(str, pos, log))  // is sequence in square paired
            return 1;
        else
            return 0;
    } else if ((str[pos]=='(') || (str[pos]=='A')) {
        if (Round(str, pos, log)) // is sequence in round paired
            return 1;
        else
            return 0;
    }

    Error("Brackets", str, pos, log); // Error processing

    return 1;
}

bool Square(std::string str, int & pos, QTextStream & log) {
    log << "Square func!\n";
    if (str[pos] == '[' && str[pos + 1] == '[') {
        step(pos, log); // go to the next symbol in str
        step(pos, log);
        if (Square(str, pos, log)) // check for paired round #1
            return 1;
        if (str[pos] == ']' && str[pos + 1] == '(') {
            step(pos, log);
            step(pos, log);
            if (Round(str, pos, log))
                return 1;
            if (str[pos] == ')' && str[pos + 1] == ']') {
                step(pos, log);
                step(pos, log);
                return 0;
            }
        } else {
            Error("square parantheses", str, pos, log);
            return 1;
        }
    }

    if (str[pos] == 'B') {
        step(pos, log);
        return 0;
    }

    Error("square parantheses", str, pos, log);

    return 1;
}

bool Round(std::string str, int & pos, QTextStream & log) {
    log << "Round func!\n";
    if (str[pos] == '(' && str[pos + 1] == '(') {
        step(pos, log); // go to the next symbol in str
        step(pos, log);
        if (Round(str, pos, log)) // check for paired round #1
            return 1;
        if (str[pos] == ')' && str[pos + 1] == '[') {
            step(pos, log);
            step(pos, log);
            if (Square(str, pos, log))
                return 1;
            if (str[pos] == ']' && str[pos + 1] == ')') {
                step(pos, log);
                step(pos, log);
                return 0;
            }
        } else {
            Error("round parantheses", str, pos, log);
            return 1;
        }
    }

    if (str[pos] == 'A') {
        step(pos, log);
        return 0;
    }

    Error("round parantheses", str, pos, log);

    return 1;
}

void step(int & pos, QTextStream & log) {
    log << "step func!\n";
    pos++;
}

void Error(const std::string & mistake, std::string str, \
          int pos, QTextStream & log) {
    log << "Error func!\n";
    std::string resStr = "String is incorrect! Mistake is in: \n" + \
            mistake + ". Successful part:\n" + "\"" + \
                str.substr(0, pos) + "\" \n" "Mistake on " + std::to_string(pos + 1) + " position.";
    std::cout << resStr;
    log << QString::fromStdString(resStr);
    QMessageBox::information(0, "Mistake", QString::fromStdString(resStr));
}
