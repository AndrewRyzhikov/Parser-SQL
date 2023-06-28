#include "storage.h"

types GetTypeCreate(const std::string& type) {
    if (type == "int") {
        return INT;
    } else if (type == "double" or type == "float") {
        return REAL;
    } else if (type == "bool") {
        return BOOL;
    } else {
        return VARCHAR;
    }
}

types GetType(const std::string& value) {
    std::regex int_regex(R"(^[+-]?\d+$)");
    std::regex bool_regex(R"(^(false|true)$)");
    std::regex real_regex(R"(^[+-]?\d+\.\d+$)");
    std::regex varchar_regex(R"(^".+"$)");
    std::regex column_regex(R"(^\w+$)");

    if (regex_match(value, int_regex)) {
        return INT;
    } else if (regex_match(value, bool_regex)) {
        return BOOL;
    } else if (regex_match(value, real_regex)) {
        return REAL;
    } else if (value == "<=" or value == "<" or value == ">" or value == ">=" or value == "=" or value == "<>") {
        return STANDARD_OPERATOR;
    } else if (value == "+" or value == "-" or value == "*" or value == "/") {
        return ARITHMETIC_OPERATOR;
    } else if (value == "AND" or value == "NOT" or value == "OR") {
        return LOGICAL_OPERATOR;
    } else if (value == "(" or value == ")") {
        return GROUP;
    } else if (regex_match(value, varchar_regex)) {
        return VARCHAR;
    } else if (regex_match(value, column_regex)) {
        return COLUMN;
    }
    return WASTE;
}

join GetJoinType(const std::string& value) {
    if (value == "INNER" or value.empty()) {
        return INNER;
    } else if (value == "LEFT") {
        return LEFT;
    }
    return RIGHT;
}
