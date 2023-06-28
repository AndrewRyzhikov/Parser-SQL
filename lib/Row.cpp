#include "Row.h"

void Row::View() const {
    for (auto [column, cell]: cells_) {
        std::cout << cell.value << ' ';
    }
    std::cout << '\n';
}

Row Row::GetTargetColumns(bool& correct_request, const std::vector<std::string>& columns){
    Row new_row;
    for (const auto& column: columns) {
        if (!cells_.contains(column)) {
            correct_request = false;
            std::cout << "Column " << column << "did not found\n";
            return {};
        }
        new_row[column] = cells_[column];
    }
    return new_row;
}

Row Row::GetTargetColumnsJoin(join type, bool& correct_request, const std::vector<std::string>& columns){
    Row new_row;
    for (const auto& column: columns) {
        if (!cells_.contains(column) and type != INNER) {
            new_row[column] = Cell(WASTE, "NULL");
        }
        new_row[column] = cells_[column];
    }
    return new_row;
}

