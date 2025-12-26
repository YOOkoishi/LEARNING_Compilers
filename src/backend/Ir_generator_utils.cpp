#include "../include/include.h"
#include "../include/ast.h"
#include "../include/Ir.h"
#include "../include/Ir_generator.h"



int IRGenerator::evaluateConstExp(const BaseAST* ast) {
    if (!ast) return 0;
    
    // ===== Number =====
    if (auto number = dynamic_cast<const NumberAST*>(ast)) {
        return number->int_const;
    }
    
    // ===== ConstInitVal =====
    if (auto init_val = dynamic_cast<const ConstInitValAST*>(ast)) {
        return evaluateConstExp(init_val->constexp.get());
    }
    
    // ===== ConstExp =====
    if (auto const_exp = dynamic_cast<const ConstExpAST*>(ast)) {
        return evaluateConstExp(const_exp->exp.get());
    }
    
    // ===== Exp =====
    if (auto exp = dynamic_cast<const ExpAST*>(ast)) {
        return evaluateConstExp(exp->lorexp.get());
    }
    
    // ===== LOrExp =====
    if (auto lorexp = dynamic_cast<const LOrExpAST*>(ast)) {
        if (lorexp->type == LOrExpAST::LANDEXP) {
            return evaluateConstExp(lorexp->landexp.get());
        } else {
            int left = evaluateConstExp(lorexp->lorexp.get());
            int right = evaluateConstExp(lorexp->landexp.get());
            return left || right;
        }
    }
    
    // ===== LAndExp =====
    if (auto landexp = dynamic_cast<const LAndExpAST*>(ast)) {
        if (landexp->type == LAndExpAST::EQEXP) {
            return evaluateConstExp(landexp->eqexp.get());
        } else {
            int left = evaluateConstExp(landexp->landexp.get());
            int right = evaluateConstExp(landexp->eqexp.get());
            return left && right;
        }
    }
    
    // ===== EqExp =====
    if (auto eqexp = dynamic_cast<const EqExpAST*>(ast)) {
        if (eqexp->type == EqExpAST::RELEXP) {
            return evaluateConstExp(eqexp->relexp.get());
        } else {
            int left = evaluateConstExp(eqexp->eqexp.get());
            int right = evaluateConstExp(eqexp->relexp.get());
            if (eqexp->op == "==") {
                return left == right;
            } else {  // !=
                return left != right;
            }
        }
    }
    
    // ===== RelExp =====
    if (auto relexp = dynamic_cast<const RelExpAST*>(ast)) {
        if (relexp->type == RelExpAST::ADDEXP) {
            return evaluateConstExp(relexp->addexp.get());
        } else {
            int left = evaluateConstExp(relexp->relexp.get());
            int right = evaluateConstExp(relexp->addexp.get());
            if (relexp->op == "<") {
                return left < right;
            } else if (relexp->op == ">") {
                return left > right;
            } else if (relexp->op == "<=") {
                return left <= right;
            } else {  // >=
                return left >= right;
            }
        }
    }
    
    // ===== AddExp =====
    if (auto addexp = dynamic_cast<const AddExpAST*>(ast)) {
        if (addexp->type == AddExpAST::MULONLY) {
            return evaluateConstExp(addexp->mulexp.get());
        } else {
            int left = evaluateConstExp(addexp->addexp.get());
            int right = evaluateConstExp(addexp->mulexp.get());
            if (addexp->op == "+") {
                return left + right;
            } else {  // -
                return left - right;
            }
        }
    }
    
    // ===== MulExp =====
    if (auto mulexp = dynamic_cast<const MulExpAST*>(ast)) {
        if (mulexp->type == MulExpAST::UNARYEXP) {
            return evaluateConstExp(mulexp->unrayexp.get());
        } else {
            int left = evaluateConstExp(mulexp->mulexp.get());
            int right = evaluateConstExp(mulexp->unrayexp.get());
            if (mulexp->op == "*") {
                return left * right;
            } else if (mulexp->op == "/") {
                if (right == 0) {
                    throw std::runtime_error("Division by zero in constant expression");
                }
                return left / right;
            } else {  // %
                if (right == 0) {
                    throw std::runtime_error("Modulo by zero in constant expression");
                }
                return left % right;
            }
        }
    }
    
    // ===== UnaryExp =====
    if (auto unaryexp = dynamic_cast<const UnaryExpAST*>(ast)) {
        if (unaryexp->type == UnaryExpAST::PRIMARYEXP) {
            return evaluateConstExp(unaryexp->primaryexp.get());
        } else {
            int value = evaluateConstExp(unaryexp->unaryexp.get());
            if (unaryexp->unaryop == "+") {
                return value;
            } else if (unaryexp->unaryop == "-") {
                return -value;
            } else {  // !
                return !value;
            }
        }
    }
    
    // ===== PrimaryExp =====
    if (auto primaryexp = dynamic_cast<const PrimaryExpAST*>(ast)) {
        if (primaryexp->type == PrimaryExpAST::NUMBER) {
            return evaluateConstExp(primaryexp->number.get());
        } else if (primaryexp->type == PrimaryExpAST::EXP) {
            return evaluateConstExp(primaryexp->exp.get());
        } else if (primaryexp->type == PrimaryExpAST::LVAL) {
            // ✅ LVal：从符号表查询
            auto lval = dynamic_cast<LValAST*>(primaryexp->lval.get());
            auto symbol = ctx.symbol_table->lookup(lval->ident);
            if (!symbol) {
                throw std::runtime_error("Undefined constant: " + lval->ident);
            }
            if (symbol->type != SymbolType::CONST) {
                throw std::runtime_error("'" + lval->ident + "' is not a constant");
            }
            return symbol->const_value;
        }
    }
    
    throw std::runtime_error("Cannot evaluate constant expression");
}


std::vector<int> IRGenerator::evaluateGlobalInitVal(const InitValAST* ast, int array_size) {
    if (!ast) return {};
    std::vector<int> init_list;
    
    if (ast->type == InitValAST::ARRAY) {
        if (auto explist = dynamic_cast<ExpListAST*>(ast->initlist.get())) {
            for (const auto& exp : explist->explist) {
                init_list.push_back(evaluateConstExp(exp.get()));
            }
        }
        while (init_list.size() < array_size) {
            init_list.push_back(0);
        }
    } else if (ast->type == InitValAST::ZEROINIT) {
        return {};
    }
    return init_list;
}


std::string IRGenerator::generate_data_type(const std::vector<int> &dim){
    int last = dim.back();
    std::string inner =  "[i32, " + std::to_string(last) + "]"; 
    for(auto it = dim.rbegin() + 1 ; it != dim.rend() ; ++it){
        inner = "[" + inner + ", " + std::to_string(*it) + "]"; 
    }
    return inner;
}




// 辅助函数：计算从第 start_dim 维开始的子数组容量
int IRGenerator::calculate_sub_capacity(const std::vector<int>& dims, int start_dim) {
    int capacity = 1;
    for (int i = start_dim; i < static_cast<int>(dims.size()); ++i) {
        capacity *= dims[i];
    }
    return capacity;
}

int IRGenerator::find_alignment_dim(int cursor, const std::vector<int>& dims) {
    if (cursor == 0) return 0;  // 起始位置总是对齐第0维
    
    // 从最外层开始检查（索引最小的维度），找到第一个能整除的维度
    // 第i维的步长 = dims[i+1] * dims[i+2] * ... * dims[n-1]
    // 即 calculate_sub_capacity(dims, i+1)
    // 例如 int[2][3][4]:
    //   - dim0步长 = 3*4 = 12
    //   - dim1步长 = 4
    //   - dim2步长 = 1
    //   - cursor=4: 4%12≠0, 4%4==0 → 返回1，对应int[4]
    //   - cursor=12: 12%12==0 → 返回0，对应int[3][4]
    for (int i = 0; i < static_cast<int>(dims.size()); ++i) {
        int stride = calculate_sub_capacity(dims, i + 1);
        if (cursor % stride == 0) {
            return i;
        }
    }
    
    return -1;  // 未对齐任何维度
}

void IRGenerator::flatten_initval_recursive(
    const InitValAST* node,
    const std::vector<int>& dims,
    int& cursor,
    std::vector<int>& result
) {
    if (!node) return;
    
    // 检查1: 维度是否为空
    if (dims.empty()) {
        throw std::runtime_error(
            "Too many nesting levels in initializer (exceeded array dimensions)"
        );
    }
    
    if (node->type == InitValAST::EXP) {
        // 情况1: 标量 - 直接填充
        if (cursor >= result.size()) {
            throw std::runtime_error("Array initializer overflow");
        }
        result[cursor++] = evaluateConstExp(node->exp.get());
    }
    else if (node->type == InitValAST::ZEROINIT) {
        // 情况2: 空列表 {} - 填充0到对齐边界
        int align_dim = find_alignment_dim(cursor, dims);
        if (align_dim == -1) {
            throw std::runtime_error(
                "Empty initializer {} at position " + std::to_string(cursor) +
                " is not aligned to any dimension boundary"
            );
        }
        
        // 检查2: 是否有子维度可以填充
        if (align_dim + 1 >= dims.size()) {
            throw std::runtime_error(
                "Empty initializer {} used for scalar element at position " +
                std::to_string(cursor)
            );
        }
        
        // 计算子维度的容量
        int sub_capacity = 1;
        for (int i = align_dim + 1; i < dims.size(); ++i) {
            sub_capacity *= dims[i];
        }
        
        // 填充0
        for (int i = 0; i < sub_capacity && cursor < result.size(); ++i) {
            result[cursor++] = 0;
        }
    }
    else if (node->type == InitValAST::ARRAY) {
        // 情况3: 嵌套列表 {...}
        int list_start = cursor;
        
        // 对齐检查
        int align_dim = find_alignment_dim(cursor, dims);
        if (align_dim == -1) {
            throw std::runtime_error(
                "Nested initializer at position " + std::to_string(cursor) +
                " is not aligned to any dimension boundary"
            );
        }
        
        // 提取子维度
        std::vector<int> sub_dims(dims.begin() + align_dim + 1, dims.end());
        
        // 计算当前 {} 对应的子数组大小
        int sub_capacity = sub_dims.empty() ? 1 : calculate_sub_capacity(sub_dims, 0);
        
        // 递归处理列表中的每个元素
        auto initlist = dynamic_cast<InitValListAST*>(node->initlist.get());
        if (initlist) {
            for (const auto& child : initlist->initlist) {
                auto child_initval = dynamic_cast<InitValAST*>(child.get());
                if (!sub_dims.empty()) {
                    // 还有子维度，继续递归
                    flatten_initval_recursive(child_initval, sub_dims, cursor, result);
                } else {
                    // 子维度为空，当前元素应该是标量
                    if (child_initval) {
                        if (child_initval->type == InitValAST::EXP) {
                            if (cursor < static_cast<int>(result.size())) {
                                result[cursor++] = evaluateConstExp(child_initval->exp.get());
                            }
                        } else {
                            // 嵌套的 {} 或 ZEROINIT 在最后一维，这是语义错误
                            throw std::runtime_error(
                                "Nested braces in scalar context at position " + std::to_string(cursor)
                            );
                        }
                    }
                }
            }
        }
        
        // 关键：填充到子数组边界
        // 每个 {} 会"消耗"其对齐维度对应的完整子数组空间
        int target = list_start + sub_capacity;
        while (cursor < target && cursor < static_cast<int>(result.size())) {
            result[cursor++] = 0;
        }
    }
}

std::vector<int> IRGenerator::flatten_array(const InitValAST* initval,const std::vector<int>& dims) {
    // 计算总容量
    int total_size = 1;
    for (int d : dims) total_size *= d;
    
    // 初始化结果数组（全0）
    std::vector<int> result(total_size, 0);
    
    if (!initval) return result;
    
    int cursor = 0;
    
    if (initval->type == InitValAST::ARRAY) {
        // 最外层是列表 {...}
        auto initlist = dynamic_cast<InitValListAST*>(initval->initlist.get());
        if (initlist) {
            for (const auto& child : initlist->initlist) {
                auto child_initval = dynamic_cast<InitValAST*>(child.get());
                flatten_initval_recursive(child_initval, dims, cursor, result);
            }
        }
    } 
    else if (initval->type == InitValAST::EXP) {
        // 单个表达式初始化整个数组（少见但可能）
        result[0] = evaluateConstExp(initval->exp.get());
    }
    else if (initval->type == InitValAST::ZEROINIT) {
        // 空列表 {} - 已经是全0，直接返回
    }
    
    return result;
}


void IRGenerator::flatten_constinitval_recursive(const ConstInitValAST* node,const std::vector<int>& dims,int& cursor,std::vector<int>& result
) {
    if (!node) return;
    
    // 检查1: 维度是否为空
    if (dims.empty()) {
        throw std::runtime_error(
            "Too many nesting levels in initializer (exceeded array dimensions)"
        );
    }
    
    if (node->type == ConstInitValAST::CONSTEXP) {
        // 情况1: 常量表达式 - 直接填充
        if (cursor >= result.size()) {
            throw std::runtime_error("Array initializer overflow");
        }
        result[cursor++] = evaluateConstExp(node->constexp.get());
    }
    else if (node->type == ConstInitValAST::ZEROINIT) {
        // 情况2: 空列表 {} - 填充0到对齐边界
        int align_dim = find_alignment_dim(cursor, dims);
        if (align_dim == -1) {
            throw std::runtime_error( 
                "Empty initializer {} at position " + std::to_string(cursor) +
                " is not aligned to any dimension boundary"
            );
        }
        
        // 检查2: 是否有子维度可以填充
        if (align_dim + 1 >= dims.size()) {
            throw std::runtime_error(
                "Empty initializer {} used for scalar element at position " +
                std::to_string(cursor)
            );
        }
        
        // 计算子维度的容量
        int sub_capacity = 1;
        for (int i = align_dim + 1; i < dims.size(); ++i) {
            sub_capacity *= dims[i];
        }
        
        // 填充0
        for (int i = 0; i < sub_capacity && cursor < result.size(); ++i) {
            result[cursor++] = 0;
        }
    }
    else if (node->type == ConstInitValAST::CONSTLIST) {
        // 情况3: 嵌套列表 {...}
        int list_start = cursor;
        
        // 对齐检查
        int align_dim = find_alignment_dim(cursor, dims);
        if (align_dim == -1) {
            throw std::runtime_error(
                "Nested initializer at position " + std::to_string(cursor) +
                " is not aligned to any dimension boundary"
            );
        }
        
        // 提取子维度
        std::vector<int> sub_dims(dims.begin() + align_dim + 1, dims.end());
        
        // 计算当前 {} 对应的子数组大小
        int sub_capacity = sub_dims.empty() ? 1 : calculate_sub_capacity(sub_dims, 0);
        
        // 递归处理列表中的每个元素
        auto initlist = dynamic_cast<ConstInitValListAST*>(node->constinitlist.get());
        if (initlist) {
            for (const auto& child : initlist->constinitlist) {
                auto child_initval = dynamic_cast<ConstInitValAST*>(child.get());
                if (!sub_dims.empty()) {
                    // 还有子维度，继续递归
                    flatten_constinitval_recursive(child_initval, sub_dims, cursor, result);
                } else {
                    // 子维度为空，当前元素应该是常量表达式
                    if (child_initval) {
                        if (child_initval->type == ConstInitValAST::CONSTEXP) {
                            if (cursor < static_cast<int>(result.size())) {
                                result[cursor++] = evaluateConstExp(child_initval->constexp.get());
                            }
                        } else {
                            // 嵌套的 {} 或 ZEROINIT 在最后一维，这是语义错误
                            throw std::runtime_error(
                                "Nested braces in scalar context at position " + std::to_string(cursor)
                            );
                        }
                    }
                }
            }
        }
        
        // 关键：填充到子数组边界
        int target = list_start + sub_capacity;
        while (cursor < target && cursor < static_cast<int>(result.size())) {
            result[cursor++] = 0;
        }
    }
}





std::vector<int> IRGenerator::flatten_const_array(const ConstInitValAST* initval,const std::vector<int>& dims) {
    // 计算总容量
    int total_size = 1;
    for (int d : dims) total_size *= d;
    
    // 初始化结果数组（全0）
    std::vector<int> result(total_size, 0);
    
    if (!initval) return result;
    
    int cursor = 0;
    
    if (initval->type == ConstInitValAST::CONSTLIST) {
        // 最外层是列表 {...}
        auto initlist = dynamic_cast<ConstInitValListAST*>(initval->constinitlist.get());
        if (initlist) {
            for (const auto& child : initlist->constinitlist) {
                auto child_initval = dynamic_cast<ConstInitValAST*>(child.get());
                flatten_constinitval_recursive(child_initval, dims, cursor, result);
            }
        }
    }
    else if (initval->type == ConstInitValAST::CONSTEXP) {
        // 单个常量表达式初始化
        result[0] = evaluateConstExp(initval->constexp.get());
    }
    else if (initval->type == ConstInitValAST::ZEROINIT) {
        // 空列表 {} - 已经是全0，直接返回
    }

    return result;
}


