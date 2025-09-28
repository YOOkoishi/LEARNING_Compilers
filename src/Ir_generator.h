#pragma once

#include "include.h"
#include "Ir.h"
#include "ast.h"

// void Ir_generator(std::unique_ptr<BaseAST>& ast,std::unique_ptr<IRProgram>& ir);
// 还是用类写吧

class IRGenerator {
private:
    std::unique_ptr<IRProgram> program;
    static int blockcount;
    int temp_counter = 0;
    std::string generate_temp_name() {
        return "%" + std::to_string(temp_counter++);
    }
    
public:
    IRGenerator();
    void visitCompUnit(const CompUnitAST* ast);
    void visitFunDef(const FunDefAST* ast);
    void visitFunType(const FunTypeAST* ast);
    void visitBlock(const BlockAST* ast, IRBasicBlock* current_block);
    void visitStmt(const StmtAST* ast, IRBasicBlock* current_block);
    std::unique_ptr<BaseIRValue> visitExp(const ExpAST* ast, IRBasicBlock* current_block);
    std::unique_ptr<BaseIRValue> visitAddExp(const AddExpAST* ast, IRBasicBlock* current_block);
    std::unique_ptr<BaseIRValue> visitMulExp(const MulExpAST* ast, IRBasicBlock* current_block);
    std::unique_ptr<BaseIRValue> visitPrimaryExp(const PrimaryExpAST* ast, IRBasicBlock* current_block);
    std::unique_ptr<BaseIRValue> visitUnaryExp(const UnaryExpAST* ast, IRBasicBlock* current_block);
    std::unique_ptr<BaseIRValue> visitNumber(const NumberAST* ast, IRBasicBlock* current_block);
    std::unique_ptr<IRProgram> get_irprogram();
};