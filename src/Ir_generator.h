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
    
public:
    IRGenerator(){};
    void visitCompUnit(const CompUnitAST* ast);
    void visitFunDef(const FunDefAST* ast);
    void visitFunType(const FunTypeAST* ast);
    void visitBlock(const BlockAST* ast);
    void visitStmt(const StmtAST* ast);
    void visitNumber(const NumberAST* ast);
    std::unique_ptr<IRProgram> get_irprogram();
};