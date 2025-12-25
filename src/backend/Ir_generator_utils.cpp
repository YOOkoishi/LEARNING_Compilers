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




std::vector<int> IRGenerator::generate_float_array(const InitValAST* ast, const std::vector<int>& dim){
    
}