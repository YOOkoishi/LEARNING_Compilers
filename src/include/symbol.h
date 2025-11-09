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
    POINTER  
};

class Symbol {
public:
    std::string name;
    SymbolType type;
    DataType datatype;
    int scope_level;
    int const_value;
    
    Symbol(const std::string& n, SymbolType t, DataType dt, int lvl, int val = 0)
        : name(n), type(t), datatype(dt), scope_level(lvl), const_value(val) {}
};


class SymbolTable {
public:
    // ✅ 作用域栈：每个 map 是一个作用域
    std::vector<std::map<std::string, std::shared_ptr<Symbol>>> symbol_table;
    
private:
    int current_scope_level = 0;
    
public:
    SymbolTable() {
        // ✅ 初始化全局作用域
        enterScope();
    }
    
    // ✅ 进入新作用域
    void enterScope() {
        symbol_table.push_back(std::map<std::string, std::shared_ptr<Symbol>>());
        current_scope_level++;
    }
    
    // ✅ 离开作用域
    void exitScope() {
        if (symbol_table.size() > 1) {
            symbol_table.pop_back();
            current_scope_level--;
        }
    }
    
    // ✅ 声明符号（插入当前作用域）
    bool declare(const std::string& name, SymbolType type, DataType datatype, int value = 0) {
        auto& current_scope = symbol_table.back();
        
        // 检查当前作用域是否已存在
        if (current_scope.find(name) != current_scope.end()) {
            throw std::runtime_error("Error: Redeclaration of '" + name + "'");
        }
        
        auto symbol = std::make_shared<Symbol>(name, type, datatype, current_scope_level, value);
        current_scope[name] = symbol;
        return true;
    }
    
    // ✅ 查询符号（从内到外查找）
    std::shared_ptr<Symbol> lookup(const std::string& name) {
        // 从最内层作用域向外查找
        for (int i = symbol_table.size() - 1; i >= 0; i--) {
            auto it = symbol_table[i].find(name);
            if (it != symbol_table[i].end()) {
                return it->second;
            }
        }
        return nullptr;  // 未找到
    }
    
    // ✅ 调试：打印符号表
    void print() const {
        std::cout << "=== Symbol Table ===" << std::endl;
        for (size_t i = 0; i < symbol_table.size(); i++) {
            std::cout << "Scope " << i << ":" << std::endl;
            for (const auto& [name, symbol] : symbol_table[i]) {
                std::cout << "  " << name;
                std::cout << " (" << (symbol->type == SymbolType::CONST ? "const" : "var");
                std::cout << " int";
                if (symbol->type == SymbolType::CONST) {
                    std::cout << " = " << symbol->const_value;
                }
                std::cout << ")" << std::endl;
            }
        }
    }
};