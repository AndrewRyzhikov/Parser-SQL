#include <utility>

#include "storage.h"

struct Cell {
    Cell() : value("NULL") {};

    Cell(const types type_) : type(type_) {};

    Cell(const types type_, std::string  value_) : type(type_), value(std::move(value_)) {};

    bool operator==(const Cell& x) const {
        return (value == x.value and type == x.type);
    }

    bool operator!=(const Cell& x) const {
        return !(*this == x);
    }

    std::string value;
    types type;
};

struct ColumnDisription {
    ColumnDisription() : is_create(false) {};

    ColumnDisription(const types type, bool null_) : value_type(type), is_create(true), null(null_) {};

    types value_type;
    std::string name_column;
    bool is_create;
    bool null;
};


class Row {
public:

    Row() = default;

    Row(bool flag) : fiction(flag) {};

    Cell& operator[](const std::string& column_name) {
        return cells_[column_name];
    }

    Cell GetCell(const std::string& column_name) {
        if (!cells_.contains(column_name)) {
            return Cell(WASTE, "NULL");
        }
        return cells_[column_name];
    }

    bool operator==(const Row& other) const {
        for (auto [name, cell]: cells_) {
            auto it = other.cells_.find(name);
            if (it != other.cells_.end()) {
                if (it->second != cell) {
                    return false;
                }
            } else {
                return false;
            }
        }

        for (auto [name, cell]: other.cells_) {
            auto it = cells_.find(name);
            if (it != cells_.end()) {
                if (it->second != cell) {
                    return false;
                }
            } else {
                return false;
            }
        }

        return true;
    }


    bool operator!=(Row& other) const {
        return !(other == *this);
    }

    void View() const;

    Row GetTargetColumns(bool& correct_request, const std::vector<std::string>& columns);

    Row GetTargetColumnsJoin(join type, bool& correct_request, const std::vector<std::string>& columns);

    bool fiction = false;
private:
    std::unordered_map<std::string, Cell> cells_;
};