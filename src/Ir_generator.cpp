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
           auto result = visitExp(exp , current_block);
           auto ret_ir = std::make_unique<ReturnIRValue>();
           ret_ir ->return_value = std::move(result);
           current_block -> ADD_Value(std::move(ret_ir)); 
        }

    }
}

std::unique_ptr<BaseIRValue> IRGenerator::visitExp(const ExpAST* ast , IRBasicBlock* current_block){
    if(!ast)return nullptr;
    if(ast ->addexp){
        if(auto addexp = dynamic_cast<UnaryExpAST*>(ast ->addexp.get())){
           return visitUnaryExp(addexp,current_block);
        }
    }
    return nullptr;
}

std::unique_ptr<BaseIRValue> IRGenerator::visitUnaryExp(const UnaryExpAST* ast, IRBasicBlock* current_block){
    if(!ast) return nullptr;
    if(ast ->type == UnaryExpAST::PRIMARYEXP){
        if(auto primary = dynamic_cast<PrimaryExpAST*>(ast->primaryexp.get())){
            return visitPrimaryExp(primary , current_block);
        }   
    }
    else if(ast ->type == UnaryExpAST::UNARYEXP){
        if(auto unaryexp = dynamic_cast<UnaryExpAST*>(ast ->unaryexp.get())){
            if(ast ->unaryop == "+"){
                return visitUnaryExp(unaryexp , current_block);
            }
            else if(ast -> unaryop == "-"){
                auto zero = std::make_unique<IntegerIRValue>();
                zero ->value = 0;

                auto subval = std::make_unique<BinaryIRValue>();
                subval -> operation = BinaryIRValue::SUB;
                subval -> left = std::move(zero);
                subval -> right = visitUnaryExp(unaryexp , current_block);
                subval -> result_name = generate_temp_name();
                
                auto temp_name = std::make_unique<TemporaryIRValue>();
                temp_name ->temp_name = subval ->result_name;
               
                current_block->ADD_Value(std::move(subval));

                return std::move(temp_name);
            }
            else if(ast -> unaryop == "!"){
                auto zero = std::make_unique<IntegerIRValue>();
                zero ->value = 0;

                auto eqval = std::make_unique<BinaryIRValue>();
                eqval -> operation = BinaryIRValue::EQ;
                eqval -> left = visitUnaryExp(unaryexp , current_block);
                eqval -> right = std::move(zero);
                eqval -> result_name = generate_temp_name();

                auto temp_name = std::make_unique<TemporaryIRValue>() ;

                temp_name ->temp_name = eqval ->result_name;

                current_block ->ADD_Value(std::move(eqval));

                return std::move(temp_name);
            }
        }
    }
    return nullptr;
}

std::unique_ptr<BaseIRValue> IRGenerator::visitPrimaryExp(const PrimaryExpAST* ast, IRBasicBlock* current_block){
    if(!ast) return nullptr;
    if(ast -> type == PrimaryExpAST::EXP){
        if(auto exp = dynamic_cast<ExpAST*>(ast->exp.get())){
            return visitExp(exp , current_block);
        }
    }
    else if(ast-> type == PrimaryExpAST::NUMBER){
        if(auto number = dynamic_cast<NumberAST*>(ast->number.get())){
            return visitNumber(number , current_block);
        } 
    }
    return nullptr;
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