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
    std::string name;       // 源码中的名字
    std::string ir_name;    // IR 中的唯一名字
    SymbolType type;
    DataType datatype;
    int scope_level;
    int const_value;
    std::vector<int> array_dims;
    std::vector<int> const_array_values;

    int get_const_array_value(int flat_index){
        if(type == SymbolType::CONST && datatype == DataType::ARRAY){
            if(flat_index < const_array_values.size()){
                return const_array_values[flat_index];
            }
        }
        return 0;
    }


    Symbol() = default;
    Symbol(const std::string& n, const std::string& ir, SymbolType t, DataType dt, int lvl, int val = 0)
        : name(n), ir_name(ir), type(t), datatype(dt), scope_level(lvl), const_value(val) {}

    // Constructor for arrays
    Symbol(const std::string& n, const std::string& ir, SymbolType t, DataType dt, int lvl, 
           const std::vector<int>& dims, const std::vector<int>& vals = {})
        : name(n), ir_name(ir), type(t), datatype(dt), scope_level(lvl), const_value(0), 
          array_dims(dims), const_array_values(vals) {}
};


class SymbolTable {
public:
    // 作用域栈：每个 map 是一个作用域
    std::vector<std::map<std::string, std::shared_ptr<Symbol>>> symbol_table;
    
private:
    int current_scope_level = -1;
    // 用于生成唯一 IR 名称
    std::map<std::string, int> name_counter;
    
public:
    SymbolTable() {
        // 初始化全局作用域
        enterScope();
    }
    
    // 进入新作用域
    void enterScope() {
        symbol_table.push_back(std::map<std::string, std::shared_ptr<Symbol>>());
        current_scope_level++;
    } 
    
    // 离开作用域
    void exitScope() {
        if (symbol_table.size() > 1) {
            symbol_table.pop_back();
            current_scope_level--;
        }
    } 
    
    // 生成唯一的 IR 名称
    std::string generateIRName(const std::string& name) {
        int count = name_counter[name]++;
        std::string prefix = (current_scope_level == 0) ? "@" : "%";
        if (count == 0) {
            return prefix + name;
        }
        return prefix + name + "_" + std::to_string(count);
    }
    
    // 声明符号（插入当前作用域），返回生成的 IR 名称
    std::string declare(const std::string& name, SymbolType type, DataType datatype, int value = 0) {
        auto& current_scope = symbol_table.back();
        
        // 检查当前作用域是否已存在
        if (current_scope.find(name) != current_scope.end()) {
            throw std::runtime_error("Error: Redeclaration of '" + name + "'");
        }
        
        std::string ir_name = generateIRName(name);
        auto symbol = std::make_shared<Symbol>(name, ir_name, type, datatype, current_scope_level, value);
        current_scope[name] = symbol;
        return ir_name;
    }

    // 声明数组符号
    std::string declareArray(const std::string& name, SymbolType type, DataType datatype, 
                             const std::vector<int>& dims, const std::vector<int>& values = {}) {
        auto& current_scope = symbol_table.back();
        
        if (current_scope.find(name) != current_scope.end()) {
            throw std::runtime_error("Error: Redeclaration of '" + name + "'");
        }
        
        std::string ir_name = generateIRName(name);
        auto symbol = std::make_shared<Symbol>(name, ir_name, type, datatype, current_scope_level, dims, values);
        current_scope[name] = symbol;
        return ir_name;
    }
    
    // 查询符号（从内到外查找）
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
    
    // 调试：打印符号表
    void print() const {
        std::cout << "=== Symbol Table ===" << std::endl;
        for (size_t i = 0; i < symbol_table.size(); i++) {
            std::cout << "Scope " << i << ":" << std::endl;
            for (const auto& [name, symbol] : symbol_table[i]) {
                std::cout << "  " << name << " -> " << symbol->ir_name;
                std::cout << " (" << (symbol->type == SymbolType::CONST ? "const" : "var");
                std::cout << " int";
                if (symbol->type == SymbolType::CONST) {
                    std::cout << " = " << symbol->const_value;
                }
                std::cout << ")" << std::endl;
            }
        }
    }


    bool isGlobalScope() const {
        return current_scope_level == 0;
    }

    // 获取当前作用域层级
    int getCurrentScopeLevel() const {
        return current_scope_level;
    }
};