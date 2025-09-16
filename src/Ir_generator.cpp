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

    if (ast ->block){
        if(auto block = dynamic_cast<const BlockAST*>(ast->block.get())){
            visitBlock(block);
        }
    }
}

void IRGenerator::visitFunType(const FunTypeAST* ast){

}

void IRGenerator::visitBlock(const BlockAST* ast){
    if(!ast) return;
    blockcount++;
    
}