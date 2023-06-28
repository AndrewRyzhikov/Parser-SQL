#include "DataBase.h"

std::vector<std::string> split(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}

std::pair<std::string, std::string> pair_split(const std::string& input, char delimiter) {
    std::vector<std::string> pair = split(input, delimiter);
    return {pair[0], pair[1]};
}

void Data::ParseCreateTable(const std::smatch& info_table) {
    std::vector<std::string> split_columns = split(info_table[2], ',');
    std::regex column_regex(R"(^\s*(\w+)\s*(int|bool|double|varchar|float)(\s+|$)(NOT\s+NULL)?\s*$)");
    std::smatch info;

    std::unordered_map<std::string, ColumnDisription> head;

    for (const auto& str: split_columns) {
        if (std::regex_match(str, info, column_regex)) {
            std::string name_column = info[1];
            types type_column = GetTypeCreate(info[2]);
            bool null = info[4].str().empty();
            head[name_column] = ColumnDisription(type_column, null);
        } else {
            std::cout << "Incorrect create request\n";
            correct_request_ = false;
            return;
        }
    }

    std::string table_name = info_table[1];
    tables_[table_name] = Table(table_name, head);
}

std::vector<std::string> Data::ParseNames(const std::string& str_columns) {
    std::vector<std::string> names;
    std::vector<std::string> split_columns = split(str_columns, ',');
    std::regex column_regex(R"(^\s*(\"?(.+)\"?)\s*)");
    std::smatch info;

    for (const auto& str: split_columns) {
        if (std::regex_match(str, info, column_regex)) {
            std::string name_column = info[1];
            names.emplace_back(name_column);
        } else {
            correct_request_ = false;
            std::cout << "Incorrect name\n";
            return {};
        }
    }

    return names;
}

void Data::MakeBijection(const std::string& table_name, const std::vector<std::string>& columns,
                         const std::vector<std::string>& values) {
    if (columns.size() != values.size()) {
        std::cout << "Different number columns and values\n";
        correct_request_ = false;
        return;
    }

    Table& table = tables_[table_name];
    Row row;

    for (const auto& column_name_from_table: table.GetColumnsNames()) {
        bool insert = false;
        for (size_t i = 0; i < columns.size(); ++i) {
            if (columns[i] == column_name_from_table) {
                insert = true;
                types type_column = table.GetTypeColumn(columns[i]);
                types type_value = GetType(values[i]);

                if (type_value == type_column and table.GetColumnDisr(columns[i]).is_create) {
                    row[columns[i]] = Cell(type_column, values[i]);
                } else {
                    correct_request_ = false;
                    if (type_value == WASTE) {
                        std::cout << "Incorrect value type\n";
                    } else if (!table.GetColumnDisr(columns[i]).is_create) {
                        std::cout << "Column " << values[i] << "did not found\n";
                    } else {
                        std::cout << "Different type column and value\n";
                    }
                    return;
                }
            }
        }
        if (!insert) {
            if (table.GetColumnDisr(column_name_from_table).null) {
                row[column_name_from_table] = Cell();
            } else {
                std::cout << "Table have NOT NULL columns\n";
                correct_request_ = false;
                return;
            }
        }
    }
    table.AddRow(row);
}

void Data::ParseInsert(const std::smatch& info_select) {
    std::string table_name = info_select[1];

    std::vector<std::string> columns;
    if (info_select[2].str().empty()) {
        columns = tables_[table_name].GetColumnsNames();
    } else {
        columns = ParseNames(info_select[3]);
    }

    std::vector<std::string> values = ParseNames(info_select[5]);

    if (correct_request_) {
        MakeBijection(table_name, columns, values);
    }
}

void Data::ParseDrop(const std::smatch& info_drop) {
    std::string table_name = info_drop[1];
    if (!tables_.contains(table_name)) {
        correct_request_ = false;
        std::cout << "This table did not found\n";
        return;
    }
    tables_.erase(table_name);
}

void Data::ParseDelete(const std::smatch& info_delete) {

    std::string table_name = info_delete[1];
    std::string condition = info_delete[2];

    if (tables_.find(table_name) != tables_.end()) {
        Table& table = tables_[table_name];

        std::vector<Row> delete_rows = table.ReturnRow(correct_request_, condition);

        table.DeleteRow(delete_rows);
    } else {
        std::cout << "This table did not found\n";
    }
}

void Data::ParseRequest(const std::string& request) {
    correct_request_ = true;
    std::regex regex_create(R"(\s*CREATE\s+TABLE\s+(\w+)\s*\(((.|\n)+)\)\s*;\s*\n*)");
    std::regex regex_insert(R"(\s*INSERT\s+INTO\s+(\w+)\s*(\(((.|\n)+)\)|)\s*VALUES\s*\(((.|\n)+)\);\s*)");
    std::regex regex_drop(R"(\s*DROP\s+TABLE\s+(\w+)\s*;)");
    std::regex regex_delete(R"(\s*DELETE\s+FROM\s+(\w+)\s+WHERE\s+(.+);\s*)");
    std::regex regex_update(R"(\s*UPDATE\s+(\w*)\s+SET\s+(.+)\s+WHERE\s+(.+)\s*;\s*)");
    std::regex regex_select(R"(\s*SELECT\s+(.+)\s+FROM\s+(\w+)\s*(WHERE\s+(.+)\s*)?\s*;\s*)");
    std::regex regex_join(
            R"(\s*SELECT\s+(.+)\s+FROM\s+(\w+)\s*(INNER|LEFT|RIGHT)?\s+JOIN\s+(\w+)\s+ON\s+((\w|\.)+)\s*=\s*((\w|\.)+)\s*;\s*)");
    std::regex regex_view(R"(^\s*VIEW\s+(\w+))");
    std::smatch info;

    if (regex_match(request, info, regex_create)) {
        ParseCreateTable(info);
        if (correct_request_) {
            std::cout << "Table was successful created\n";
        }
    } else if (regex_match(request, info, regex_insert)) {
        ParseInsert(info);
        if (correct_request_) {
            std::cout << "Inserted successfully\n";
        }
    } else if (regex_match(request, info, regex_drop)) {
        ParseDrop(info);
        if (correct_request_) {
            std::cout << info[1].str() << ' ' << "was successfully deleted\n";
        }
    } else if (regex_match(request, info, regex_delete)) {
        ParseDelete(info);
        if (correct_request_) {
            std::cout << "Rows were successfully deleted\n";
        }
    } else if (regex_match(request, info, regex_update)) {
        ParseUpdate(info);
        if (correct_request_) {
            std::cout << "Rows were successfully updated\n";
        }
    } else if (regex_match(request, info, regex_select)) {
        ParseSelect(info);
    } else if (regex_match(request, info, regex_join)) {
        ParseJoin(info);
    } else if (regex_match(request, info, regex_view)) {
        View(info[1]);
    } else {
        std::cout << "Incorrect request\n";
    }
}

void Data::ParseUpdate(const std::smatch& info_update) {
    std::string table_name = info_update[1];
    std::vector<std::pair<std::string, Cell>> pair = ParsePair(info_update[2]);

    if (!tables_.contains(table_name)) {
        correct_request_ = false;
        std::cout << "Table did not found\n";
        return;
    }

    Table& table = tables_[table_name];

    std::vector<Row*> condition_rows = table.ReturnRowPonter(correct_request_, info_update[3]);

    table.Update(correct_request_, pair, condition_rows);
}

std::vector<std::pair<std::string, Cell>> Data::ParsePair(const std::string& pair) {
    std::vector<std::string> split_pair = split(pair, ',');
    std::vector<std::pair<std::string, Cell>> bijection;
    std::regex pattern(R"(\s*(\w+)\s*=\s*(\w+|"\w+"))");
    std::smatch info;

    for (const auto& tmp_str: split_pair) {
        if (std::regex_match(tmp_str, info, pattern)) {
            std::string column_name = info[1];
            std::string value = info[2];
            types value_type = GetType(value);

            if (value_type == WASTE) {
                correct_request_ = false;
                std::cout << "Unknown value type\n";
                return {};
            }

            bijection.emplace_back(column_name, Cell(value_type, value));
        } else {
            correct_request_ = false;
            std::cout << "Syntax error\n";
            return {};
        }
    }

    return bijection;
}

void Data::View(const std::string& table_name) {
    Table table = tables_[table_name];
    table.View();
}

void Data::ParseSelect(const std::smatch& info_select) {
    std::string info_columns = info_select[1];
    std::string table_name = info_select[2];

    if (!tables_.contains(table_name)) {
        correct_request_ = false;
        std::cout << "This table did not found\n";
        return;
    }

    Table table = tables_[table_name];

    std::vector<std::string> target_columns;
    if (info_columns == "*") {
        target_columns = table.GetColumnsNames();
    } else {
        target_columns = ParseNames(info_columns);
    }

    std::vector<Row> target_rows;
    if (!info_select[3].str().empty()) {
        target_rows = table.ReturnRow(correct_request_, info_select[4]);
    } else {
        target_rows = table.GetRows();
    }

    std::vector<Row> selected_rows = SelectRows(target_columns, target_rows);

    if (correct_request_) {
        std::cout << "Selected columns:\n";
        ViewSelect(target_columns, selected_rows);
    }
}

void Data::ViewSelect(const std::vector<std::string>& columns_names, const std::vector<Row>& selected_rows) {
    for (size_t i = columns_names.size() - 1; i >= 0; --i) {
        std::cout << columns_names[i] << ' ';
    }
}

std::vector<Row> Data::SelectRows(const std::vector<std::string>& target_columns, const std::vector<Row>& target_rows) {
    std::vector<Row> selected_rows;
    for (auto row: target_rows) {
        Row tmp_row = row.GetTargetColumns(correct_request_, target_columns);
        if (!correct_request_) {
            return {};
        }
        selected_rows.emplace_back(tmp_row);
    }
    return selected_rows;
}

void Data::ParseJoin(const std::smatch& info_join) {
    std::vector<std::pair<std::string, std::string>> finish_columns = ReturnPairTableColumn(info_join[1]);

    std::string table_name_left = info_join[2];

    join type_join = GetJoinType(info_join[3]);

    std::string table_name_right = info_join[4];

    std::pair<std::string, std::string> table1 = pair_split(info_join[5], '.');

    std::pair<std::string, std::string> table2 = pair_split(info_join[7], '.');

    if (table1.second != table2.second) {
        correct_request_ = false;
        std::cout << "Different column";
        return;
    }

    std::string column_target = table1.second;

    if (!tables_.contains(table1.first) or !tables_.contains(table2.first)) {
        correct_request_ = false;
        std::cout << "This tables did not found\n";
        return;
    }

    if (table_name_left == table2.first and table_name_right == table1.first) {
        std::swap(table1, table2);
    } else if (table_name_left != table1.first or table_name_right != table2.first) {
        correct_request_ = false;
        std::cout << "Input different tables\n";
        return;
    }

    std::vector<std::pair<Row, Row>> rows_pairs = DisplayTables(table_name_left, table_name_right, column_target,
                                                                type_join);

    std::vector<Row> new_table = ReturnNewRowsJoin(finish_columns, rows_pairs, table_name_left, table_name_right);

    for (size_t i = 0; i < finish_columns.size(); ++i) {
        std::cout << finish_columns[finish_columns.size() - 1 - i].second << ' ';
    }

    std::cout << '\n';;
    for (auto& row: new_table) {
        row.View();
    }
}

std::vector<std::pair<std::string, std::string>> Data::ReturnPairTableColumn(const std::string& pair_group) {
    std::vector<std::pair<std::string, std::string>> finish_columns;
    std::vector<std::string> pairs = split(pair_group, ',');
    for (const auto& pair: pairs) {
        std::pair<std::string, std::string> ans = pair_split(pair, '.');
        finish_columns.emplace_back(ans.first, ans.second);
    }
    return finish_columns;
}

std::vector<std::pair<Row, Row>>
Data::DisplayTables(const std::string& table_left, const std::string& table_right,
                    const std::string& column, join type) {
    std::vector<std::pair<Row, Row>> rows_pair;
    Table left = tables_[table_left], right = tables_[table_right];

    if (!left.ContainColumn(column)) {
        correct_request_ = false;
        std::cout << "Incorrect request with column name\n";
        return {};
    }

    for (auto row_left: left.GetRows()) {
        bool inserted = false;
        for (auto row_right: right.GetRows()) {
            if (row_left.GetCell(column) == row_right.GetCell(column)) {
                rows_pair.emplace_back(row_left, row_right);
                inserted = true;
                break;
            }
        }

        if (!inserted and (type == LEFT or type == RIGHT)) {
            rows_pair.emplace_back(row_left, Row());
        }
    }
    return rows_pair;
}

std::vector<Row> Data::ReturnNewRowsJoin(const std::vector<std::pair<std::string, std::string>>& finish_columns,
                                         const std::vector<std::pair<Row, Row>>& pair,
                                         const std::string& table_left,
                                         const std::string& table_right) {
    std::vector<Row> new_rows;

    for (auto [row_left, row_right]: pair) {
        Row row;
        for (auto [table_name, column]: finish_columns) {
            if (table_name == table_left) {
                row[column] = row_left[column];
            } else {
                if (row_right.fiction) {
                    row[column] = Cell(WASTE, "NULL");
                } else {
                    row[column] = row_right[column];
                }
            }
        }
        new_rows.emplace_back(row);
    }
    return new_rows;
}


void Parser() {
    Data data;
    std::string request;
    while (std::getline(std::cin, request) and request != "exit") {
        data.ParseRequest(request);
    }
}