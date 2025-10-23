#include "../include/include.h"
#include "../include/ast.h"
#include "../include/Ir.h"
#include "../include/Ir_generator.h"

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
    if(ast ->lorexp){
        if(auto lorexp = dynamic_cast<LOrExpAST*>(ast ->lorexp.get())){
           return visitLOrExp(lorexp,current_block);
        }
    }
    return nullptr;
}


std::unique_ptr<BaseIRValue> IRGenerator::visitLOrExp(const LOrExpAST* ast, IRBasicBlock* current_block){
    if(!ast)return nullptr;
    if(ast ->type == LOrExpAST::LANDEXP){
        if(auto land = dynamic_cast<LAndExpAST*>(ast -> landexp.get())){
            return visitLAndExp(land , current_block);
        }
    }
    else if(ast -> type == LOrExpAST::LORLAND){
        if(auto lor = dynamic_cast<LOrExpAST*>(ast -> lorexp.get())){
            if(auto land = dynamic_cast<LAndExpAST*>(ast -> landexp.get())){
                auto orval = std::make_unique<BinaryIRValue>();

                auto ltp = std::make_unique<BinaryIRValue>();
                ltp -> left = visitLOrExp(lor , current_block);
                ltp -> right = std::make_unique<IntegerIRValue>(0);
                ltp -> operation = BinaryIRValue::NE;
                ltp -> result_name = generate_temp_name();

                auto tp_left = std::make_unique<TemporaryIRValue>();
                tp_left -> temp_name = ltp ->result_name;

                current_block ->ADD_Value(std::move(ltp));


                auto rtp = std::make_unique<BinaryIRValue>();
                rtp -> left = visitLAndExp(land , current_block);
                rtp -> right = std::make_unique<IntegerIRValue>(0);
                rtp -> operation = BinaryIRValue::NE;
                rtp -> result_name = generate_temp_name();

                auto tp_right = std::make_unique<TemporaryIRValue>();
                tp_right -> temp_name = rtp ->result_name;

                current_block ->ADD_Value(std::move(rtp));



                orval -> left = std::move(tp_left);
                orval -> right = std::move(tp_right);
                orval -> operation = BinaryIRValue::OR;
                orval -> result_name = generate_temp_name();

                auto temp_name = std::make_unique<TemporaryIRValue>();
                temp_name ->temp_name = orval -> result_name;

                current_block ->ADD_Value(std::move(orval));

                return std::move(temp_name);
            }
        }
    }
    return nullptr;
}


std::unique_ptr<BaseIRValue> IRGenerator::visitLAndExp(const LAndExpAST* ast, IRBasicBlock* current_block){
    if(!ast)return nullptr;
    if(ast ->type == LAndExpAST::EQEXP){
        if(auto eq = dynamic_cast<EqExpAST*>(ast -> eqexp.get())){
            return visitEqExp(eq , current_block);
        }
    }
    else if(ast -> type == LAndExpAST::LANDEQ){
        if(auto land = dynamic_cast<LAndExpAST*>(ast -> landexp.get())){
            if(auto eq = dynamic_cast<EqExpAST*>(ast -> eqexp.get())){
                auto andval = std::make_unique<BinaryIRValue>();

                auto ltp = std::make_unique<BinaryIRValue>();
                ltp -> left = visitLAndExp(land , current_block);
                ltp -> right = std::make_unique<IntegerIRValue>(0);
                ltp -> operation = BinaryIRValue::NE;
                ltp -> result_name = generate_temp_name();

                auto tp_left = std::make_unique<TemporaryIRValue>();
                tp_left -> temp_name = ltp ->result_name;

                current_block ->ADD_Value(std::move(ltp));


                auto rtp = std::make_unique<BinaryIRValue>();
                rtp -> left = visitEqExp(eq , current_block);
                rtp -> right = std::make_unique<IntegerIRValue>(0);
                rtp -> operation = BinaryIRValue::NE;
                rtp -> result_name = generate_temp_name();

                auto tp_right = std::make_unique<TemporaryIRValue>();
                tp_right -> temp_name = rtp ->result_name;

                current_block ->ADD_Value(std::move(rtp));



                andval -> right = std::move(tp_left);
                andval -> left = std::move(tp_right);
                andval -> operation = BinaryIRValue::AND;
                andval -> result_name = generate_temp_name();

                auto temp_name = std::make_unique<TemporaryIRValue>();
                temp_name ->temp_name = andval -> result_name;

                current_block ->ADD_Value(std::move(andval));

                return std::move(temp_name);
            }
        }
    }
    return nullptr;
}

std::unique_ptr<BaseIRValue> IRGenerator::visitEqExp(const EqExpAST* ast, IRBasicBlock* current_block){
    if(!ast) return nullptr;
    if(ast -> type == EqExpAST::RELEXP){
        if(auto rel = dynamic_cast<RelExpAST*>(ast ->relexp.get())){
            return visitRelExp(rel ,current_block);
        }
    }
    else if(ast -> type == EqExpAST::EQREL){
        if(auto eq = dynamic_cast<EqExpAST*>(ast ->eqexp.get())){
            if(auto rel = dynamic_cast<RelExpAST*>(ast -> relexp.get())){
                auto eqval = std::make_unique<BinaryIRValue>();

                eqval -> left = visitEqExp(eq , current_block);
                eqval -> right = visitRelExp(rel , current_block);
                if(ast -> op == "=="){
                    eqval ->operation = BinaryIRValue::EQ;
                }
                else if(ast -> op == "!="){
                    eqval ->operation = BinaryIRValue::NE;
                }
                eqval -> result_name = generate_temp_name();

                auto temp_name = std::make_unique<TemporaryIRValue>();
                temp_name -> temp_name = eqval -> result_name;

                current_block->ADD_Value(std::move(eqval));

                return std::move(temp_name);

            }
        }
    }
    return nullptr;
}



std::unique_ptr<BaseIRValue> IRGenerator::visitRelExp(const RelExpAST* ast, IRBasicBlock* current_block){
    if(!ast) return nullptr;
    if(ast -> type == RelExpAST::ADDEXP){
        if(auto add = dynamic_cast<AddExpAST*>(ast ->addexp.get())){
            return visitAddExp(add ,current_block);
        }
    }
    else if(ast -> type == RelExpAST::RELADD){
        if(auto rel = dynamic_cast<RelExpAST*>(ast ->relexp.get())){
            if(auto add = dynamic_cast<AddExpAST*>(ast -> addexp.get())){
                auto relval = std::make_unique<BinaryIRValue>();

                relval -> left = visitRelExp(rel , current_block);
                relval -> right = visitAddExp(add , current_block);
                if(ast -> op == ">"){
                    relval ->operation = BinaryIRValue::GT;
                }
                else if(ast -> op == "<"){
                    relval ->operation = BinaryIRValue::LT;
                }
                else if(ast -> op == "<="){
                    relval ->operation = BinaryIRValue::LE;
                }
                else if(ast -> op == ">="){
                    relval ->operation = BinaryIRValue::GE;
                }


                relval -> result_name = generate_temp_name();

                auto temp_name = std::make_unique<TemporaryIRValue>();
                temp_name -> temp_name = relval -> result_name;

                current_block->ADD_Value(std::move(relval));

                return std::move(temp_name);

            }
        }
    }
    return nullptr;
}





std::unique_ptr<BaseIRValue> IRGenerator::visitMulExp(const MulExpAST* ast , IRBasicBlock* current_block){
    if(!ast) return nullptr;
    if(ast ->type == MulExpAST::UNARYEXP){
        if(auto unaryexp = dynamic_cast<UnaryExpAST*>(ast -> unrayexp.get())){
            return visitUnaryExp(unaryexp , current_block);
        }
    }
    else if(ast ->type == MulExpAST::MULOPUNRAY){
        if(auto mulexp = dynamic_cast<MulExpAST*>(ast->mulexp.get())){
            if(auto unrayexp = dynamic_cast<UnaryExpAST*>(ast ->unrayexp.get())){
                if(ast ->op == "*"){
                    auto mulval = std::make_unique<BinaryIRValue>();
                    mulval -> operation = BinaryIRValue::MUL;
                    mulval -> left = visitMulExp(mulexp , current_block);
                    mulval -> right = visitUnaryExp(unrayexp , current_block);
                    mulval -> result_name = generate_temp_name();

                    auto temp_value = std::make_unique<TemporaryIRValue>();
                    temp_value -> temp_name = mulval -> result_name;

                    current_block -> ADD_Value(std::move(mulval));

                    return std::move(temp_value);
                }
                else if(ast -> op == "/"){
                    auto divval = std::make_unique<BinaryIRValue>();
                    divval -> operation = BinaryIRValue::DIV;
                    divval -> left = visitMulExp(mulexp , current_block);
                    divval -> right = visitUnaryExp(unrayexp , current_block);
                    divval -> result_name = generate_temp_name();

                    auto temp_value = std::make_unique<TemporaryIRValue>();
                    temp_value -> temp_name = divval -> result_name;

                    current_block -> ADD_Value(std::move(divval));

                    return std::move(temp_value);       
                }
                else if(ast -> op == "%"){
                    auto modval = std::make_unique<BinaryIRValue>();
                    modval -> operation = BinaryIRValue::MOD;
                    modval -> left = visitMulExp(mulexp , current_block);
                    modval -> right = visitUnaryExp(unrayexp , current_block);
                    modval -> result_name = generate_temp_name();

                    auto temp_value = std::make_unique<TemporaryIRValue>();
                    temp_value -> temp_name = modval -> result_name;

                    current_block -> ADD_Value(std::move(modval));

                    return std::move(temp_value);      
                }
            } 
        }
    }
    return nullptr;

}

std::unique_ptr<BaseIRValue> IRGenerator::visitAddExp(const AddExpAST* ast, IRBasicBlock* current_block){
    if(!ast) return nullptr;
    if(ast ->type == AddExpAST::MULONLY){
        if(auto mulexp = dynamic_cast<MulExpAST*>(ast->mulexp.get())){
            return visitMulExp(mulexp , current_block);
        }
    }
    else if (ast -> type == AddExpAST::ADDOPMUL){
        if(auto addexp = dynamic_cast<AddExpAST*>(ast ->addexp.get())){
            if(auto mulexp = dynamic_cast<MulExpAST*>(ast -> mulexp.get())){
                if(ast ->op == "+"){
                    auto addval = std::make_unique<BinaryIRValue>();
                    
                    addval ->operation = BinaryIRValue::ADD;
                    addval ->left = visitAddExp(addexp , current_block);
                    addval ->right = visitMulExp(mulexp , current_block);
                    addval ->result_name = generate_temp_name();

                    auto temp_value = std::make_unique<TemporaryIRValue>();
                    temp_value -> temp_name = addval -> result_name;

                    current_block ->ADD_Value(std::move(addval));

                    return std::move(temp_value);

                }
                else if(ast ->op == "-"){
                    auto subval = std::make_unique<BinaryIRValue>();
                    
                    subval ->operation = BinaryIRValue::SUB;
                    subval ->left = visitAddExp(addexp , current_block);
                    subval ->right = visitMulExp(mulexp , current_block);
                    subval ->result_name = generate_temp_name();

                    auto temp_value = std::make_unique<TemporaryIRValue>();
                    temp_value -> temp_name = subval -> result_name;

                    current_block ->ADD_Value(std::move(subval));

                    return std::move(temp_value);

                }
            }
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