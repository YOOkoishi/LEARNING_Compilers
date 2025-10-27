#pragma once
#include "include.h"

enum class SymbolType {
    CONST,
    VAR,
    FUNCTION
};


enum class DataType {
    INT,
    VOID,
    FLOAT,
    DOUBLE,
    ARRAY,
    FOINTER
};


class Symbol {
public:
    std::string name;
    SymbolType type;
    DataType datatype;
    int scope_level;

    int const_value;

};


class SymbolTable {
public:
    std::vector<std::map<std::string,std::shared_ptr<Symbol>>> symbol_table;
    
};