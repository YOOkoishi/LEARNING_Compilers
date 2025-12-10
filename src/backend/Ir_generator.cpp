#include "../include/include.h"
#include "../include/ast.h"
#include "../include/Ir.h"
#include "../include/Ir_generator.h"

int IRGenerator::blockcount = 0;


IRGenerator::IRGenerator() 
    : program(std::make_unique<IRProgram>()), 
      symbol_table(std::make_unique<SymbolTable>()) {
    ctx.program = program.get();
    ctx.symbol_table = symbol_table.get();
    ctx.current_block = nullptr;
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
            ir_fun ->functype = ((fun_type ->type) == FunTypeAST::INT)? "i32":"other";
        }
    }

    program ->ADD_Function(std::move(ir_fun));

    IRFunction* current_func = program->ir_function.back().get();

    auto entry_block = std::make_unique<IRBasicBlock>();
    entry_block->block_name = "%entry" + std::to_string(blockcount++);
    
    current_func ->ADD_Block(std::move(entry_block));
    
    IRBasicBlock* current_block = current_func ->ir_basicblock.back().get();
    
    setCurrentBlock(current_block);

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
    
    ctx.symbol_table->enterScope();
    
    if(ast -> blockitems){
        if(auto blockitems = dynamic_cast<const BlockItemsAST*>(ast ->blockitems.get())){
            visitBlockItems(blockitems);
        }
    }
    
    ctx.symbol_table->exitScope();
}




void IRGenerator::visitBlockItems(const BlockItemsAST* ast){
    if(!ast) return;

    for(const auto& item : ast->item){
        // Check if current block is terminated (dead code elimination)
        if (ctx.current_block && !ctx.current_block->ir_value.empty()) {
            auto last_inst = ctx.current_block->ir_value.back().get();
            if (dynamic_cast<ReturnIRValue*>(last_inst) ||
                dynamic_cast<JumpIRValue*>(last_inst) ||
                dynamic_cast<BranchIRValue*>(last_inst)) {
                continue; // Skip unreachable code
            }
        }

        if(auto blockitem = dynamic_cast<BlockItemAST*>(item.get())){
            visitBlockItem(blockitem);
        }
    }
}

void IRGenerator::visitBlockItem(const BlockItemAST* ast){
    if(!ast) return;

    if(ast ->type == BlockItemAST::DECL){
        if(auto decl = dynamic_cast<DeclAST*>(ast -> decl.get())){
            visitDecl(decl);
        }
    }
    else if(ast -> type == BlockItemAST::STMT){
        if(auto stmt = dynamic_cast<StmtAST*>(ast -> stmt.get())){
            visitStmt(stmt);
        }
    }
}



void IRGenerator::visitDecl(const DeclAST* ast){
    if(!ast) return;
    if(ast->type == DeclAST::CONST){
        if(auto constdecl = dynamic_cast<ConstDeclAST*>(ast->constdecl.get())){
            visitConstDecl(constdecl);
        }
    }
    else {
        if(auto vardecl = dynamic_cast<VarDeclAST*>(ast -> vardecl.get())){
            visitVarDecl(vardecl);
        }
    }

}


void IRGenerator::visitStmt(const StmtAST* ast){
    if(!ast) return;
    if( ast->type ==  StmtAST::RETURNEXP) {
        // return exp;
        if(auto exp = dynamic_cast<const ExpAST*>(ast->exp.get())){
            auto result = visitExp(exp);
            auto ret_ir = std::make_unique<ReturnIRValue>(ReturnIRValue::VALUE);
            ret_ir->return_value = std::move(result);
            ctx.current_block->ADD_Value(std::move(ret_ir));
        }
    }
    
    else if(ast->type == StmtAST::LVALEXP){
        // lval = exp;
        auto lval = dynamic_cast<LValAST*>(ast->lval.get());
        auto exp = dynamic_cast<ExpAST*>(ast->exp.get());
        
        if(!lval || !exp) {
            std::cerr << "Error: Invalid assignment statement" << std::endl;
            return;
        }
        
        // 1. 查找变量是否存在
        auto symbol = ctx.symbol_table->lookup(lval->ident);
        if(!symbol) {
            std::cerr << "Error: Undefined variable '" << lval->ident << "'" << std::endl;
            return;
        }
        
        // 2. 检查是否为常量（常量不能被赋值）
        if(symbol->type == SymbolType::CONST) {
            std::cerr << "Error: Cannot assign to constant '" << lval->ident << "'" << std::endl;
            return;
        }
        
        // 3. 计算右侧表达式的值
        auto rhs_value = visitExp(exp);
        
        // 4. 生成 store 指令，使用 symbol->ir_name
        auto store_ir = std::make_unique<StoreIRValue>(std::move(rhs_value), symbol->ir_name);
        ctx.current_block->ADD_Value(std::move(store_ir));
        
    }

    else if(ast -> type == StmtAST::BLOCK){
        if(auto next_block = dynamic_cast<BlockAST*>(ast->block.get())){
            visitBlock(next_block);
        }
    }

    else if(ast -> type == StmtAST::EXP){
        if(auto exp = dynamic_cast<ExpAST*>(ast->exp.get())){
            visitExp(exp);
        }
    }

    else if(ast -> type == StmtAST::RETURNNULL){
        auto ret = std::make_unique<ReturnIRValue>(ReturnIRValue::NUL);
        ctx.current_block->ADD_Value(std::move(ret));
    }

    else if(ast -> type == StmtAST::IF){
        // if (cond) stmt
        
        // 1. Create blocks
        auto then_block = std::make_unique<IRBasicBlock>();
        then_block->block_name = "%then" + std::to_string(blockcount++);
        std::string then_label = then_block->block_name;

        auto end_block = std::make_unique<IRBasicBlock>();
        end_block->block_name = "%end" + std::to_string(blockcount++);
        std::string end_label = end_block->block_name;

        // 2. Evaluate condition
        auto cond = visitExp(dynamic_cast<ExpAST*>(ast->exp.get()));
        
        // 3. Branch
        auto br = std::make_unique<BranchIRValue>(std::move(cond), then_label, end_label);
        ctx.current_block->ADD_Value(std::move(br));

        // 4. Visit Then Block
        auto current_func = ctx.program->ir_function.back().get();
        current_func->ADD_Block(std::move(then_block));
        setCurrentBlock(current_func->ir_basicblock.back().get());
        
        if(auto stmt = dynamic_cast<StmtAST*>(ast->if_stmt.get())){
            visitStmt(stmt);
        }
        
        // ✅ 安全检查：检查是否需要跳转
        bool need_jump = true;
        if (!ctx.current_block->ir_value.empty()) {
            auto last_value = ctx.current_block->ir_value.back().get();
            if (dynamic_cast<ReturnIRValue*>(last_value) || 
                dynamic_cast<JumpIRValue*>(last_value) || 
                dynamic_cast<BranchIRValue*>(last_value)) {
                need_jump = false;
            }
        }
        
        if(need_jump){
            auto jump_end = std::make_unique<JumpIRValue>(end_label);
            ctx.current_block->ADD_Value(std::move(jump_end));
        }

        // 5. Visit End Block
        current_func->ADD_Block(std::move(end_block));
        setCurrentBlock(current_func->ir_basicblock.back().get());
    }

    else if(ast -> type == StmtAST::IFELSE){
        // if (cond) stmt else elsestmt

        // 1. Create blocks
        auto then_block = std::make_unique<IRBasicBlock>();
        then_block->block_name = "%then" + std::to_string(blockcount++);
        std::string then_label = then_block->block_name;

        auto else_block = std::make_unique<IRBasicBlock>();
        else_block->block_name = "%else" + std::to_string(blockcount++);
        std::string else_label = else_block->block_name;

        auto end_block = std::make_unique<IRBasicBlock>();
        end_block->block_name = "%end" + std::to_string(blockcount++);
        std::string end_label = end_block->block_name;

        // 2. Evaluate condition
        auto cond = visitExp(dynamic_cast<ExpAST*>(ast->exp.get()));

        // 3. Branch
        auto br = std::make_unique<BranchIRValue>(std::move(cond), then_label, else_label);
        ctx.current_block->ADD_Value(std::move(br));

        auto current_func = ctx.program->ir_function.back().get();

        // 4. Visit Then Block
        current_func->ADD_Block(std::move(then_block));
        setCurrentBlock(current_func->ir_basicblock.back().get());
        
        if(auto stmt = dynamic_cast<StmtAST*>(ast->if_stmt.get())){
            visitStmt(stmt);
        }
        
        // ✅ 安全检查 Then 块
        bool need_jump_if = true;
        if (!ctx.current_block->ir_value.empty()) {
            auto last_value = ctx.current_block->ir_value.back().get();
            if (dynamic_cast<ReturnIRValue*>(last_value) || 
                dynamic_cast<JumpIRValue*>(last_value) || 
                dynamic_cast<BranchIRValue*>(last_value)) {
                need_jump_if = false;
            }
        }

        if(need_jump_if){
            auto jump_end_1 = std::make_unique<JumpIRValue>(end_label);
            ctx.current_block->ADD_Value(std::move(jump_end_1));
        }

        // 5. Visit Else Block
        current_func->ADD_Block(std::move(else_block));
        setCurrentBlock(current_func->ir_basicblock.back().get());

        if(auto stmt = dynamic_cast<StmtAST*>(ast->else_stmt.get())){
            visitStmt(stmt);
        }
         
        // ✅ 安全检查 Else 块
        bool need_jump_else = true;
        if (!ctx.current_block->ir_value.empty()) {
            auto last_value = ctx.current_block->ir_value.back().get();
            if (dynamic_cast<ReturnIRValue*>(last_value) || 
                dynamic_cast<JumpIRValue*>(last_value) || 
                dynamic_cast<BranchIRValue*>(last_value)) {
                need_jump_else = false;
            }
        }

        if(need_jump_else){
            auto jump_end_2 = std::make_unique<JumpIRValue>(end_label);
            ctx.current_block->ADD_Value(std::move(jump_end_2));
        }

        // 6. Visit End Block
        current_func->ADD_Block(std::move(end_block));
        setCurrentBlock(current_func->ir_basicblock.back().get());
    }
    else if(ast -> type == StmtAST::WHILE){
        
        //first step : make block
        auto while_entry_block = std::make_unique<IRBasicBlock>();
        while_entry_block ->block_name = "%while_entry" + std::to_string(blockcount++);
        std::string while_entry_label = while_entry_block ->block_name ;

        auto while_body_block = std::make_unique<IRBasicBlock>();
        while_body_block -> block_name = "%while_body" + std::to_string(blockcount++);
        std::string while_body_label = while_body_block -> block_name;
 
        auto end_block = std::make_unique<IRBasicBlock>();
        end_block -> block_name = "%end" + std::to_string(blockcount++);
        std::string end_label = end_block -> block_name;
              
        ctx.loop_stack.push_back({while_entry_label,end_label});
        //second step: jump        

        auto jump_while_entry = std::make_unique<JumpIRValue>(while_entry_label);
        ctx.current_block ->ADD_Value(std::move(jump_while_entry));
        
        auto current_func = ctx.program -> ir_function.back().get();
        
        current_func ->ADD_Block(std::move(while_entry_block));
        
        setCurrentBlock(current_func->ir_basicblock.back().get());
        
        //third step: let's go while entry block
        auto cond = visitExp(dynamic_cast<ExpAST*>(ast->exp.get()));
    
        auto br = std::make_unique<BranchIRValue>(std::move(cond), while_body_label , end_label);    
        ctx.current_block ->ADD_Value(std::move(br));

        
        //fourth step: go while body block 
        
        current_func ->ADD_Block(std::move(while_body_block));
        setCurrentBlock(current_func->ir_basicblock.back().get());
        
        if(auto stmt = dynamic_cast<StmtAST*>(ast->while_stmt.get())){
            visitStmt(stmt);
        }
        
        bool need_jump_while = true;

        if(!ctx.current_block->ir_value.empty()){
            auto last_value = ctx.current_block -> ir_value.back().get();
            if(dynamic_cast<ReturnIRValue*>(last_value) ||
            dynamic_cast<JumpIRValue*>(last_value)||
            dynamic_cast<BranchIRValue*>(last_value)) {
                need_jump_while = false;
            }
        }

        if(need_jump_while){
            auto jump_while_entry2 = std::make_unique<JumpIRValue>(while_entry_label);
            ctx.current_block ->ADD_Value(std::move(jump_while_entry2));
        }

        ctx.loop_stack.pop_back();
        //fifth step: go end

        current_func->ADD_Block(std::move(end_block));
        setCurrentBlock(current_func->ir_basicblock.back().get());
    }


    else if(ast-> type == StmtAST::BREAK){
        if(ctx.loop_stack.empty()){
            std::cerr << "Error : break statement not within loop" << std::endl;
            return;
        }

        std::string target = ctx.loop_stack.back().end_label;

        auto jump = std::make_unique<JumpIRValue>(target);
        ctx.current_block->ADD_Value(std::move(jump));

    }
    else if(ast->type == StmtAST::CONTINUE){
         if(ctx.loop_stack.empty()){
            std::cerr << "Error : continue statement not within loop" << std::endl;
            return;
        }

        std::string target = ctx.loop_stack.back().entry_label;

        auto jump = std::make_unique<JumpIRValue>(target);
        ctx.current_block->ADD_Value(std::move(jump));
        
    }
}



/*



下面是常量相关内容



*/





void IRGenerator::visitConstDecl(const ConstDeclAST* ast){
    if(!ast) return;
    if(auto constdefs = dynamic_cast<ConstDefsAST*>( ast->constdefs.get())){
        visitConstDefs(constdefs);
    }
}



void IRGenerator::visitConstDefs(const ConstDefsAST* ast){
    if(!ast) return;
    for(const auto& x : ast -> constdef){
           if(auto constdef = dynamic_cast<ConstDefAST*>(x.get())){
                visitConstDef(constdef);
           }
    }
}

void IRGenerator::visitConstDef(const ConstDefAST* ast){
    if(!ast) return ;
    
    int cosnt_value = evaluateConstExp(ast->constinitval.get());

    try
    {
        ctx.symbol_table->declare(
            ast->ident,
            SymbolType::CONST,
            DataType::INT,
            cosnt_value
        );
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << "Error in constant declaration" << e.what() << '\n';
    }
    

}

/*

void IRGenerator::visitConstInitVal(const ConstInitValAST* ast){
    if(!ast) return;
    if(auto constexp = dynamic_cast<ConstExpAST*>(ast->constexp.get())){
        visitConstExp(constexp);
    }
}



std::unique_ptr<BaseIRValue> IRGenerator::visitConstExp(const ConstExpAST* ast){
    
if(!ast) return;
if(auto exp = dynamic_cast<ExpAST*>(ast->exp.get())){
    visitExp(exp);
}
}


*/




void IRGenerator::visitVarDecl(const VarDeclAST* ast){
    if(!ast) return ;
    if(auto vardefs = dynamic_cast<VarDefsAST*>(ast ->vardefs.get())){
        visitVarDefs(vardefs);
    }
}





void IRGenerator::visitVarDefs(const VarDefsAST* ast){
    if(!ast) return;
    for(const auto& x : ast -> vardef){
           if(auto vardef = dynamic_cast<VarDefAST*>(x.get())){
                visitVarDef(vardef);
           }
    }
}




void IRGenerator::visitVarDef(const VarDefAST* ast){
    if(!ast) return;

    std::string var_name;

    if(ast ->type == VarDefAST::IDENT){
        try
        {
            var_name = ctx.symbol_table->declare(
                ast->ident,
                SymbolType::VAR,
                DataType::INT
            );
        }
        catch(const std::runtime_error& e)
        {
            std::cerr << "Error in variable declaration" << e.what() << '\n';
        }
        
        // only allocIR
        auto alloc = std::make_unique<AllocIRValue>(var_name,"i32");
        ctx.current_block ->ADD_Value(std::move(alloc));

    }
    else if(ast ->type == VarDefAST::IDENTDEF){
        try
        {
            var_name = ctx.symbol_table->declare(
                ast->ident,
                SymbolType::VAR,
                DataType::INT
            );
        }
        catch(const std::runtime_error& e)
        {
            std::cerr << "Error in variable declaration" << e.what() << '\n';
        }

        // allocIR
        auto alloc = std::make_unique<AllocIRValue>(var_name,"i32");

        ctx.current_block ->ADD_Value(std::move(alloc));    
        
        
        //storeIR 

        if(auto initval = dynamic_cast<InitValAST*>(ast->initval.get())){
            if(auto exp = dynamic_cast<ExpAST*>(initval ->exp.get())){
                auto var_value = visitExp(exp);
                auto store = std::make_unique<StoreIRValue>(std::move(var_value),var_name);

                ctx.current_block->ADD_Value(std::move(store));
            }
        }
    }
}








/*




下面是运算表达式




*/




std::unique_ptr<BaseIRValue> IRGenerator::visitExp(const ExpAST* ast){
    if(!ast)return nullptr;
    if(ast ->lorexp){
        if(auto lorexp = dynamic_cast<LOrExpAST*>(ast ->lorexp.get())){
           return visitLOrExp(lorexp);
        }
    }
    return nullptr;
}


std::unique_ptr<BaseIRValue> IRGenerator::visitLOrExp(const LOrExpAST* ast){
    if(!ast)return nullptr;
    if(ast ->type == LOrExpAST::LANDEXP){
        if(auto land = dynamic_cast<LAndExpAST*>(ast -> landexp.get())){
            return visitLAndExp(land);
        }
    }
    else if(ast -> type == LOrExpAST::LORLAND){
        if(auto lor = dynamic_cast<LOrExpAST*>(ast -> lorexp.get())){
            if(auto land = dynamic_cast<LAndExpAST*>(ast -> landexp.get())){
                auto orval = std::make_unique<BinaryIRValue>();

                auto ltp = std::make_unique<BinaryIRValue>();
                ltp -> left = visitLOrExp(lor);
                ltp -> right = std::make_unique<IntegerIRValue>(0);
                ltp -> operation = BinaryIRValue::NE;
                ltp -> result_name = generate_temp_name();

                auto tp_left = std::make_unique<TemporaryIRValue>();
                tp_left -> temp_name = ltp ->result_name;

                ctx.current_block->ADD_Value(std::move(ltp));


                auto rtp = std::make_unique<BinaryIRValue>();
                rtp -> left = visitLAndExp(land);
                rtp -> right = std::make_unique<IntegerIRValue>(0);
                rtp -> operation = BinaryIRValue::NE;
                rtp -> result_name = generate_temp_name();

                auto tp_right = std::make_unique<TemporaryIRValue>();
                tp_right -> temp_name = rtp ->result_name;

                ctx.current_block->ADD_Value(std::move(rtp));



                orval -> left = std::move(tp_left);
                orval -> right = std::move(tp_right);
                orval -> operation = BinaryIRValue::OR;
                orval -> result_name = generate_temp_name();

                auto temp_name = std::make_unique<TemporaryIRValue>();
                temp_name ->temp_name = orval -> result_name;

                ctx.current_block->ADD_Value(std::move(orval));

                return std::move(temp_name);
            }
        }
    }
    return nullptr;
}


std::unique_ptr<BaseIRValue> IRGenerator::visitLAndExp(const LAndExpAST* ast){
    if(!ast)return nullptr;
    if(ast ->type == LAndExpAST::EQEXP){
        if(auto eq = dynamic_cast<EqExpAST*>(ast -> eqexp.get())){
            return visitEqExp(eq);
        }
    }
    else if(ast -> type == LAndExpAST::LANDEQ){
        if(auto land = dynamic_cast<LAndExpAST*>(ast -> landexp.get())){
            if(auto eq = dynamic_cast<EqExpAST*>(ast -> eqexp.get())){
                auto andval = std::make_unique<BinaryIRValue>();

                auto ltp = std::make_unique<BinaryIRValue>();
                ltp -> left = visitLAndExp(land);
                ltp -> right = std::make_unique<IntegerIRValue>(0);
                ltp -> operation = BinaryIRValue::NE;
                ltp -> result_name = generate_temp_name();

                auto tp_left = std::make_unique<TemporaryIRValue>();
                tp_left -> temp_name = ltp ->result_name;

                ctx.current_block->ADD_Value(std::move(ltp));


                auto rtp = std::make_unique<BinaryIRValue>();
                rtp -> left = visitEqExp(eq);
                rtp -> right = std::make_unique<IntegerIRValue>(0);
                rtp -> operation = BinaryIRValue::NE;
                rtp -> result_name = generate_temp_name();

                auto tp_right = std::make_unique<TemporaryIRValue>();
                tp_right -> temp_name = rtp ->result_name;

                ctx.current_block->ADD_Value(std::move(rtp));



                andval -> right = std::move(tp_left);
                andval -> left = std::move(tp_right);
                andval -> operation = BinaryIRValue::AND;
                andval -> result_name = generate_temp_name();

                auto temp_name = std::make_unique<TemporaryIRValue>();
                temp_name ->temp_name = andval -> result_name;

                ctx.current_block->ADD_Value(std::move(andval));

                return std::move(temp_name);
            }
        }
    }
    return nullptr;
}

std::unique_ptr<BaseIRValue> IRGenerator::visitEqExp(const EqExpAST* ast){
    if(!ast) return nullptr;
    if(ast -> type == EqExpAST::RELEXP){
        if(auto rel = dynamic_cast<RelExpAST*>(ast ->relexp.get())){
            return visitRelExp(rel);
        }
    }
    else if(ast -> type == EqExpAST::EQREL){
        if(auto eq = dynamic_cast<EqExpAST*>(ast ->eqexp.get())){
            if(auto rel = dynamic_cast<RelExpAST*>(ast -> relexp.get())){
                auto eqval = std::make_unique<BinaryIRValue>();

                eqval -> left = visitEqExp(eq);
                eqval -> right = visitRelExp(rel);
                if(ast -> op == "=="){
                    eqval ->operation = BinaryIRValue::EQ;
                }
                else if(ast -> op == "!="){
                    eqval ->operation = BinaryIRValue::NE;
                }
                eqval -> result_name = generate_temp_name();

                auto temp_name = std::make_unique<TemporaryIRValue>();
                temp_name -> temp_name = eqval -> result_name;

                ctx.current_block->ADD_Value(std::move(eqval));

                return std::move(temp_name);

            }
        }
    }
    return nullptr;
}



std::unique_ptr<BaseIRValue> IRGenerator::visitRelExp(const RelExpAST* ast){
    if(!ast) return nullptr;
    if(ast -> type == RelExpAST::ADDEXP){
        if(auto add = dynamic_cast<AddExpAST*>(ast ->addexp.get())){
            return visitAddExp(add);
        }
    }
    else if(ast -> type == RelExpAST::RELADD){
        if(auto rel = dynamic_cast<RelExpAST*>(ast ->relexp.get())){
            if(auto add = dynamic_cast<AddExpAST*>(ast -> addexp.get())){
                auto relval = std::make_unique<BinaryIRValue>();

                relval -> left = visitRelExp(rel);
                relval -> right = visitAddExp(add);
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

                ctx.current_block->ADD_Value(std::move(relval));

                return std::move(temp_name);

            }
        }
    }
    return nullptr;
}





std::unique_ptr<BaseIRValue> IRGenerator::visitMulExp(const MulExpAST* ast){
    if(!ast) return nullptr;
    if(ast ->type == MulExpAST::UNARYEXP){
        if(auto unaryexp = dynamic_cast<UnaryExpAST*>(ast -> unrayexp.get())){
            return visitUnaryExp(unaryexp);
        }
    }
    else if(ast ->type == MulExpAST::MULOPUNRAY){
        if(auto mulexp = dynamic_cast<MulExpAST*>(ast->mulexp.get())){
            if(auto unrayexp = dynamic_cast<UnaryExpAST*>(ast ->unrayexp.get())){
                if(ast ->op == "*"){
                    auto mulval = std::make_unique<BinaryIRValue>();
                    mulval -> operation = BinaryIRValue::MUL;
                    mulval -> left = visitMulExp(mulexp);
                    mulval -> right = visitUnaryExp(unrayexp);
                    mulval -> result_name = generate_temp_name();

                    auto temp_value = std::make_unique<TemporaryIRValue>();
                    temp_value -> temp_name = mulval -> result_name;

                    ctx.current_block->ADD_Value(std::move(mulval));

                    return std::move(temp_value);
                }
                else if(ast -> op == "/"){
                    auto divval = std::make_unique<BinaryIRValue>();
                    divval -> operation = BinaryIRValue::DIV;
                    divval -> left = visitMulExp(mulexp);
                    divval -> right = visitUnaryExp(unrayexp);
                    divval -> result_name = generate_temp_name();

                    auto temp_value = std::make_unique<TemporaryIRValue>();
                    temp_value -> temp_name = divval -> result_name;

                    ctx.current_block->ADD_Value(std::move(divval));

                    return std::move(temp_value);       
                }
                else if(ast -> op == "%"){
                    auto modval = std::make_unique<BinaryIRValue>();
                    modval -> operation = BinaryIRValue::MOD;
                    modval -> left = visitMulExp(mulexp);
                    modval -> right = visitUnaryExp(unrayexp);
                    modval -> result_name = generate_temp_name();

                    auto temp_value = std::make_unique<TemporaryIRValue>();
                    temp_value -> temp_name = modval -> result_name;

                    ctx.current_block->ADD_Value(std::move(modval));

                    return std::move(temp_value);      
                }
            } 
        }
    }
    return nullptr;
}

std::unique_ptr<BaseIRValue> IRGenerator::visitAddExp(const AddExpAST* ast){
    if(!ast) return nullptr;
    if(ast ->type == AddExpAST::MULONLY){
        if(auto mulexp = dynamic_cast<MulExpAST*>(ast->mulexp.get())){
            return visitMulExp(mulexp);
        }
    }
    else if (ast -> type == AddExpAST::ADDOPMUL){
        if(auto addexp = dynamic_cast<AddExpAST*>(ast ->addexp.get())){
            if(auto mulexp = dynamic_cast<MulExpAST*>(ast -> mulexp.get())){
                if(ast ->op == "+"){
                    auto addval = std::make_unique<BinaryIRValue>();
                    
                    addval ->operation = BinaryIRValue::ADD;
                    addval ->left = visitAddExp(addexp);
                    addval ->right = visitMulExp(mulexp);
                    addval ->result_name = generate_temp_name();

                    auto temp_value = std::make_unique<TemporaryIRValue>();
                    temp_value -> temp_name = addval -> result_name;

                    ctx.current_block->ADD_Value(std::move(addval));

                    return std::move(temp_value);

                }
                else if(ast ->op == "-"){
                    auto subval = std::make_unique<BinaryIRValue>();
                    
                    subval ->operation = BinaryIRValue::SUB;
                    subval ->left = visitAddExp(addexp);
                    subval ->right = visitMulExp(mulexp);
                    subval ->result_name = generate_temp_name();

                    auto temp_value = std::make_unique<TemporaryIRValue>();
                    temp_value -> temp_name = subval -> result_name;

                    ctx.current_block->ADD_Value(std::move(subval));

                    return std::move(temp_value);

                }
            }
        }
    }
    return nullptr;
}










std::unique_ptr<BaseIRValue> IRGenerator::visitUnaryExp(const UnaryExpAST* ast){
    if(!ast) return nullptr;
    if(ast ->type == UnaryExpAST::PRIMARYEXP){
        if(auto primary = dynamic_cast<PrimaryExpAST*>(ast->primaryexp.get())){
            return visitPrimaryExp(primary);
        }   
    }
    else if(ast ->type == UnaryExpAST::UNARYEXP){
        if(auto unaryexp = dynamic_cast<UnaryExpAST*>(ast ->unaryexp.get())){
            if(ast ->unaryop == "+"){
                return visitUnaryExp(unaryexp);
            }
            else if(ast -> unaryop == "-"){
                auto zero = std::make_unique<IntegerIRValue>();
                zero ->value = 0;

                auto subval = std::make_unique<BinaryIRValue>();
                subval -> operation = BinaryIRValue::SUB;
                subval -> left = std::move(zero);
                subval -> right = visitUnaryExp(unaryexp);
                subval -> result_name = generate_temp_name();
                
                auto temp_name = std::make_unique<TemporaryIRValue>();
                temp_name ->temp_name = subval ->result_name;
               
                ctx.current_block->ADD_Value(std::move(subval));

                return std::move(temp_name);
            }
            else if(ast -> unaryop == "!"){
                auto zero = std::make_unique<IntegerIRValue>();
                zero ->value = 0;

                auto eqval = std::make_unique<BinaryIRValue>();
                eqval -> operation = BinaryIRValue::EQ;
                eqval -> left = visitUnaryExp(unaryexp);
                eqval -> right = std::move(zero);
                eqval -> result_name = generate_temp_name();

                auto temp_name = std::make_unique<TemporaryIRValue>();
                temp_name ->temp_name = eqval ->result_name;

                ctx.current_block->ADD_Value(std::move(eqval));

                return std::move(temp_name);
            }
        }
    }
    return nullptr;
}


std::unique_ptr<BaseIRValue> IRGenerator::visitPrimaryExp(const PrimaryExpAST* ast){
    if(!ast) return nullptr;
    if(ast -> type == PrimaryExpAST::EXP){
        if(auto exp = dynamic_cast<ExpAST*>(ast->exp.get())){
            return visitExp(exp);
        }
    }
    else if(ast-> type == PrimaryExpAST::NUMBER){
        if(auto number = dynamic_cast<NumberAST*>(ast->number.get())){
            return visitNumber(number);
        } 
    }
    else if(ast-> type == PrimaryExpAST::LVAL){
        if(auto lval = dynamic_cast<LValAST*>(ast->lval.get())){
            auto symbol = ctx.symbol_table->lookup(lval -> ident);

            if(!symbol) {
                throw std::runtime_error("Undefined variable: " + lval->ident);
            } 


            if(symbol -> type == SymbolType::CONST){
                auto value = std::make_unique<IntegerIRValue>();
                value -> value = symbol ->const_value;
                return value;
            }
            else if(symbol -> type == SymbolType::VAR){
                std::string temp_name = generate_temp_name();

                auto load = std::make_unique<LoadIRValue>();
                load ->result_name = temp_name;
                load ->src = symbol->ir_name;  // 使用 symbol->ir_name
                ctx.current_block->ADD_Value(std::move(load));

                auto temp = std::make_unique<TemporaryIRValue>();
                temp -> temp_name = temp_name;
                return temp;
                
            }
            
        }
    }
    return nullptr;
}


std::unique_ptr<BaseIRValue> IRGenerator::visitNumber(const NumberAST* ast){
    if(!ast) return nullptr;

    auto int_value = std::make_unique<IntegerIRValue>();
    int_value->value = ast ->int_const;
    return std::move(int_value);
}


std::unique_ptr<IRProgram> IRGenerator::get_irprogram(){
    return std::move(program);
}






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