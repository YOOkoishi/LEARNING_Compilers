#include "include.h"
#include "ast.h"
#include "Ir.h"
#include "Ir_generator.h"

int IRGenerator::blockcount = 0;

IRGenerator::IRGenerator() : program(std::make_unique<IRProgram>()){

}

void IRGenerator::visitCompUnit(const CompUnitAST* ast){
    if(ast && ast -> fun_def){
        if(auto func_def = dynamic_cast<const FunDefAST*>(ast->fun_def.get())){
            visitFunDef(func_def);
        }
    } 
}

void IRGenerator::visitFunDef(const FunDefAST* ast){
    if(!ast) return;
    
    auto ir_fun = std::make_unique<IRFunction>(); 
    ir_fun -> function_name = "@" + (ast -> ident);
    
    if (ast -> fun_type){
        if (auto fun_type = dynamic_cast<const FunTypeAST*>(ast ->fun_type.get())){
            visitFunType(fun_type);
            ir_fun ->functype = ((fun_type ->tp) == "int")? "i32":"other";
        }
    }

    program ->ADD_Function(std::move(ir_fun));

    IRFunction* current_func = program->ir_function.back().get();

    auto entry_block = std::make_unique<IRBasicBlock>();
    entry_block->block_name = "%entry" + std::to_string(blockcount++);
    
    current_func ->ADD_Block(std::move(entry_block));
    
    IRBasicBlock* current_block = current_func ->ir_basicblock.back().get();

    if (ast ->block){
        if(auto block = dynamic_cast<const BlockAST*>(ast->block.get())){
            visitBlock(block , current_block);
        }
    }
}

void IRGenerator::visitFunType(const FunTypeAST* ast){

}

void IRGenerator::visitBlock(const BlockAST* ast , IRBasicBlock* current_block){
    if(!ast) return;
    if(ast -> stmt){
        if(auto stmt = dynamic_cast<const StmtAST*>(ast ->stmt.get())){
            visitStmt(stmt ,current_block);
        }
    }
}

void IRGenerator::visitStmt(const StmtAST* ast , IRBasicBlock* current_block){
    if(!ast)return;
    if(ast ->retrn == "return" && ast->number){
        if(auto num = dynamic_cast<const NumberAST*>(ast->number.get())){
            auto ir_value = std::make_unique<ReturnIRValue>();
            current_block->ADD_Value(std::move(ir_value));
            visitNumber(num ,current_block);
        }

    }
}

void IRGenerator::visitNumber(const NumberAST* ast,IRBasicBlock* current_block){
    if(!ast)return;
    
    auto int_value = std::make_unique<IntegerIRValue>();
    int_value->value = ast ->int_const;
    current_block ->ADD_Value(std::move(int_value));
}

std::unique_ptr<IRProgram> IRGenerator::get_irprogram(){
    return std::move(program);
}