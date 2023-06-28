#include "Table.h"

std::vector<std::string> split(const std::string& input, char delimiter);

std::pair<std::string, std::string> pair_split(const std::string& input, char delimiter);

void Parser();

class Data {
public:
    Data() = default;

    void ParseRequest(const std::string& request);

    Table& operator[](const std::string& table_name) {
        return tables_[table_name];
    }

    ~Data() = default;

    void View(const std::string& table_name);

private:

    void MakeBijection(const std::string& table_name, const std::vector<std::string>& columns,
                       const std::vector<std::string>& values);

    void ParseCreateTable(const std::smatch& info_table);

    void ParseInsert(const std::smatch& info_select);

    void ParseDrop(const std::smatch& info_drop);

    void ParseDelete(const std::smatch& info_delete);

    void ParseSelect(const std::smatch& info_select);

    void ParseJoin(const std::smatch& info_join);

    void ParseUpdate(const std::smatch& info_update);

    std::vector<std::pair<std::string, std::string>> ReturnPairTableColumn(const std::string& pair);

    static void ViewSelect(const std::vector<std::string>& columns_names, const std::vector<Row>& selected_rows);

    std::vector<Row> SelectRows(const std::vector<std::string>& target_columns, const std::vector<Row>& target_rows);

    std::vector<std::pair<std::string, Cell>> ParsePair(const std::string& pair);

    std::vector<std::string> ParseNames(const std::string& str_columns);

    std::vector<std::pair<Row, Row>>
    DisplayTables(const std::string& table_left, const std::string& table_right, const std::string& column, join type);

    std::vector<Row> ReturnNewRowsJoin(const std::vector<std::pair<std::string, std::string>>& finish_columns,
                                       const std::vector<std::pair<Row, Row>>& pair, const std::string& table_left,
                                       const std::string& table_right);

    bool correct_request_;
    std::unordered_map<std::string, Table> tables_;
};
