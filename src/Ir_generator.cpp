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
    ir_fun -> function_name = (ast -> ident);
    
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
    if(ast ->retrn == "return" && ast->exp){
        if(auto exp = dynamic_cast<const ExpAST*>(ast->exp.get())){
           visitExp(exp , current_block); 
        }

    }
}

std::unique_ptr<BaseIRValue> IRGenerator::visitExp(const ExpAST* ast , IRBasicBlock* current_block){
    if(!ast)return;
    if(ast ->unaryexp){
        if(auto unaryexp = dynamic_cast<UnaryExpAST*>(ast ->unaryexp.get())){
            visitUnaryExp(unaryexp,current_block);
        }
    }
}

std::unique_ptr<BaseIRValue> IRGenerator::visitUnaryExp(const UnaryExpAST* ast, IRBasicBlock* current_block){
    if(!ast) return;
    if(ast ->PRIMARYEXP){
        if(auto primary = dynamic_cast<PrimaryExpAST*>(ast->primaryexp.get())){
            visitPrimaryExp(primary , current_block);
        }   
    }
    else if(ast ->UNARYEXP){
        if(auto unaryexp = dynamic_cast<UnaryExpAST*>(ast ->unaryexp.get())){
            visitUnaryExp(unaryexp , current_block);
        }
    }
}

std::unique_ptr<BaseIRValue> IRGenerator::visitPrimaryExp(const PrimaryExpAST* ast, IRBasicBlock* current_block){
    if(!ast) return;
    if(ast -> type == PrimaryExpAST::EXP){

    }
    else if(ast-> type == PrimaryExpAST::NUMBER){
        if(auto number = dynamic_cast<NumberAST*>(ast->number.get())){
            return visitNumber(number , current_block);
        } 
    }
}

std::unique_ptr<BaseIRValue> IRGenerator::visitUnaryOp(const UnaryOpAST* ast,IRBasicBlock* current_block){


}




std::unique_ptr<BaseIRValue> IRGenerator::visitNumber(const NumberAST* ast,IRBasicBlock* current_block){
    if(!ast) return nullptr;

    auto int_value = std::make_unique<IntegerIRValue>();
    int_value->value = ast ->int_const;
    return std::move(int_value);
}

std::unique_ptr<IRProgram> IRGenerator::get_irprogram(){
    return std::move(program);
}