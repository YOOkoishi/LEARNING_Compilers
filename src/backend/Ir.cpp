#include "../include/include.h"
#include "../include/Ir.h"
#include "../include/Ir_generator.h"



GenContext* GenContext::current_ctx = nullptr;








// 各类型IR Dump() 函数的定义





void ReturnIRValue::Dump() const{
    if(type == ReturnIRValue::VALUE){
        std::cout<<"  ret ";
        if(auto result = dynamic_cast<TemporaryIRValue* >(return_value.get())){
            result -> Dump() ;
        }
        else if(auto result = dynamic_cast<IntegerIRValue* >(return_value.get())){
            result -> Dump() ;   
        }
        else {
            std::cout<<"<unknown> ";
        }
        std::cout<<std::endl;
    }
    else {
        std::cout<<"  ret "<<std::endl;
    }
}



void AllocIRValue::Dump() const {
    std::cout << "  " << var_name << " = alloc " << data_type << std::endl;
}


void StoreIRValue::Dump() const {
    std::cout << "  store ";
    if (value) {
        value->Dump();  
    }
    std::cout << ", " << dest << std::endl;
}


void LoadIRValue::Dump() const {
    std::cout << "  " << result_name << " = load " << src << std::endl;
}




void IntegerIRValue::Dump() const{
    std::cout<<value;
}




void BinaryIRValue::Dump() const{
    std::string operation_name;
    switch (operation)
    {
        case ADD:
        std::cout<<"  "<<result_name << " = " << "add " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;  
        
        case SUB:
        std::cout<<"  "<<result_name << " = " << "sub " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case EQ:
        std::cout<<"  "<<result_name << " = " << "eq " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case MUL:
        std::cout<<"  "<<result_name << " = " << "mul " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case DIV:
        std::cout<<"  "<<result_name << " = " << "div " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case MOD:
        std::cout<<"  "<<result_name << " = " << "mod " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case NE:
        std::cout<<"  "<<result_name << " = " << "ne " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case LT:
        std::cout<<"  "<<result_name << " = " << "lt " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case GT:
        std::cout<<"  "<<result_name << " = " << "gt " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case GE:
        std::cout<<"  "<<result_name << " = " << "ge " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case LE:
        std::cout<<"  "<<result_name << " = " << "le " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case AND:
        std::cout<<"  "<<result_name << " = " << "and " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case OR:
        std::cout<<"  "<<result_name << " = " << "or " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        
        default:
        break;
    }
}

void TemporaryIRValue ::Dump() const{
    std::cout<< temp_name;
}


void JumpIRValue::Dump() const {
    std::cout << "  jump " << target_label << std::endl;
}




void CallIRValue::Dump() const {
    std::cout << "  ";
    if (type == OTHER) {
        std::cout << result_name << " = ";
    }
    std::cout << "call " << func_name << "(";
    for (size_t i = 0; i < funcrparams.size(); ++i) {
        funcrparams[i]->Dump();
        if (i < funcrparams.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << ")" << std::endl;
}




void GlobalAllocIRValue::Dump() const {
    std::cout << "global " << var_name << " = alloc " << data_type << ", ";
    if (type == VAR) {
        if (auto val = dynamic_cast<IntegerIRValue*>(value.get())) {
            if (val->value == 0) {
                std::cout << "zeroinit";
            } else {
                std::cout << val->value;
            }
        } else {
            std::cout << "zeroinit";
        }
    } else if (type == ARRAY) {
        if (init_list.empty()) {
            std::cout << "zeroinit";
        } else {
            std::cout << "{";
            for (size_t i = 0; i < init_list.size(); ++i) {
                std::cout << init_list[i];
                if (i < init_list.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "}";
        }
    }
    std::cout << std::endl;
}


void GetElemPtrIRValue::Dump() const {
    std::cout << "  " << result_name << " = getelemptr " << src << ", ";
    index->Dump();
    std::cout << std::endl;
}





void BranchIRValue::Dump() const {
    std::cout << "  br ";
    if (condition) condition->Dump();
    std::cout << ", " << true_label << ", " << false_label << std::endl;
}


void IRBasicBlock::DumpValue() const{
    std::cout << "\n" << block_name << ":" << std::endl;
    for(const auto &val : ir_value){
        val -> Dump();
    }    
}


void IRFunction::DumpBlock() const{
    std::cout << "fun " << function_name << "(";
    for(size_t i = 0; i < funcfparams.size(); ++i) {
        std::cout << funcfparams[i].first << ": " << funcfparams[i].second;
        if(i < funcfparams.size() - 1) std::cout << ", ";
    }
    std::cout << ")";
    if(functype != "void") std::cout << ": " << functype;
    std::cout << " {\n";
    for(const auto &block : ir_basicblock){
        block -> DumpValue();
    }
    std::cout << "\n}\n\n";
}


void IRProgram::DumpFunction() const{
    std::cout << "decl @getint(): i32" << std::endl;
    std::cout << "decl @getch(): i32" << std::endl;
    std::cout << "decl @getarray(*i32): i32" << std::endl;
    std::cout << "decl @putint(i32)" << std::endl;
    std::cout << "decl @putch(i32)" << std::endl;
    std::cout << "decl @putarray(i32, *i32)" << std::endl;
    std::cout << "decl @starttime()" << std::endl;
    std::cout << "decl @stoptime()" << std::endl;
    std::cout << std::endl;

    for(const auto &gv : global_value){
        gv -> Dump();
    }
    
    if(!global_value.empty()){
        std::cout<<std::endl;
    }

    for(const auto &fun : ir_function){
        fun -> DumpBlock();
    }
}



//  IR 中的 ADD 函数定义



void IRFunction::ADD_Block(std::unique_ptr<IRBasicBlock> block){
    ir_basicblock.push_back(std::move(block));
}


void IRBasicBlock::ADD_Value(std::unique_ptr<BaseIRValue> val){
    ir_value.push_back(std::move(val));
}


void IRProgram::ADD_Function(std::unique_ptr<IRFunction> func){
    ir_function.push_back(std::move(func));
}


void IRProgram::ADD_Globalvalue(std::unique_ptr<BaseIRValue> val){
    global_value.push_back(std::move(val));
}

// 各类型IR的To_RiscV() 函数定义





void IRProgram::To_RiscV() const{
    for(const auto &gv : global_value){
        std::cout<<"  .data"<<std::endl;
        gv ->To_RiscV();
    }
    

    for(const auto &fun : ir_function){
        std::cout<<"  .text"<<std::endl;
        fun -> To_RiscV();
    }
}


void IRFunction::To_RiscV() const{
    GenContext ctx;
    GenContext::current_ctx = &ctx;

    // 注册函数参数的映射
    for (size_t i = 0; i < funcfparams.size(); ++i) {
        if (i < 8) {
            // 前8个参数通过寄存器 a0-a7 传递
            ctx.stack.registerParam(funcfparams[i].first, i);
        } else {
            // 第9个及以后的参数通过栈传递
            // 这些参数在调用者栈帧中，相对于当前 sp+frame_size 的偏移是 (i-8)*4
            ctx.stack.registerStackParam(funcfparams[i].first, (i - 8) * 4);
        }
    }

    // 第一遍扫描: 计算 S (局部变量), R (ra), A (传参)
    bool has_call = false;
    int max_args = 0;
    
    for(const auto& block : ir_basicblock){
        for (const auto& value : block ->ir_value){
            if(auto* alloc = dynamic_cast<AllocIRValue*>(value.get())){
                // 修复：分配数组大小
                if (alloc->type == AllocIRValue::ARRAY) {
                    ctx.stack.allocate(alloc->var_name, alloc->size);
                } else {
                    ctx.stack.allocate(alloc->var_name, 4);
                }
            }   
            else if(auto* load = dynamic_cast<LoadIRValue*>(value.get())){
                ctx.stack.allocate(load->result_name);
            }
            else if(auto* binary = dynamic_cast<BinaryIRValue*>(value.get())){
                ctx.stack.allocate(binary->result_name);
            }
            else if(auto* ptr = dynamic_cast<GetElemPtrIRValue*>(value.get())){
                ctx.stack.allocate(ptr->result_name);
            }
            else if(auto* call = dynamic_cast<CallIRValue*>(value.get())){
                has_call = true;
                int args_count = call->funcrparams.size();
                if (args_count > max_args) max_args = args_count;
                // call 有返回值时也需要分配空间
                if (call->type == CallIRValue::OTHER) {
                    ctx.stack.allocate(call->result_name);
                }
            }
        }
    }
    
    // R: 如果有 call 则为 4, 否则为 0
    int R = has_call ? 4 : 0;
    ctx.stack.setR(R);
    
    // A: max(max_args - 8, 0) * 4
    int A = (max_args > 8) ? (max_args - 8) * 4 : 0;
    ctx.stack.setA(A);
    
    // 计算总栈帧大小 S' = (S + R + A) 向上取整到 16
    ctx.stack.calculateFrameSize();
    int frame_size = ctx.stack.getFrameSize();

    // 生成 prologue
    std::string name = function_name;
    if (name.length() > 0 && name[0] == '@') {
        name = name.substr(1);
    }
    std::cout<<"  .globl "<<name<<std::endl;
    std::cout<<name<<":\n";

    if(frame_size > 0){
        if (frame_size >= 2048) {
            std::cout << "  li t0, -" << frame_size << std::endl;
            std::cout << "  add sp, sp, t0" << std::endl;
        } else {
            std::cout << "  addi sp, sp, -" << frame_size << std::endl;
        }
        
        // 如果 R != 0, 保存 ra 到 sp + S' - 4
        if (R > 0) {
            int ra_offset = ctx.stack.getRaOffset();
            if (ra_offset >= 2048) {
                std::cout << "  li t0, " << ra_offset << std::endl;
                std::cout << "  add t0, sp, t0" << std::endl;
                std::cout << "  sw ra, 0(t0)" << std::endl;
            } else {
                std::cout << "  sw ra, " << ra_offset << "(sp)" << std::endl;
            }
        }
    }    

    // 生成函数体
    for(const auto &block : ir_basicblock){
        block ->To_RiscV();
    }

    GenContext::current_ctx = nullptr;
}




void IRBasicBlock::To_RiscV() const{
    std::string label = block_name;
    if (label.length() > 0 && label[0] == '%') {
        label = label.substr(1);
    }
    // 使用 .L 前缀避免与函数名冲突
    std::cout << ".L" << label << ":" << std::endl;

    for(const auto &val : ir_value){
        val ->To_RiscV();
        std::cout<<std::endl;
    }
}


void ReturnIRValue::To_RiscV() const{
    auto& stack = GenContext::current_ctx->stack;
    
    if(type == ReturnIRValue::VALUE){
        if(auto int_val = dynamic_cast<IntegerIRValue*>(return_value.get())) {
            if(int_val->value == 0) {
                std::cout << "  mv a0, x0" << std::endl;
            } else {
                std::cout << "  li a0, " << int_val->value << std::endl;
            }
        } else if(auto temp = dynamic_cast<TemporaryIRValue*>(return_value.get())) {
            int offset = stack.getOffset(temp->temp_name);
            if (offset >= 2048) {
                std::cout << "  li t0, " << offset << std::endl;
                std::cout << "  add t0, sp, t0" << std::endl;
                std::cout << "  lw a0, 0(t0)" << std::endl;
            } else {
                std::cout << "  lw a0, " << offset << "(sp)" << std::endl;
            }
        }
    }
    
    // epilogue: 恢复 ra (如果需要), 恢复 sp, 然后 ret
    int frame_size = stack.getFrameSize();
    if (frame_size > 0) {
        // 如果 R != 0, 从栈恢复 ra
        if (stack.getR() > 0) {
            int ra_offset = stack.getRaOffset();
            if (ra_offset >= 2048) {
                std::cout << "  li t0, " << ra_offset << std::endl;
                std::cout << "  add t0, sp, t0" << std::endl;
                std::cout << "  lw ra, 0(t0)" << std::endl;
            } else {
                std::cout << "  lw ra, " << ra_offset << "(sp)" << std::endl;
            }
        }
        
        if (frame_size >= 2048) {
            std::cout << "  li t0, " << frame_size << std::endl;
            std::cout << "  add sp, sp, t0" << std::endl;
        } else {
            std::cout << "  addi sp, sp, " << frame_size << std::endl;
        }
    }
    std::cout << "  ret";
}


void IntegerIRValue::To_RiscV() const{
    // 通常不直接调用，由其他指令处理
    std::cout << value;
}


void BinaryIRValue::To_RiscV() const {
    auto& stack = GenContext::current_ctx->stack;
    int result_offset = stack.getOffset(result_name);
    
    // 加载左操作数到 t0
    if (auto* left_int = dynamic_cast<IntegerIRValue*>(left.get())) {
        std::cout << "  li t0, " << left_int->value << std::endl;
    } else if (auto* left_temp = dynamic_cast<TemporaryIRValue*>(left.get())) {
        int left_offset = stack.getOffset(left_temp->temp_name);
        if (left_offset >= 2048) {
            std::cout << "  li t2, " << left_offset << std::endl;
            std::cout << "  add t2, sp, t2" << std::endl;
            std::cout << "  lw t0, 0(t2)" << std::endl;
        } else {
            std::cout << "  lw t0, " << left_offset << "(sp)" << std::endl;
        }
    }
    
    // 加载右操作数到 t1
    if (auto* right_int = dynamic_cast<IntegerIRValue*>(right.get())) {
        std::cout << "  li t1, " << right_int->value << std::endl;
    } else if (auto* right_temp = dynamic_cast<TemporaryIRValue*>(right.get())) {
        int right_offset = stack.getOffset(right_temp->temp_name);
        if (right_offset >= 2048) {
            std::cout << "  li t2, " << right_offset << std::endl;
            std::cout << "  add t2, sp, t2" << std::endl;
            std::cout << "  lw t1, 0(t2)" << std::endl;
        } else {
            std::cout << "  lw t1, " << right_offset << "(sp)" << std::endl;
        }
    }
    
    // 执行运算，结果放在 t0
    switch (operation) {
        case ADD: std::cout << "  add t0, t0, t1" << std::endl; break;
        case SUB: std::cout << "  sub t0, t0, t1" << std::endl; break;
        case MUL: std::cout << "  mul t0, t0, t1" << std::endl; break;
        case DIV: std::cout << "  div t0, t0, t1" << std::endl; break;
        case MOD: std::cout << "  rem t0, t0, t1" << std::endl; break;
        case LT:  std::cout << "  slt t0, t0, t1" << std::endl; break;
        case GT:  std::cout << "  sgt t0, t0, t1" << std::endl; break;
        case AND: std::cout << "  and t0, t0, t1" << std::endl; break;
        case OR:  std::cout << "  or t0, t0, t1" << std::endl; break;
        case LE:
            std::cout << "  sgt t0, t0, t1" << std::endl;
            std::cout << "  xori t0, t0, 1" << std::endl;
            break;
        case GE:
            std::cout << "  slt t0, t0, t1" << std::endl;
            std::cout << "  xori t0, t0, 1" << std::endl;
            break;
        case EQ:
            std::cout << "  xor t0, t0, t1" << std::endl;
            std::cout << "  seqz t0, t0" << std::endl;
            break;
        case NE:
            std::cout << "  xor t0, t0, t1" << std::endl;
            std::cout << "  snez t0, t0" << std::endl;
            break;
        default:
            std::cout << "  # Unknown operation" << std::endl;
            break;
    }
    
    // 保存结果到栈
    if (result_offset >= 2048) {
        std::cout << "  li t1, " << result_offset << std::endl;
        std::cout << "  add t1, sp, t1" << std::endl;
        std::cout << "  sw t0, 0(t1)";
    } else {
        std::cout << "  sw t0, " << result_offset << "(sp)";
    }
}


void TemporaryIRValue::To_RiscV() const{
    // 通常不直接调用，由其他指令处理
    // 如果需要，可以从栈加载
    auto& stack = GenContext::current_ctx->stack;
    int offset = stack.getOffset(temp_name);
    std::cout << "  lw t0, " << offset << "(sp)";
}



void AllocIRValue::To_RiscV() const{
    // alloc 不生成代码，空间已在 prologue 分配
}


void StoreIRValue::To_RiscV() const{
    auto& stack = GenContext::current_ctx->stack;
    
    // 把值加载到 t0
    if (auto* int_val = dynamic_cast<IntegerIRValue*>(value.get())) {
        std::cout << "  li t0, " << int_val->value << std::endl;
    }
    else if (auto* temp = dynamic_cast<TemporaryIRValue*>(value.get())) {
        // 检查是否是寄存器参数 (前8个参数，从 a0-a7 读取)
        int reg_num = stack.getParamReg(temp->temp_name);
        if (reg_num >= 0) {
            std::cout << "  mv t0, a" << reg_num << std::endl;
        } else {
            // 检查是否是栈上参数 (第9个及以后)
            int stack_param_offset = stack.getStackParamOffset(temp->temp_name);
            if (stack_param_offset >= 0) {
                // 从调用者栈帧读取: sp + frame_size + stack_param_offset
                int frame_size = stack.getFrameSize();
                int offset = frame_size + stack_param_offset;
                if (offset >= 2048) {
                    std::cout << "  li t1, " << offset << std::endl;
                    std::cout << "  add t1, sp, t1" << std::endl;
                    std::cout << "  lw t0, 0(t1)" << std::endl;
                } else {
                    std::cout << "  lw t0, " << offset << "(sp)" << std::endl;
                }
            } else {
                // 普通临时变量，从当前栈帧读取
                int src_offset = stack.getOffset(temp->temp_name);
                if (src_offset >= 2048) {
                    std::cout << "  li t1, " << src_offset << std::endl;
                    std::cout << "  add t1, sp, t1" << std::endl;
                    std::cout << "  lw t0, 0(t1)" << std::endl;
                } else {
                    std::cout << "  lw t0, " << src_offset << "(sp)" << std::endl;
                }
            }
        }
    }
    
    // 存入目标地址
    // 检查是否是全局变量 (以 @ 开头)
    if (dest.length() > 0 && dest[0] == '@') {
        std::string name = dest.substr(1);
        std::cout << "  la t1, " << name << std::endl;
        std::cout << "  sw t0, 0(t1)" << std::endl;
    } else {
        // 检查是否是指针变量（getelemptr 的结果，通常以 %ptr 开头）
        bool is_pointer = (dest.find("%ptr") == 0);
        
        int dest_offset = stack.getOffset(dest);
        if (is_pointer) {
            // 指针变量：需要先加载指针的值（地址），然后存储到那个地址
            if (dest_offset >= 2048) {
                std::cout << "  li t1, " << dest_offset << std::endl;
                std::cout << "  add t1, sp, t1" << std::endl;
                std::cout << "  lw t1, 0(t1)" << std::endl;
            } else {
                std::cout << "  lw t1, " << dest_offset << "(sp)" << std::endl;
            }
            std::cout << "  sw t0, 0(t1)" << std::endl;
        } else {
            // 普通变量：直接存储到栈上的位置
            if (dest_offset >= 2048) {
                std::cout << "  li t1, " << dest_offset << std::endl;
                std::cout << "  add t1, sp, t1" << std::endl;
                std::cout << "  sw t0, 0(t1)" << std::endl;
            } else {
                std::cout << "  sw t0, " << dest_offset << "(sp)" << std::endl;
            }
        }
    }
}


void LoadIRValue::To_RiscV() const{
    auto& stack = GenContext::current_ctx->stack;
    int dest_offset = stack.getOffset(result_name);
    
    // 检查是否是全局变量 (以 @ 开头)
    if (src.length() > 0 && src[0] == '@') {
        std::string name = src.substr(1);
        std::cout << "  la t0, " << name << std::endl;
        std::cout << "  lw t0, 0(t0)" << std::endl;
    } else {
        // 检查是否是指针变量（getelemptr 的结果）
        bool is_pointer = (src.find("%ptr") == 0);
        
        int src_offset = stack.getOffset(src);
        if (is_pointer) {
            // 指针变量：先加载指针的值（地址），然后从那个地址加载值
            if (src_offset >= 2048) {
                std::cout << "  li t1, " << src_offset << std::endl;
                std::cout << "  add t1, sp, t1" << std::endl;
                std::cout << "  lw t0, 0(t1)" << std::endl;
            } else {
                std::cout << "  lw t0, " << src_offset << "(sp)" << std::endl;
            }
            // t0 现在是指针的值（地址），从这个地址加载实际的值
            std::cout << "  lw t0, 0(t0)" << std::endl;
        } else {
            // 普通变量：直接从栈上加载
            if (src_offset >= 2048) {
                std::cout << "  li t1, " << src_offset << std::endl;
                std::cout << "  add t1, sp, t1" << std::endl;
                std::cout << "  lw t0, 0(t1)" << std::endl;
            } else {
                std::cout << "  lw t0, " << src_offset << "(sp)" << std::endl;
            }
        }
    }
    
    // 存储加载的值到结果变量
    if (dest_offset >= 2048) {
        std::cout << "  li t1, " << dest_offset << std::endl;
        std::cout << "  add t1, sp, t1" << std::endl;
        std::cout << "  sw t0, 0(t1)" << std::endl;
    } else {
        std::cout << "  sw t0, " << dest_offset << "(sp)" << std::endl;
    }
}


void JumpIRValue::To_RiscV() const {
    std::string label = target_label;
    if (label.length() > 0 && label[0] == '%') {
        label = label.substr(1);
    }
    std::cout << "  j .L" << label << std::endl;
}



void BranchIRValue::To_RiscV() const {
    auto& stack = GenContext::current_ctx->stack;
    
    if (auto* int_val = dynamic_cast<IntegerIRValue*>(condition.get())) {
        std::cout << "  li t0, " << int_val->value << std::endl;
    } else if (auto* temp = dynamic_cast<TemporaryIRValue*>(condition.get())) {
        int offset = stack.getOffset(temp->temp_name);
        if (offset >= 2048) {
            std::cout << "  li t0, " << offset << std::endl;
            std::cout << "  add t0, sp, t0" << std::endl;
            std::cout << "  lw t0, 0(t0)" << std::endl;
        } else {
            std::cout << "  lw t0, " << offset << "(sp)" << std::endl;
        }
    }
    
    std::string t_label = true_label;
    if (t_label.length() > 0 && t_label[0] == '%') t_label = t_label.substr(1);
    
    std::string f_label = false_label;
    if (f_label.length() > 0 && f_label[0] == '%') f_label = f_label.substr(1);

    std::cout << "  bnez t0, .L" << t_label << std::endl;
    std::cout << "  j .L" << f_label << std::endl;
}


void CallIRValue::To_RiscV() const {
    auto& stack = GenContext::current_ctx->stack;
    
    // 1. Prepare arguments
    for (size_t i = 0; i < funcrparams.size(); ++i) {
        if (i < 8) {
            // 前 8 个参数通过 a0-a7 传递
            if (auto* int_val = dynamic_cast<IntegerIRValue*>(funcrparams[i].get())) {
                std::cout << "  li a" << i << ", " << int_val->value << std::endl;
            } else if (auto* temp = dynamic_cast<TemporaryIRValue*>(funcrparams[i].get())) {
                int offset = stack.getOffset(temp->temp_name);
                if (offset >= 2048) {
                    std::cout << "  li t0, " << offset << std::endl;
                    std::cout << "  add t0, sp, t0" << std::endl;
                    std::cout << "  lw a" << i << ", 0(t0)" << std::endl;
                } else {
                    std::cout << "  lw a" << i << ", " << offset << "(sp)" << std::endl;
                }
            }
        } else {
            // 第 9 个及以后的参数通过栈传递
            // 存放位置: sp + (i - 8) * 4
            if (auto* int_val = dynamic_cast<IntegerIRValue*>(funcrparams[i].get())) {
                std::cout << "  li t0, " << int_val->value << std::endl;
            } else if (auto* temp = dynamic_cast<TemporaryIRValue*>(funcrparams[i].get())) {
                int offset = stack.getOffset(temp->temp_name);
                if (offset >= 2048) {
                    std::cout << "  li t1, " << offset << std::endl;
                    std::cout << "  add t1, sp, t1" << std::endl;
                    std::cout << "  lw t0, 0(t1)" << std::endl;
                } else {
                    std::cout << "  lw t0, " << offset << "(sp)" << std::endl;
                }
            }
            
            int arg_offset = (i - 8) * 4;
            if (arg_offset >= 2048) {
                std::cout << "  li t1, " << arg_offset << std::endl;
                std::cout << "  add t1, sp, t1" << std::endl;
                std::cout << "  sw t0, 0(t1)" << std::endl;
            } else {
                std::cout << "  sw t0, " << arg_offset << "(sp)" << std::endl;
            }
        }
    }
    
    // 2. Call
    std::string name = func_name;
    if (name.length() > 0 && name[0] == '@') name = name.substr(1);
    std::cout << "  call " << name << std::endl;
    
    // 3. Handle return value
    if (type == OTHER) {
        int offset = stack.getOffset(result_name);
        if (offset >= 2048) {
            std::cout << "  li t1, " << offset << std::endl;
            std::cout << "  add t1, sp, t1" << std::endl;
            std::cout << "  sw a0, 0(t1)" << std::endl;
        } else {
            std::cout << "  sw a0, " << offset << "(sp)" << std::endl;
        }
    }
}


void GlobalAllocIRValue::To_RiscV() const {
    if(type == VAR){
        std::string name = var_name;
        if(name.length() > 0)name = name.substr(1);
        std::cout<<"  .global "<< name <<std::endl;
        std::cout<<name<<":"<<std::endl;
        std::cout<<"  ";
        if(auto val = dynamic_cast<IntegerIRValue*>(value.get())){
            if(val->value == 0){
                std::cout<<".zero 4"<<std::endl;
            }
            else{
                std::cout<<".word "<<val->value<<std::endl;
            }
        }
    }    
    else {
        std::string name = var_name;
        if(name.length() > 0)name = name.substr(1);
        std::cout<<"  .global "<< name <<std::endl;
        std::cout<<name<<":"<<std::endl;
        
        // 计算需要的总字节数
        int total_bytes = 0;
        if (!init_list.empty()) {
            total_bytes = init_list.size() * 4;
        } else {
            total_bytes = float_size * 4 ;
        }
        
        if (!init_list.empty()) {
            // 有初始化列表，逐个输出
            for(auto i : init_list){
                std::cout<<"  .word "<<i<<std::endl;
            }
        } else if (total_bytes > 0) {
            // 没有初始化列表，使用 .zero
            std::cout<<"  .zero "<<total_bytes<<std::endl;
        }
        std::cout<<std::endl;
    }
}




void GetElemPtrIRValue::To_RiscV() const {
    auto& stack = GenContext::current_ctx->stack;
    int dest_offset = stack.getOffset(result_name);
    
    // 1. Calculate offset (index * 4) -> t1
    if (auto* int_val = dynamic_cast<IntegerIRValue*>(index.get())) {
        int offset_bytes = int_val->value * 4;
        if (offset_bytes >= 2048 || offset_bytes <= -2048) {
            std::cout << "  li t1, " << offset_bytes << std::endl;
        } else {
            std::cout << "  li t1, " << offset_bytes << std::endl;
        }
    } else if (auto* temp = dynamic_cast<TemporaryIRValue*>(index.get())) {
        int idx_offset = stack.getOffset(temp->temp_name);
        if (idx_offset >= 2048) {
            std::cout << "  li t2, " << idx_offset << std::endl;
            std::cout << "  add t2, sp, t2" << std::endl;
            std::cout << "  lw t1, 0(t2)" << std::endl;
        } else {
            std::cout << "  lw t1, " << idx_offset << "(sp)" << std::endl;
        }
        std::cout << "  slli t1, t1, 2" << std::endl;
    }
    
    // 2. Calculate base address -> t0
    if (src.length() > 0 && src[0] == '@') {
        // Global array
        std::string name = src.substr(1);
        std::cout << "  la t0, " << name << std::endl;
    } else {
        // Local
        // Check if it is a parameter
        int reg_param = stack.getParamReg(src);
        int stack_param = stack.getStackParamOffset(src);
        
        if (reg_param >= 0) {
            // Parameter in register (it's a pointer)
            std::cout << "  mv t0, a" << reg_param << std::endl;
        } else if (stack_param >= 0) {
            // Parameter on stack (it's a pointer)
            int frame_size = stack.getFrameSize();
            int offset = frame_size + stack_param;
            if (offset >= 2048) {
                std::cout << "  li t2, " << offset << std::endl;
                std::cout << "  add t2, sp, t2" << std::endl;
                std::cout << "  lw t0, 0(t2)" << std::endl;
            } else {
                std::cout << "  lw t0, " << offset << "(sp)" << std::endl;
            }
        } else {
            // Not a parameter.
            // Check if it is a temporary (getelemptr result, starts with %ptr or %[0-9])
            // bool is_temp_ptr = (src.length() > 1 && src[0] == '%');
            
            int src_offset = stack.getOffset(src);
            
            if (src_offset < 0) {
                // 变量未找到，可能是错误
                std::cerr << "Warning: variable " << src << " not found in stack frame" << std::endl;
                return;
            }
            
            // 判断是指针还是数组：如果 src 以 %ptr 开头或者是前一个 getelemptr 的结果，则是指针
            // 否则是局部数组（如 %arr）
            bool is_pointer = (src.find("%ptr") == 0) || (src.length() > 1 && src[0] == '%' && isdigit(src[1]));
            
            if (is_pointer) {
                // Temporary pointer variable -> Load pointer value
                if (src_offset >= 2048) {
                    std::cout << "  li t2, " << src_offset << std::endl;
                    std::cout << "  add t2, sp, t2" << std::endl;
                    std::cout << "  lw t0, 0(t2)" << std::endl;
                } else {
                    std::cout << "  lw t0, " << src_offset << "(sp)" << std::endl;
                }
            } else {
                // Local Array (like %arr) -> Calculate address (sp + offset)
                if (src_offset >= 2048) {
                    std::cout << "  li t0, " << src_offset << std::endl;
                    std::cout << "  add t0, sp, t0" << std::endl;
                } else {
                    std::cout << "  addi t0, sp, " << src_offset << std::endl;
                }
            }
        }
    }
    
    // 3. Add base + offset -> t0
    std::cout << "  add t0, t0, t1" << std::endl;
    
    // 4. Store result
    if (dest_offset >= 2048) {
        std::cout << "  li t1, " << dest_offset << std::endl;
        std::cout << "  add t1, sp, t1" << std::endl;
        std::cout << "  sw t0, 0(t1)" << std::endl;
    } else {
        std::cout << "  sw t0, " << dest_offset << "(sp)" << std::endl;
    }
} 