#pragma once

#include "include.h"
#include "Ir.h"
#include "ast.h"
#include "symbol.h"


struct GenContext
{
    IRBasicBlock* current_block = nullptr;
    SymbolTable* symbol_table = nullptr;
    IRProgram* program = nullptr;
};



class IRGenerator {
private:
    std::unique_ptr<IRProgram> program;
    static int blockcount;
    std::unique_ptr<SymbolTable> symbol_table;

    GenContext ctx;

    int temp_counter = 0;
    std::string generate_temp_name() {
        return "%" + std::to_string(temp_counter++);
    }
    
public:
    IRGenerator();
    void visitCompUnit(const CompUnitAST* ast);
    void visitFunDef(const FunDefAST* ast);
    void visitFunType(const FunTypeAST* ast);
    void visitBlock(const BlockAST* ast );
    void visitStmt(const StmtAST* ast );
    void visitBlockItems(const BlockItemsAST* ast);
    void visitBlockItem(const BlockItemAST* ast);
    void visitDecl(const DeclAST* ast);
    void visitConstDecl(const ConstDeclAST* ast);
    void visitConstDefs(const ConstDefsAST* ast);
    void visitConstDef(const ConstDefAST* ast);
    std::unique_ptr<BaseIRValue> visitConstInitVal(const ConstInitValAST* ast);
    std::unique_ptr<BaseIRValue> visitExp(const ExpAST* ast );
    std::unique_ptr<BaseIRValue> visitAddExp(const AddExpAST* ast );
    std::unique_ptr<BaseIRValue> visitMulExp(const MulExpAST* ast );
    std::unique_ptr<BaseIRValue> visitLOrExp(const LOrExpAST* ast );
    std::unique_ptr<BaseIRValue> visitLAndExp(const LAndExpAST* ast );
    std::unique_ptr<BaseIRValue> visitEqExp(const EqExpAST* ast );
    std::unique_ptr<BaseIRValue> visitRelExp(const RelExpAST* ast );
    std::unique_ptr<BaseIRValue> visitPrimaryExp(const PrimaryExpAST* ast );
    std::unique_ptr<BaseIRValue> visitUnaryExp(const UnaryExpAST* ast );
    std::unique_ptr<BaseIRValue> visitNumber(const NumberAST* ast );
    std::unique_ptr<BaseIRValue> visitConstExp(const ConstExpAST* ast);

    std::unique_ptr<IRProgram> get_irprogram();

    void setCurrentBlock(IRBasicBlock* block){
        ctx.current_block = block;
    }

    IRBasicBlock* getCurrentBlock() const {
        return ctx.current_block;
    }


};