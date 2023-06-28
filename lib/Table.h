#pragma once

#include "PolishNotation.h"
#include <algorithm>

bool GetBool(const std::string& bool_str);

bool CountType(bool& correct_request, const std::string& operation, const std::string& tmp1_word,
               const std::string& tmp2_word, types type1, types type2);

class Table {
public:
    Table() = default;

    Table(const std::string& name, const std::unordered_map<std::string, ColumnDisription>& head) : name_(name),
                                                                                                    head_(head) {};

    Row& operator[](size_t index) {
        return rows_[index];
    }

    void Update(bool& correct_request, const std::vector<std::pair<std::string, Cell>>& pairs,
                const std::vector<Row*>& rows);

    std::vector<std::string> GetColumnsNames();

    ColumnDisription GetColumnDisr(const std::string& column_name) {
        return head_[column_name];
    }

    types GetTypeColumn(const std::string& column_name) {
        return head_[column_name].value_type;
    }

    void AddRow(const Row& row) {
        rows_.emplace_back(row);
    }

    std::vector<Row> ReturnRow(bool& correct_request, const std::string& input_condition);


    std::vector<Row*> ReturnRowPonter(bool& correct_request, const std::string& input_condition);


    void DeleteRow(std::vector<Row>& rows);

    std::vector<Row> GetRows() {
        return rows_;
    }

    std::vector<Row> GetSpecialColumn(join type, bool flag_left_table, const std::vector<std::string>& columns);

    void View();

    bool ContainColumn(const std::string& column) const {
        return head_.contains(column);
    }

private:

    static std::pair<std::string, types> CountArithmeticOperation(bool& correct_request, Row& row,
                                                                  const std::string& operation,
                                                                  const std::string& tmp1_word,
                                                                  const std::string& tmp2_word,
                                                                  types type1, types type2);

    static bool CountStandardOperation(bool& correct_request, Row& row,
                                       const std::string& operation,
                                       const std::string& tmp1_word,
                                       const std::string& tmp2_word,
                                       types type1, types type2);

    static bool
    CheckCondition(bool& correct_request, Row& row, const std::vector<std::pair<std::string, types>>& rpn_form);

    std::unordered_map<std::string, ColumnDisription> head_;
    std::vector<Row> rows_;
    std::string name_;
};
