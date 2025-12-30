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
    GenContext::current_ctx = &ctx; // Initialize static pointer

    // Register SysY library functions
    ctx.symbol_table->declare("getint", SymbolType::FUNCTION, DataType::INT);
    ctx.symbol_table->declare("getch", SymbolType::FUNCTION, DataType::INT);
    ctx.symbol_table->declare("getarray", SymbolType::FUNCTION, DataType::INT);
    ctx.symbol_table->declare("putint", SymbolType::FUNCTION, DataType::VOID);
    ctx.symbol_table->declare("putch", SymbolType::FUNCTION, DataType::VOID);
    ctx.symbol_table->declare("putarray", SymbolType::FUNCTION, DataType::VOID);
    ctx.symbol_table->declare("starttime", SymbolType::FUNCTION, DataType::VOID);
    ctx.symbol_table->declare("stoptime", SymbolType::FUNCTION, DataType::VOID);
}


void IRGenerator::visitCompUnit(const CompUnitAST* ast){
    if(!ast) return;
    
    if(ast->type == CompUnitAST::FUNCDEF){
        if(auto func_def = dynamic_cast<const FunDefAST*>(ast->fun_def.get())){
            visitFunDef(func_def);
        }
    }
    else if(ast->type == CompUnitAST::DECL){
        if(auto decl = dynamic_cast<const DeclAST*>(ast->decl.get())){
            visitDecl(decl);
        }
    }
    else if(ast->type == CompUnitAST::COMPFUNC){
        visitCompUnit(dynamic_cast<const CompUnitAST*>(ast->compunit.get()));
        if(auto func_def = dynamic_cast<const FunDefAST*>(ast->fun_def.get())){
            visitFunDef(func_def);
        }
    }
    else if(ast->type == CompUnitAST::COMPDECL){
        visitCompUnit(dynamic_cast<const CompUnitAST*>(ast->compunit.get()));
        if(auto decl = dynamic_cast<const DeclAST*>(ast->decl.get())){
            visitDecl(decl);
        }
    }
}


void IRGenerator::visitFunDef(const FunDefAST* ast){
    if(!ast) return;
    
    // 1. Declare function in global symbol table
    DataType ret_type = DataType::VOID;
    if (ast->fun_type) {
        if (auto ft = dynamic_cast<const FunTypeAST*>(ast->fun_type.get())) {
            if (ft->type == FunTypeAST::INT) ret_type = DataType::INT;
        }
    }
    
    if (!ctx.symbol_table->lookup(ast->ident)) {
        ctx.symbol_table->declare(ast->ident, SymbolType::FUNCTION, ret_type);
    }
    
    auto ir_fun = std::make_unique<IRFunction>(); 
    ir_fun -> function_name = "@" + ast->ident;
    ir_fun -> functype = (ret_type == DataType::INT) ? "i32" : "void";
    
    // 2. Enter scope for parameters and body
    ctx.symbol_table->enterScope();
    
    // 3. Process parameters
    if(ast -> type == FunDefAST::FUNCF){
        if(auto funcfs = dynamic_cast<const FuncFParamsAST*>(ast -> funcfparams.get())){
            for(auto& i : funcfs ->funcflist){
                if(auto funcf = dynamic_cast<FuncFParamAST*>(i.get())){
                    if(funcf->type == FuncFParamAST::VAR){
                        std::string param_name = funcf->ident;
                        // Use %arg_ prefix for parameters to avoid conflict with globals (@) and locals (%)
                        std::string ir_param_name = "%arg_" + param_name; 
                        
                        ir_fun ->funcfparams.push_back({ir_param_name, "i32"});
                    }
                    else {
                        std::string param_name = funcf->ident;
                        // Use %arg_ prefix for parameters to avoid conflict with globals (@) and locals (%)
                        std::string ir_param_name = "%arg_" + param_name; 
                        
                        ir_fun ->funcfparams.push_back({ir_param_name, "*i32"});
                    
                    }
                }
            }
        }   
    }

    program ->ADD_Function(std::move(ir_fun));

    IRFunction* current_func = program->ir_function.back().get();

    auto entry_block = std::make_unique<IRBasicBlock>();
    entry_block->block_name = "%entry" + std::to_string(blockcount++);
    
    current_func ->ADD_Block(std::move(entry_block));
    
    IRBasicBlock* current_block = current_func ->ir_basicblock.back().get();
    
    setCurrentBlock(current_block);

    // Generate Alloc and Store for params
    if(ast -> type == FunDefAST::FUNCF){
        if(auto funcfs = dynamic_cast<const FuncFParamsAST*>(ast -> funcfparams.get())){
            for(auto& i : funcfs ->funcflist){
                if(auto funcf = dynamic_cast<FuncFParamAST*>(i.get())){
                    std::string param_name = funcf->ident;
                    std::string ir_param_name = "%arg_" + param_name;
                    
                    if(funcf ->type == FuncFParamAST::VAR){
                        // Declare local var for param
                        std::string local_var = ctx.symbol_table->declare(param_name, SymbolType::VAR, DataType::INT);
                        
                        // Alloc
                        auto alloc = std::make_unique<AllocIRValue>(AllocIRValue::VAR ,local_var, "i32");
                        ctx.current_block->ADD_Value(std::move(alloc));
                        
                        // Store
                        auto param_val = std::make_unique<TemporaryIRValue>();
                        param_val->temp_name = ir_param_name;
                        
                        auto store = std::make_unique<StoreIRValue>(std::move(param_val), local_var);
                        ctx.current_block->ADD_Value(std::move(store));
                    }
                    else{
                        // Declare local array for param - 使用 declareArray 并标记为数组形参
                        // 收集数组维度信息（第一维为0表示未知大小）
                        std::vector<int> dims;
                        dims.push_back(0);  // 第一维未知
                        
                        // 如果是多维数组形参，提取后续维度
                        if(funcf->type == FuncFParamAST::MULTIARRAY) {
                            if(auto arr_decl = dynamic_cast<ConstExpListAST*>(funcf->arraydeclarator.get())) {
                                for(auto& dim_exp : arr_decl->constexplist) {
                                    int dim_size = evaluateConstExp(dim_exp.get());
                                    dims.push_back(dim_size);
                                }
                            }
                        }
                        
                        std::string local_array = ctx.symbol_table->declareArray(
                            param_name, SymbolType::VAR, DataType::ARRAY, dims, {}, true);
                        
                        // Alloc - 对于指针类型，只需要 4 字节存储指针
                        auto alloc = std::make_unique<AllocIRValue>(AllocIRValue::ARRAY ,local_array, "*i32");
                        alloc->size = 4;  // 指针大小为 4 字节
                        ctx.current_block->ADD_Value(std::move(alloc));
                        
                        // Store
                        auto param_val = std::make_unique<TemporaryIRValue>();
                        param_val->temp_name = ir_param_name;
                        
                        auto store = std::make_unique<StoreIRValue>(std::move(param_val), local_array);
                        ctx.current_block->ADD_Value(std::move(store));
 
                        
                    }
                }
            }
        }   
    }

    // 4. Process Block
    if (ast ->block){
        if(auto block = dynamic_cast<const BlockAST*>(ast->block.get())){
            visitBlock(block);
        }
    }
    
    ctx.symbol_table->exitScope();
    
    // 5. Ensure return
    if (ctx.current_block && (ctx.current_block->ir_value.empty() || 
        (!dynamic_cast<ReturnIRValue*>(ctx.current_block->ir_value.back().get()) &&
         !dynamic_cast<JumpIRValue*>(ctx.current_block->ir_value.back().get()) &&
         !dynamic_cast<BranchIRValue*>(ctx.current_block->ir_value.back().get())))) {
            
        if (current_func->functype == "void") {
             auto ret = std::make_unique<ReturnIRValue>(ReturnIRValue::NUL);
             ctx.current_block->ADD_Value(std::move(ret));
        } else {
             auto zero = std::make_unique<IntegerIRValue>(0);
             auto ret = std::make_unique<ReturnIRValue>(ReturnIRValue::VALUE);
             ret->return_value = std::move(zero);
             ctx.current_block->ADD_Value(std::move(ret));
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
        
        // 4. 生成 store 指令
        if (lval->type == LValAST::ARRAY) {
            // Array assignment: arr[i][j][k] = val -> 使用线性索引
            auto index_list = dynamic_cast<ExpListAST*>(lval->address.get());
            auto is_param = symbol->get_is_array_param();
            
            // 收集所有索引的IR值
            std::vector<std::unique_ptr<BaseIRValue>> indices;
            for(auto &item : index_list->explist){
                if(auto exp_item = dynamic_cast<ExpAST*>(item.get())){
                    auto index_val = visitExp(exp_item);
                    indices.push_back(std::move(index_val));
                }    
            }
            
            // 计算线性索引
            auto linear_index = calculate_linear_index(indices, symbol->array_dims);
            
            std::string ptr_name;
            if(is_param) {
                // 数组形参：先 load 指针，再用 getptr
                std::string ptr_src = generate_temp_name();
                auto load_ir = std::make_unique<LoadIRValue>();
                load_ir->result_name = ptr_src;
                load_ir->src = symbol->ir_name;
                ctx.current_block->ADD_Value(std::move(load_ir));
                
                auto gtp = std::make_unique<GetPtrIRValue>();
                ptr_name = "%ptr_" + symbol->ir_name.substr(1) + "_" + std::to_string(blockcount++);
                gtp->result_name = ptr_name;
                gtp->src = ptr_src;
                gtp->index = std::move(linear_index);
                ctx.current_block->ADD_Value(std::move(gtp));
            } else {
                // 普通数组：getelemptr
                auto gep = std::make_unique<GetElemPtrIRValue>();
                ptr_name = "%ptr_" + symbol->ir_name.substr(1) + "_" + std::to_string(blockcount++);
                gep->result_name = ptr_name;
                gep->src = symbol->ir_name;
                gep->index = std::move(linear_index);
                ctx.current_block->ADD_Value(std::move(gep));
            }
            
            auto store_ir = std::make_unique<StoreIRValue>(std::move(rhs_value), ptr_name);
            ctx.current_block->ADD_Value(std::move(store_ir));
        } else {
            // Scalar assignment: a = val
            auto store_ir = std::make_unique<StoreIRValue>(std::move(rhs_value), symbol->ir_name);
            ctx.current_block->ADD_Value(std::move(store_ir));
        }
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
    if(ast ->type == ConstDefAST::CONST){
        int const_value = evaluateConstExp(ast->constinitval.get());
    
        try
        {
            ctx.symbol_table->declare(
                ast->ident,
                SymbolType::CONST,
                DataType::INT,
                const_value
            );
        }
        catch(const std::runtime_error& e)
        {
            std::cerr << "Error in constant declaration" << e.what() << '\n';
        }

    }
    else if(ast -> type == ConstDefAST::ARRAY){
        int array_size = 1;
        std::vector<int> dims = {};
        if(auto dimlist = dynamic_cast<ConstExpListAST*>(ast->dimlist.get())){
            for(auto &i : dimlist->constexplist){
                if(auto dim = dynamic_cast<ConstExpAST*>(i.get())){
                    int current_dim = evaluateConstExp(dim);
                    array_size *= current_dim; 
                    dims.push_back(current_dim);
                }
            }
        }
        if(auto constinitval =  dynamic_cast<ConstInitValAST*>(ast->constinitval.get())){
            auto initval = flatten_const_array(constinitval,dims);
            try
            {
                std::string ir_name = ctx.symbol_table->declareArray(
                    ast->ident,
                    SymbolType::CONST,
                    DataType::ARRAY,
                    dims,
                    initval
                );
    
                if (ctx.symbol_table->getCurrentScopeLevel() == 0) {
                    // Global constant array
                    auto global_alloc = std::make_unique<GlobalAllocIRValue>();
                    global_alloc->type = GlobalAllocIRValue::ARRAY;
                    global_alloc->var_name = ir_name;
                    global_alloc->data_type = "[i32 ," + std::to_string(array_size) + "]";
                    global_alloc->init_list = initval;
                    global_alloc->float_size = array_size;
                    program->global_value.push_back(std::move(global_alloc));
                } else {
                    // Local constant array
                    auto alloc = std::make_unique<AllocIRValue>();
                    alloc->type = AllocIRValue::ARRAY;
                    alloc->var_name = ir_name;
                    alloc->data_type ="[i32 ," + std::to_string(array_size) + "]";
                    alloc->size = array_size * 4;
                    ctx.current_block->ADD_Value(std::move(alloc));
    
                    // Initialize local array
                    for(int i=0; i<array_size; ++i) {
                        auto gep = std::make_unique<GetElemPtrIRValue>();
                        std::string ptr_name = "%ptr_" + ir_name.substr(1) + "_" + std::to_string(i) + "_" + std::to_string(blockcount++); 
                        gep->result_name = ptr_name;
                        gep->src = ir_name;
                        gep->index = std::make_unique<IntegerIRValue>(i);
                        ctx.current_block->ADD_Value(std::move(gep));
    
                        auto store = std::make_unique<StoreIRValue>();
                        store->value = std::make_unique<IntegerIRValue>(initval[i]);
                        store->dest = ptr_name;
                        ctx.current_block->ADD_Value(std::move(store));
                    }
                }
            }
            catch(const std::runtime_error& e)
            {
                std::cerr << "Error in constant declaration" << e.what() << '\n';
            }
        }
    }
}








std::vector<int> IRGenerator::visitConstInitVal(const ConstInitValAST* ast,int array_size){
    if(!ast) return {};
    std::vector<int> init_list;
    if(ast -> type ==  ConstInitValAST::CONSTLIST){
        if(auto constlist = dynamic_cast<ConstExpListAST*>(ast->constinitlist.get())){
            for(const auto &i : constlist->constexplist){
                if(auto constexp = dynamic_cast<ConstExpAST*>(i.get())){
                    init_list.push_back(evaluateConstExp(constexp));
                }
            }
        }
        int init_size = init_list.size();
        for(int i = init_size ; i<array_size ; i++){
            init_list.push_back(0);
        }
    }
    else if(ast -> type == ConstInitValAST::ZEROINIT){
        return {};
    }
    return init_list;
}

/*

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

    bool is_global = ctx.symbol_table->isGlobalScope();

    if(ast->type == VarDefAST::IDENT || ast->type == VarDefAST::IDENTDEF){
        std::string var_name;
        try {
            var_name = ctx.symbol_table->declare(
                ast->ident,
                SymbolType::VAR,
                DataType::INT
            );
        } catch(const std::runtime_error& e) {
            std::cerr << "Error in variable declaration: " << e.what() << '\n';
            return;
        }

        if(is_global){
            int init_value = 0;
            if(ast->type == VarDefAST::IDENTDEF){
                if(auto initval = dynamic_cast<InitValAST*>(ast->initval.get())){
                    init_value = evaluateConstExp(initval->exp.get());
                }
            }

            auto init_ir = std::make_unique<IntegerIRValue>(init_value);            
            auto global_alloc = std::make_unique<GlobalAllocIRValue>(
                GlobalAllocIRValue::VAR, std::move(init_ir), var_name, "i32"
            );
            ctx.program->ADD_Globalvalue(std::move(global_alloc));
        }
        else{
            // allocIR
            auto alloc = std::make_unique<AllocIRValue>(AllocIRValue::VAR, var_name, "i32");
            ctx.current_block->ADD_Value(std::move(alloc));
            
            if(ast->type == VarDefAST::IDENTDEF){
                if(auto initval = dynamic_cast<InitValAST*>(ast->initval.get())){
                    if(auto exp = dynamic_cast<ExpAST*>(initval->exp.get())){
                        auto val = visitExp(exp);
                        auto store = std::make_unique<StoreIRValue>(std::move(val), var_name);
                        ctx.current_block->ADD_Value(std::move(store));
                    }
                }
            }
        }     
    }
    else if(ast->type == VarDefAST::ARRAY || ast->type == VarDefAST::ARRAYDEF){
        int array_size = 1;
        std::vector<int> dims = {};
        if(auto dimlist = dynamic_cast<ConstExpListAST*>(ast->dimlist.get())){
            for(auto &i : dimlist->constexplist){
                if(auto dim = dynamic_cast<ConstExpAST*>(i.get())){
                    int current_dim = evaluateConstExp(dim);
                    array_size *= current_dim; 
                    dims.push_back(current_dim);
                }
            }
        }     
        std::string ir_name;
        try {
            ir_name = ctx.symbol_table->declareArray(ast->ident, SymbolType::VAR, DataType::ARRAY, dims);
        } catch(const std::runtime_error& e) {
            std::cerr << "Error in array declaration: " << e.what() << '\n';
            return;
        }

        if(is_global){
            auto global_alloc = std::make_unique<GlobalAllocIRValue>();
            global_alloc->type = GlobalAllocIRValue::ARRAY;
            global_alloc->var_name = ir_name;
            global_alloc->data_type = "[i32 ," + std::to_string(array_size) + "]";
            global_alloc->float_size = array_size; 

            if(ast->type == VarDefAST::ARRAYDEF){
                global_alloc->init_list = flatten_array(dynamic_cast<InitValAST*>(ast->initval.get()), dims);
            } else {
                global_alloc->init_list = {};
            }
            ctx.program->ADD_Globalvalue(std::move(global_alloc));
        } else {
            auto alloc = std::make_unique<AllocIRValue>();
            alloc->type = AllocIRValue::ARRAY;
            alloc->var_name = ir_name;
            alloc->data_type = "[i32 ," + std::to_string(array_size) + "]";
            alloc->size = array_size * 4;
            ctx.current_block->ADD_Value(std::move(alloc));
            
            if(ast->type == VarDefAST::ARRAYDEF){
                // 使用 flatten_array 获取展平的初始化值
                auto flat_values = flatten_array(dynamic_cast<InitValAST*>(ast->initval.get()), dims);
                
                // 逐个存储到数组元素
                for(int i = 0; i < array_size; ++i) {
                    auto gep = std::make_unique<GetElemPtrIRValue>();
                    std::string ptr_name = "%ptr_" + ir_name.substr(1) + "_" + 
                                           std::to_string(i) + "_" + std::to_string(blockcount++);
                    gep->result_name = ptr_name;
                    gep->src = ir_name;
                    gep->index = std::make_unique<IntegerIRValue>(i);
                    ctx.current_block->ADD_Value(std::move(gep));
                    
                    auto store = std::make_unique<StoreIRValue>();
                    store->value = std::make_unique<IntegerIRValue>(flat_values[i]);
                    store->dest = ptr_name;
                    ctx.current_block->ADD_Value(std::move(store));
                }
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
                // Short-circuit evaluation for OR (||)
                // A || B: if A is true, result is 1. Else result is B != 0.
                
                // 1. Allocate result variable
                std::string result_var_name = "%or_res_" + std::to_string(blockcount++);
                auto alloc = std::make_unique<AllocIRValue>(result_var_name, "i32");
                ctx.current_block->ADD_Value(std::move(alloc));

                // 2. Evaluate Left
                auto left_val = visitLOrExp(lor);
                
                // Check Left != 0
                auto l_ne = std::make_unique<BinaryIRValue>();
                l_ne->operation = BinaryIRValue::NE;
                l_ne->left = std::move(left_val);
                l_ne->right = std::make_unique<IntegerIRValue>(0);
                l_ne->result_name = generate_temp_name();
                auto l_cond_name = l_ne->result_name;
                ctx.current_block->ADD_Value(std::move(l_ne));
                
                auto l_cond_val = std::make_unique<TemporaryIRValue>();
                l_cond_val->temp_name = l_cond_name;

                // 3. Create Blocks
                auto true_block = std::make_unique<IRBasicBlock>();
                true_block->block_name = "%or_true_" + std::to_string(blockcount++);
                std::string true_label = true_block->block_name;

                auto false_block = std::make_unique<IRBasicBlock>();
                false_block->block_name = "%or_false_" + std::to_string(blockcount++);
                std::string false_label = false_block->block_name;

                auto end_block = std::make_unique<IRBasicBlock>();
                end_block->block_name = "%or_end_" + std::to_string(blockcount++);
                std::string end_label = end_block->block_name;

                // 4. Branch
                auto br = std::make_unique<BranchIRValue>(std::move(l_cond_val), true_label, false_label);
                ctx.current_block->ADD_Value(std::move(br));

                auto current_func = ctx.program->ir_function.back().get();

                // 5. True Block (Short Circuit -> 1)
                current_func->ADD_Block(std::move(true_block));
                setCurrentBlock(current_func->ir_basicblock.back().get());
                
                auto store_true = std::make_unique<StoreIRValue>(std::make_unique<IntegerIRValue>(1), result_var_name);
                ctx.current_block->ADD_Value(std::move(store_true));
                
                auto jump_end1 = std::make_unique<JumpIRValue>(end_label);
                ctx.current_block->ADD_Value(std::move(jump_end1));

                // 6. False Block (Evaluate Right)
                current_func->ADD_Block(std::move(false_block));
                setCurrentBlock(current_func->ir_basicblock.back().get());

                auto right_val = visitLAndExp(land);
                
                // Check Right != 0
                auto r_ne = std::make_unique<BinaryIRValue>();
                r_ne->operation = BinaryIRValue::NE;
                r_ne->left = std::move(right_val);
                r_ne->right = std::make_unique<IntegerIRValue>(0);
                r_ne->result_name = generate_temp_name();
                auto r_cond_name = r_ne->result_name;
                ctx.current_block->ADD_Value(std::move(r_ne));

                auto r_cond_val = std::make_unique<TemporaryIRValue>();
                r_cond_val->temp_name = r_cond_name;

                auto store_right = std::make_unique<StoreIRValue>(std::move(r_cond_val), result_var_name);
                ctx.current_block->ADD_Value(std::move(store_right));

                auto jump_end2 = std::make_unique<JumpIRValue>(end_label);
                ctx.current_block->ADD_Value(std::move(jump_end2));

                // 7. End Block
                current_func->ADD_Block(std::move(end_block));
                setCurrentBlock(current_func->ir_basicblock.back().get());

                auto load_res = std::make_unique<LoadIRValue>();
                load_res->result_name = generate_temp_name();
                load_res->src = result_var_name;
                
                auto result_val = std::make_unique<TemporaryIRValue>();
                result_val->temp_name = load_res->result_name;
                
                ctx.current_block->ADD_Value(std::move(load_res));

                return std::move(result_val);
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
                // Short-circuit evaluation for AND (&&)
                // A && B: if A is false, result is 0. Else result is B != 0.

                // 1. Allocate result variable
                std::string result_var_name = "%and_res_" + std::to_string(blockcount++);
                auto alloc = std::make_unique<AllocIRValue>(result_var_name, "i32");
                ctx.current_block->ADD_Value(std::move(alloc));

                // 2. Evaluate Left
                auto left_val = visitLAndExp(land);
                
                // Check Left != 0
                auto l_ne = std::make_unique<BinaryIRValue>();
                l_ne->operation = BinaryIRValue::NE;
                l_ne->left = std::move(left_val);
                l_ne->right = std::make_unique<IntegerIRValue>(0);
                l_ne->result_name = generate_temp_name();
                auto l_cond_name = l_ne->result_name;
                ctx.current_block->ADD_Value(std::move(l_ne));
                
                auto l_cond_val = std::make_unique<TemporaryIRValue>();
                l_cond_val->temp_name = l_cond_name;

                // 3. Create Blocks
                auto true_block = std::make_unique<IRBasicBlock>();
                true_block->block_name = "%and_true_" + std::to_string(blockcount++);
                std::string true_label = true_block->block_name;

                auto false_block = std::make_unique<IRBasicBlock>();
                false_block->block_name = "%and_false_" + std::to_string(blockcount++);
                std::string false_label = false_block->block_name;

                auto end_block = std::make_unique<IRBasicBlock>();
                end_block->block_name = "%and_end_" + std::to_string(blockcount++);
                std::string end_label = end_block->block_name;

                // 4. Branch
                auto br = std::make_unique<BranchIRValue>(std::move(l_cond_val), true_label, false_label);
                ctx.current_block->ADD_Value(std::move(br));

                auto current_func = ctx.program->ir_function.back().get();

                // 5. False Block (Short Circuit -> 0)
                current_func->ADD_Block(std::move(false_block));
                setCurrentBlock(current_func->ir_basicblock.back().get());
                
                auto store_false = std::make_unique<StoreIRValue>(std::make_unique<IntegerIRValue>(0), result_var_name);
                ctx.current_block->ADD_Value(std::move(store_false));
                
                auto jump_end1 = std::make_unique<JumpIRValue>(end_label);
                ctx.current_block->ADD_Value(std::move(jump_end1));

                // 6. True Block (Evaluate Right)
                current_func->ADD_Block(std::move(true_block));
                setCurrentBlock(current_func->ir_basicblock.back().get());

                auto right_val = visitEqExp(eq);
                
                // Check Right != 0
                auto r_ne = std::make_unique<BinaryIRValue>();
                r_ne->operation = BinaryIRValue::NE;
                r_ne->left = std::move(right_val);
                r_ne->right = std::make_unique<IntegerIRValue>(0);
                r_ne->result_name = generate_temp_name();
                auto r_cond_name = r_ne->result_name;
                ctx.current_block->ADD_Value(std::move(r_ne));

                auto r_cond_val = std::make_unique<TemporaryIRValue>();
                r_cond_val->temp_name = r_cond_name;

                auto store_right = std::make_unique<StoreIRValue>(std::move(r_cond_val), result_var_name);
                ctx.current_block->ADD_Value(std::move(store_right));

                auto jump_end2 = std::make_unique<JumpIRValue>(end_label);
                ctx.current_block->ADD_Value(std::move(jump_end2));

                // 7. End Block
                current_func->ADD_Block(std::move(end_block));
                setCurrentBlock(current_func->ir_basicblock.back().get());

                auto load_res = std::make_unique<LoadIRValue>();
                load_res->result_name = generate_temp_name();
                load_res->src = result_var_name;
                
                auto result_val = std::make_unique<TemporaryIRValue>();
                result_val->temp_name = load_res->result_name;
                
                ctx.current_block->ADD_Value(std::move(load_res));

                return std::move(result_val);
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
    else if(ast->type == UnaryExpAST::FUNCVOID || ast->type == UnaryExpAST::FUNCRPARAMS) {
        std::string func_name = ast->ident;
        auto symbol = ctx.symbol_table->lookup(func_name);
        if (!symbol || symbol->type != SymbolType::FUNCTION) {
             throw std::runtime_error("Undefined function: " + func_name);
        }
        
        auto call = std::make_unique<CallIRValue>();
        call->func_name = symbol->ir_name;
        
        if (ast->type == UnaryExpAST::FUNCRPARAMS) {
            if (auto params = dynamic_cast<const FuncRParamsAST*>(ast->funcrparams.get())) {
                for (const auto& exp : params->explist) {
                    if (auto e = dynamic_cast<const ExpAST*>(exp.get())) {
                        auto arg_val = visitExp(e);
                        if (!arg_val) {
                            throw std::runtime_error("Failed to generate IR for function argument");
                        }
                        call->funcrparams.push_back(std::move(arg_val));
                    }
                }
            }
        }
        
        if (symbol->datatype == DataType::VOID) {
            call->type = CallIRValue::VOID;
            ctx.current_block->ADD_Value(std::move(call));
            return nullptr;
        } else {
            call->type = CallIRValue::OTHER;
            call->result_name = generate_temp_name();
            
            auto temp = std::make_unique<TemporaryIRValue>();
            temp->temp_name = call->result_name;
            
            ctx.current_block->ADD_Value(std::move(call));
            return std::move(temp);
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

            if (lval->type == LValAST::ARRAY) {
                // Array access: arr[i][j][k] -> 使用线性索引 i*D1*D2 + j*D2 + k
                auto index_list = dynamic_cast<ExpListAST*>(lval->address.get());
                auto is_param = symbol->get_is_array_param();
                
                // 收集所有索引的IR值
                std::vector<std::unique_ptr<BaseIRValue>> indices;
                for(auto &item : index_list->explist){
                    if(auto exp = dynamic_cast<ExpAST*>(item.get())){
                        auto index_val = visitExp(exp);
                        indices.push_back(std::move(index_val));
                    }    
                }
                
                // 判断是否为部分索引（索引数量 < 维度数量）
                // 对于数组形参，dims 可能为空或第一维为0，需要特殊处理
                size_t num_dims = symbol->array_dims.size();
                size_t num_indices = indices.size();
                bool is_partial_index = (num_indices < num_dims);
                
                // 计算线性索引
                auto linear_index = calculate_linear_index(indices, symbol->array_dims);
                
                if(is_param){
                    // 数组形参：先 load 指针，再用 getptr
                    std::string ptr_src = generate_temp_name();
                    auto load_ir1 = std::make_unique<LoadIRValue>();
                    load_ir1->result_name = ptr_src;
                    load_ir1->src = symbol->ir_name;
                    ctx.current_block->ADD_Value(std::move(load_ir1));

                    auto gtp = std::make_unique<GetPtrIRValue>();
                    std::string ptr_name = "%ptr_nm_" + symbol->ir_name.substr(1) + "_" + std::to_string(blockcount++);
                    gtp->src = ptr_src;
                    gtp->result_name = ptr_name; 
                    gtp->index = std::move(linear_index);
                    ctx.current_block->ADD_Value(std::move(gtp));

                    if(is_partial_index) {
                        // 部分索引：返回地址，不 load
                        auto temp = std::make_unique<TemporaryIRValue>();
                        temp->temp_name = ptr_name;
                        return temp;
                    } else {
                        // 完整索引：load 值
                        std::string temp_name = generate_temp_name();
                        auto load_ir2 = std::make_unique<LoadIRValue>();
                        load_ir2->result_name = temp_name;
                        load_ir2->src = ptr_name;
                        ctx.current_block->ADD_Value(std::move(load_ir2));
                        
                        auto temp = std::make_unique<TemporaryIRValue>();
                        temp->temp_name = temp_name;
                        return temp;
                    }
                }
                else{
                    // 普通数组：getelemptr
                    auto gep = std::make_unique<GetElemPtrIRValue>();
                    std::string ptr_name = "%ptr_ar_" + symbol->ir_name.substr(1) + "_" + std::to_string(blockcount++);
                    gep->result_name = ptr_name;
                    gep->src = symbol->ir_name;
                    gep->index = std::move(linear_index);
                    ctx.current_block->ADD_Value(std::move(gep));
                    
                    if(is_partial_index) {
                        // 部分索引：返回地址，不 load
                        auto temp = std::make_unique<TemporaryIRValue>();
                        temp->temp_name = ptr_name;
                        return temp;
                    } else {
                        // 完整索引：load 值
                        std::string temp_name = generate_temp_name();
                        auto load = std::make_unique<LoadIRValue>();
                        load->result_name = temp_name;
                        load->src = ptr_name;
                        ctx.current_block->ADD_Value(std::move(load));
                        
                        auto temp = std::make_unique<TemporaryIRValue>();
                        temp->temp_name = temp_name;
                        return temp;
                    }
                }
            } else {
                // lval->type == LValAST::VAR（没有索引）
                if(symbol->datatype == DataType::ARRAY) {
                    // 数组名作为实参：传递数组首地址
                    // 对于普通数组：getelemptr @arr, 0
                    // 对于数组形参：load %arr（已经是指针）
                    auto is_param = symbol->get_is_array_param();
                    
                    if(is_param) {
                        // 数组形参：load 出指针值直接返回
                        std::string ptr_val = generate_temp_name();
                        auto load = std::make_unique<LoadIRValue>();
                        load->result_name = ptr_val;
                        load->src = symbol->ir_name;
                        ctx.current_block->ADD_Value(std::move(load));
                        
                        auto temp = std::make_unique<TemporaryIRValue>();
                        temp->temp_name = ptr_val;
                        return temp;
                    } else {
                        // 普通数组：getelemptr 获取首地址
                        auto gep = std::make_unique<GetElemPtrIRValue>();
                        std::string ptr_name = "%ptr_" + symbol->ir_name.substr(1) + "_" + std::to_string(blockcount++);
                        gep->result_name = ptr_name;
                        gep->src = symbol->ir_name;
                        gep->index = std::make_unique<IntegerIRValue>(0);
                        ctx.current_block->ADD_Value(std::move(gep));
                        
                        auto temp = std::make_unique<TemporaryIRValue>();
                        temp->temp_name = ptr_name;
                        return temp;
                    }
                }
                else if(symbol -> type == SymbolType::CONST){
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




