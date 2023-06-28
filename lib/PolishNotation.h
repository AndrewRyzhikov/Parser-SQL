#pragma once

#include "storage.h"
#include "Row.h"
#include <cctype>
#include <regex>
#include <stack>

static std::map<std::string, uint8_t> priority = {{"OR",  0},
                                                  {"AND", 1},
                                                  {"NOT", 2},
                                                  {"<",   3},
                                                  {"<=",  3},
                                                  {">",   3},
                                                  {">=",  3},
                                                  {"=",   3},
                                                  {"<>",  3},
                                                  {"+",   4},
                                                  {"-",   4},
                                                  {"*",   5},
                                                  {"/",   5},
                                                  {"%",   5},
                                                  {"(",   10}};

class PolishNotation {
public:
    PolishNotation(const std::string& input) : input_(input), index_(0) {};

    std::vector<std::pair<std::string, types>> operator()(bool& correct_request);

private:
    char GetChar() {
        return input_[index_];
    }

    void IncChar() {
        ++index_;
    }

    std::string GetNum();

    std::string GetWord();

    std::string GetSpace();

//    std::string GetCompare();

    std::string GetString();

    std::string GetNext();

    std::vector<std::string> GetVectorCommand();

    size_t index_;
    std::string input_;
};
