#include "Table.h"

std::vector<std::string> Table::GetColumnsNames() {
    std::vector<std::string> columns_names;
    columns_names.reserve(head_.size());
    for (auto [column_name, discription]: head_) {
        columns_names.emplace_back(column_name);
    }
    std::reverse(columns_names.begin(), columns_names.end());

    return columns_names;
}

std::pair<std::string, types> Table::CountArithmeticOperation(bool& correct_request, Row& row,
                                                              const std::string& operation,
                                                              const std::string& tmp1_word,
                                                              const std::string& tmp2_word,
                                                              types type1, types type2) {
    std::string result;
    if (type1 == INT and type2 == INT) {
        if (operation == "-") {
            result = std::to_string(std::stoi(tmp1_word) - std::stoi(tmp2_word));
        } else if (operation == "/") {
            result = std::to_string(std::stoi(tmp1_word) / std::stoi(tmp2_word));
        } else if (operation == "*") {
            result = std::to_string(std::stoi(tmp1_word) * std::stoi(tmp2_word));
        } else if (operation == "+") {
            result = std::to_string(std::stoi(tmp1_word) + std::stoi(tmp2_word));
        } else {
            result = std::to_string(std::stoi(tmp1_word) % std::stoi(tmp2_word));
        }
        return {result, INT};
    } else if (type1 == VARCHAR and type2 == VARCHAR and operation == "+") {
        result = tmp1_word + tmp2_word;
        return {result, VARCHAR};
    } else if (type1 == COLUMN or type2 == COLUMN) {
        types tmp1_value_type = row.GetCell(tmp1_word).type, tmp2_value_type = row.GetCell(tmp2_word).type;
        std::string tmp1_value = row.GetCell(tmp1_word).value, tmp2_value = row.GetCell(tmp2_word).value;

        if (type1 == COLUMN and type2 != COLUMN) {
            tmp2_value_type = type2;
            tmp2_value = tmp2_word;
        } else if (type2 == COLUMN and type1 != COLUMN) {
            tmp1_value_type = type1;
            tmp1_value = tmp1_word;
        }

        if (tmp1_value_type == tmp2_value_type) {
            if (tmp1_value_type == INT) {
                if (operation == "-") {
                    result = std::to_string(std::stoi(tmp1_value) - std::stoi(tmp2_value));
                } else if (operation == "/") {
                    result = std::to_string(std::stoi(tmp1_value) / std::stoi(tmp2_value));
                } else if (operation == "*") {
                    result = std::to_string(std::stoi(tmp1_value) * std::stoi(tmp2_value));
                } else {
                    result = std::to_string(std::stoi(tmp1_value) + std::stoi(tmp2_value));
                }
                return {result, INT};
            } else if ((tmp1_value_type == REAL and (tmp2_value_type == INT or tmp2_value_type == REAL)) or
                       (tmp2_value_type == REAL and (tmp1_value_type == INT or tmp1_value_type == REAL))) {
                if (operation == "-") {
                    result = std::to_string(std::stod(tmp1_value) - std::stod(tmp2_value));
                } else if (operation == "/") {
                    result = std::to_string(std::stod(tmp1_value) / std::stod(tmp2_value));
                } else if (operation == "*") {
                    result = std::to_string(std::stod(tmp1_value) * std::stod(tmp2_value));
                } else {
                    result = std::to_string(std::stod(tmp1_value) + std::stod(tmp2_value));
                }
                return {result, REAL};
            } else if (tmp1_value_type == VARCHAR and operation == "+") {
                result = tmp1_value + tmp2_value;
                return {result, VARCHAR};
            } else {
                correct_request = false;
                std::cout << "Operator" << "\"" << operation << "\"" << "does not support operation with this type\n";
                return {};
            }
        }
        correct_request = false;
        std::cout << "Different type to operator" << "\"" << operation << "\"\n";
        return {};
    } else if ((type1 == REAL or type2 == INT) and (type2 == REAL or type2 == INT)) {
        if (operation == "-") {
            result = std::to_string(std::stod(tmp1_word) - std::stod(tmp2_word));
        } else if (operation == "/") {
            result = std::to_string(std::stod(tmp1_word) / std::stod(tmp2_word));
        } else if (operation == "*") {
            result = std::to_string(std::stod(tmp1_word) * std::stod(tmp2_word));
        } else {
            result = std::to_string(std::stod(tmp1_word) + std::stod(tmp2_word));
        }
        return {result, REAL};
    }
}

bool CountType(bool& correct_request, const std::string& operation, const std::string& tmp1_word,
               const std::string& tmp2_word, types type1, types type2) {
    if (type1 == INT) {
        if (operation == "<") {
            return std::stoi(tmp1_word) < std::stoi(tmp2_word);
        } else if (operation == "<=") {
            return std::stoi(tmp1_word) <= std::stoi(tmp2_word);
        } else if (operation == ">") {
            return std::stoi(tmp1_word) > std::stoi(tmp2_word);
        } else if (operation == ">=") {
            return std::stoi(tmp1_word) >= std::stoi(tmp2_word);
        } else if (operation == "=") {
            return std::stoi(tmp1_word) == std::stoi(tmp2_word);
        } else if (operation == "<>") {
            return std::stoi(tmp1_word) != std::stoi(tmp2_word);
        }
    } else if (type1 == BOOL) {
        bool tmp1 = GetBool(tmp1_word), tmp2 = GetBool(tmp2_word);

        if (operation == "=") {
            return tmp1 == tmp2;
        } else if (operation == "<>") {
            return tmp1 != tmp2;
        }

        correct_request = false;
        std::cout << "Error with operation with bool values\n";
        return false;
    } else if (type1 == VARCHAR) {
        if (operation == "<") {
            return tmp1_word < tmp2_word;
        } else if (operation == "<=") {
            return tmp1_word <= tmp2_word;
        } else if (operation == ">") {
            return tmp1_word > tmp2_word;
        } else if (operation == ">=") {
            return tmp1_word >= tmp2_word;
        } else if (operation == "=") {
            return tmp1_word == tmp2_word;
        } else if (operation == "<>") {
            return tmp1_word != tmp2_word;
        }
    } else if (type1 == REAL) {
        if (operation == "<") {
            return std::stod(tmp1_word) < std::stod(tmp2_word);
        } else if (operation == "<=") {
            return std::stod(tmp1_word) <= std::stod(tmp2_word);
        } else if (operation == ">") {
            return std::stod(tmp1_word) > std::stod(tmp2_word);
        } else if (operation == ">=") {
            return std::stod(tmp1_word) >= std::stod(tmp2_word);
        } else if (operation == "=") {
            return std::stod(tmp1_word) == std::stod(tmp2_word);
        } else if (operation == "<>") {
            return std::stod(tmp1_word) != std::stod(tmp2_word);
        }
    }
    correct_request = false;
    std::cout << "Different type values\n";
    return false;
}

bool GetBool(const std::string& bool_str) {
    return bool_str == "true";
}

bool Table::CountStandardOperation(bool& correct_request, Row& row, const std::string& operation,
                                   const std::string& tmp1_word,
                                   const std::string& tmp2_word,
                                   types type1, types type2) {
    if (type1 == type2) {
        if (type1 == INT or type1 == REAL or type1 == VARCHAR or type1 == BOOL) {
            return CountType(correct_request, operation, tmp1_word, tmp2_word, type1, type2);
        } else if (type1 == COLUMN) {
            types tmp1_value_type = row.GetCell(tmp1_word).type, tmp2_value_type = row.GetCell(tmp2_word).type;
            std::string tmp1_value = row.GetCell(tmp1_word).value, tmp2_value = row.GetCell(tmp2_word).value;

            if (tmp1_value != "NULL" and tmp2_value != "NULL") {
                return CountType(correct_request, operation, tmp1_word, tmp2_word, tmp1_value_type, tmp2_value_type);
            }

            return false;
        }
    } else if (type1 == COLUMN or type2 == COLUMN) {
        types tmp1_value_type = row.GetCell(tmp1_word).type, tmp2_value_type = row.GetCell(tmp2_word).type;
        std::string tmp1_value = row.GetCell(tmp1_word).value, tmp2_value = row.GetCell(tmp2_word).value;

        if (type1 == COLUMN and type2 != COLUMN) {
            tmp2_value_type = type2;
            tmp2_value = tmp2_word;
        } else if (type2 == COLUMN and type1 != COLUMN) {
            tmp1_value_type = type1;
            tmp1_value = tmp1_word;
        }

        if (tmp1_value != "NULL" and tmp2_value != "NULL") {
            if (tmp1_value_type == tmp2_value_type) {
                return CountType(correct_request, operation, tmp1_value, tmp2_value, tmp1_value_type, tmp2_value_type);
            }

            correct_request = false;
            std::cout << "Different type values\n";
            return false;
        }
        return false;
    }
}

bool
Table::CheckCondition(bool& correct_request, Row& row, const std::vector<std::pair<std::string, types>>& rpn_form) {
    std::stack<std::pair<std::string, types>> operands;
    std::stack<bool> bools;

    for (const auto& [tmp_word, type]: rpn_form) {
        if (type == COLUMN or type == BOOL or type == INT or type == REAL or type == VARCHAR) {
            operands.emplace(tmp_word, type);
        } else if (type == LOGICAL_OPERATOR) {
            if (tmp_word == "NOT") {
                if (bools.empty()) {
                    correct_request = false;
                    std::cout << "Condition is not correct with operator \"NOT\"\n";
                    return false;
                }

                bool tmp_top = bools.top();
                bools.pop();
                bools.emplace(!tmp_top);
            } else if (tmp_word == "AND" or tmp_word == "OR") {
                if (bools.size() < 2) {
                    correct_request = false;
                    std::cout << "Condition is not correct with operators \"AND\" or \"OR\"\n";
                    return false;
                }

                bool tmp_top2 = bools.top();
                bools.pop();

                bool tmp_top1 = bools.top();
                bools.pop();

                if (tmp_word == "OR") {
                    bools.emplace(tmp_top1 or tmp_top2);
                } else {
                    bools.emplace(tmp_top1 and tmp_top2);
                }
            }
        } else if (type == ARITHMETIC_OPERATOR) {
            if (operands.size() < 2) {
                correct_request = false;
                std::cout << "Condition is not correct with STANDARD_OPERATOR\n";
                return false;
            }
            auto [tmp2_word, type2] = operands.top();
            operands.pop();

            auto [tmp1_word, type1] = operands.top();
            operands.pop();

            auto [new_word, type_word] = CountArithmeticOperation(correct_request, row, tmp_word, tmp1_word, tmp2_word,
                                                                  type1, type2);
            operands.emplace(new_word, type_word);

        } else if (type == STANDARD_OPERATOR) {

            if (operands.size() < 2) {
                correct_request = false;
                std::cout << "Condition is not correct with STANDARD_OPERATOR\n";
                return false;
            }

            auto [tmp2_word, type2] = operands.top();
            operands.pop();

            auto [tmp1_word, type1] = operands.top();
            operands.pop();

            bool result = CountStandardOperation(correct_request, row, tmp_word, tmp1_word, tmp2_word,
                                                 type1, type2);
            bools.push(result);
        }
    }

    return bools.top();
}

std::vector<Row> Table::ReturnRow(bool& correct_request, const std::string& input_condition) {
    PolishNotation nation(input_condition);
    std::vector<std::pair<std::string, types>> rpn_form = nation(correct_request);

    if (rpn_form.empty()) {
        correct_request = false;
        return {};
    }

    std::vector<Row> result;

    for (auto& row: rows_) {
        bool condition = CheckCondition(correct_request, row, rpn_form);
        if (correct_request) {
            if (condition) {
                result.emplace_back(row);
            }
        } else {
            return {};
        }
    }

    return result;
}

std::vector<Row*> Table::ReturnRowPonter(bool& correct_request, const std::string& input_condition) {
    PolishNotation nation(input_condition);
    std::vector<std::pair<std::string, types>> rpn_form = nation(correct_request);

    if (rpn_form.empty()) {
        correct_request = false;
        return {};
    }

    std::vector<Row*> result;

    for (auto& row: rows_) {
        bool condition = CheckCondition(correct_request, row, rpn_form);
        if (correct_request) {
            if (condition) {
                result.emplace_back(&row);
            }
        } else {
            return {};
        }
    }

    return result;
}

void Table::DeleteRow(std::vector<Row>& rows) {
    for (auto& row: rows) {
        for (auto it = rows_.begin(); it != rows_.end(); ++it) {
            if (*it == row) {
                rows_.erase(it);
                break;
            }
        }
    }
}

void Table::Update(bool& correct_request, const std::vector<std::pair<std::string, Cell>>& pairs,
                   const std::vector<Row*>& rows) {
    for (const auto& row: rows) {
        for (auto [name, cell]: pairs) {
            if ((*row)[name].type != cell.type) {
                correct_request = false;
                std::cout << "Column " << name << " and " << cell.value << " have different type";
                return;
            }
            (*row)[name] = cell;
        }
    }
}

void Table::View() {
    for (auto [name, row]: head_) {
        std::cout << name << ' ';
    }
    std::cout << '\n';
    for (auto row: rows_) {
        row.View();
    }
}


std::vector<Row> Table::GetSpecialColumn(join type, bool flag_left_table, const std::vector<std::string>& columns) {
    std::vector<Row> new_columns;
    if (type == INNER) {
        for (auto row: columns) {

        }
    } else if (type == LEFT) {

    } else {

    }
}
