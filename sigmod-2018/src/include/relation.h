#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include "parser.h"

using RelationId = unsigned;

class Relation {
private:
    /// Owns memory (false if it was mmaped)
    bool owns_memory_;
    /// The number of tuples
    uint64_t size_;
    /// The join column containing the keys
    std::vector<uint64_t *> columns_;
public:
    /// Constructor without mmap
    Relation(uint64_t size, std::vector<uint64_t *> &&columns)
        : owns_memory_(true), size_(size), columns_(columns) {}
    /// Constructor using mmap
    explicit Relation(const char *file_name);
    /// Delete copy constructor
    Relation(const Relation &other) = delete;
    /// Move constructor
    Relation(Relation &&other) = default;

    /// The destructor
    ~Relation();

    /// Stores a relation into a file (binary)
    void storeRelation(const std::string &file_name);
    /// Stores a relation into a file (csv)
    void storeRelationCSV(const std::string &file_name);
    /// Dump SQL: Create and load table (PostgreSQL)
    void dumpSQL(const std::string &file_name, unsigned relation_id);

    /// The number of tuples
    uint64_t size() const {
        return size_;
    }
    /// The join column containing the keys
    const std::vector<uint64_t *> &columns() const {
        return columns_;
    }

    std::vector<std::uint64_t> applyFilter(std::vector<FilterInfo> &filters) const{
        std::vector<std::uint64_t> res;
        res.reserve(50);
        for(int i=0;i<size_;i++){
            for(auto &f:filters){
                auto compare_col = columns_[f.filter_column.col_id];
                auto constant = f.constant;
                switch (f.comparison) {
                    case FilterInfo::Comparison::Equal:
                        if(compare_col[i] == constant)res.push_back(i);
                        break;
                    case FilterInfo::Comparison::Greater:
                        if(compare_col[i] > constant)res.push_back(i);
                        break;
                    case FilterInfo::Comparison::Less:
                        if(compare_col[i] < constant)res.push_back(i);
                };
            }
        }
        return res;
    }


private:
    /// Loads data from a file
    void loadRelation(const char *file_name);
};

