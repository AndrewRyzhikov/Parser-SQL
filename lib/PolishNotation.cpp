#include "PolishNotation.h"

std::string PolishNotation::GetNext() {
    std::string str;
    while (true) {
        char tmp_char = GetChar();
        if (tmp_char == ' ') {
            IncChar();
            continue;
        }

        if (tmp_char == '(' or tmp_char == ')' or tmp_char == '+' or tmp_char == '-' or tmp_char == '*' or
            tmp_char == '\\' or tmp_char == '%' or tmp_char == '=') {
            str += tmp_char;
            IncChar();
            return str;
        } else if (isdigit(tmp_char)) {
            str += GetNum();
            return str;
        } else if (tmp_char == '<' or tmp_char == '>') {
            IncChar();
            str = tmp_char + GetSpace();
            return str;
        } else if (isalpha(tmp_char)) {
            str = GetWord();
            return str;
        } else if (tmp_char == '\"') {
            IncChar();
            str = tmp_char + GetString();
            return str;
        }

        str = tmp_char;
        IncChar();
        return str;
    }
}

std::string PolishNotation::GetString() {
    char tmp_char = GetChar();
    if (tmp_char != '\"') {
        IncChar();
        return tmp_char + GetString();
    }

    IncChar();
    return "\"";
}

std::string PolishNotation::GetWord() {
    char tmp_char = GetChar();
    if (isalpha(GetChar())) {
        IncChar();
        return tmp_char + GetWord();
    }
    return "";
}

std::string PolishNotation::GetSpace() {
    char tmp_char = GetChar();
    if (tmp_char == '>' or tmp_char == '=') {
        IncChar();
        return tmp_char + GetSpace();
    }
    return "";
}

std::string PolishNotation::GetNum() {
    char tmp_char = GetChar();
    if (isdigit(GetChar())) {
        IncChar();
        return tmp_char + GetNum();
    }
    return "";
}

std::vector<std::string> PolishNotation::GetVectorCommand() {
    std::vector<std::string> commands;
    while (index_ < input_.size()) {
        std::string next = GetNext();
        if (next.empty()) {
            continue;
        }
        commands.push_back(next);
    }
    return commands;
}

std::vector<std::pair<std::string, types>> PolishNotation::operator()(bool& correct_request) {
    std::vector<std::string> infix_form = GetVectorCommand();
    std::vector<std::pair<std::string, types>> rpn_form;
    std::stack<std::string> commands;

    for (const auto& tmp_command: infix_form) {
        types type = GetType(tmp_command);
        if (type == LOGICAL_OPERATOR or type == STANDARD_OPERATOR or type == ARITHMETIC_OPERATOR) {
            while (!commands.empty() and commands.top() != "(" and priority[commands.top()] >= priority[tmp_command]) {
                rpn_form.emplace_back(commands.top(), GetType(commands.top()));
                commands.pop();
            }
            commands.emplace(tmp_command);
        } else if (type == COLUMN or type == BOOL or type == INT or type == REAL or type == VARCHAR) {
            rpn_form.emplace_back(tmp_command, type);
        } else if (type == GROUP) {
            if (tmp_command == "(") {
                commands.emplace(tmp_command);
            } else {
                while (commands.top() != "(") {
                    rpn_form.emplace_back(commands.top(), GetType(commands.top()));
                    commands.pop();
                    if (commands.empty()) {
                        correct_request = false;
                        std::cout << "Error with group\n";
                        return {};
                    }
                }
                commands.pop();
            }
        } else {
            correct_request = false;
            std::cout << "Error syntax\n";
            return {};
        }
    }

    while (!commands.empty()) {
        rpn_form.emplace_back(commands.top(), GetType(commands.top()));
        commands.pop();
    }

    return rpn_form;
}
