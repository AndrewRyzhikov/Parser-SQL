#pragma once

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>

enum join {
    LEFT, RIGHT, INNER
};

join GetJoinType(const std::string& value);

enum types {
    LOGICAL_OPERATOR, STANDARD_OPERATOR, ARITHMETIC_OPERATOR, GROUP, COLUMN, BOOL, INT, VARCHAR, REAL, WASTE
};

types GetTypeCreate(const std::string& type);

types GetType(const std::string& value);

