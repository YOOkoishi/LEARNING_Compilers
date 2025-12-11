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
    std::cout << "  " << var_name << " = alloc " << type << std::endl;
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




// 各类型IR的To_RiscV() 函数定义





void IRProgram::To_RiscV() const{
    std::cout<<"  .text"<<std::endl;
    for(const auto &fun : ir_function){
        fun -> To_RiscV();
    }
}


void IRFunction::To_RiscV() const{
    GenContext ctx;
    GenContext::current_ctx = &ctx;

    for(const auto& block : ir_basicblock){
        for (const auto& value : block ->ir_value){
            if(auto* alloc = dynamic_cast<AllocIRValue*>(value.get())){
                ctx.stack.allocate(alloc->var_name);
            }   
            else if(auto* load = dynamic_cast<LoadIRValue*>(value.get())){
                ctx.stack.allocate(load->result_name);
            }
            else if(auto* binary = dynamic_cast<BinaryIRValue*>(value.get())){
                ctx.stack.allocate(binary->result_name);
            }
        }
    }

    int frame_size = ctx.stack.getAlignedSize();


    //second time
    std::string name = function_name;
    if (name.length() > 0 && name[0] == '@') {
        name = name.substr(1);
    }
    std::cout<<"  .globl "<<name<<std::endl;
    std::cout<<name<<":\n";

    if(frame_size > 0){
        std::cout << "  addi sp, sp, -" << frame_size << std::endl;
    }    

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
    if(type == ReturnIRValue::VALUE){
        
        auto& stack = GenContext::current_ctx->stack;
        
        if(auto int_val = dynamic_cast<IntegerIRValue*>(return_value.get())) {
            if(int_val->value == 0) {
                std::cout << "  mv a0, x0" << std::endl;
            } else {
                std::cout << "  li a0, " << int_val->value << std::endl;
            }
        } else if(auto temp = dynamic_cast<TemporaryIRValue*>(return_value.get())) {
            int offset = stack.getOffset(temp->temp_name);
            std::cout << "  lw a0, " << offset << "(sp)" << std::endl;
        }
        
        // epilogue
        int frame_size = stack.getAlignedSize();
        if (frame_size > 0) {
            std::cout << "  addi sp, sp, " << frame_size << std::endl;
        }
        std::cout << "  ret";

    }
    else {
        std::cout << "  ret"<<std::endl;
    }
    
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
        std::cout << "  lw t0, " << left_offset << "(sp)" << std::endl;
    }
    
    // 加载右操作数到 t1
    if (auto* right_int = dynamic_cast<IntegerIRValue*>(right.get())) {
        std::cout << "  li t1, " << right_int->value << std::endl;
    } else if (auto* right_temp = dynamic_cast<TemporaryIRValue*>(right.get())) {
        int right_offset = stack.getOffset(right_temp->temp_name);
        std::cout << "  lw t1, " << right_offset << "(sp)" << std::endl;
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
    std::cout << "  sw t0, " << result_offset << "(sp)";
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
    int dest_offset = stack.getOffset(dest);
    
    // 把值加载到 t0
    if (auto* int_val = dynamic_cast<IntegerIRValue*>(value.get())) {
        std::cout << "  li t0, " << int_val->value << std::endl;
    }
    else if (auto* temp = dynamic_cast<TemporaryIRValue*>(value.get())) {
        int src_offset = stack.getOffset(temp->temp_name);
        std::cout << "  lw t0, " << src_offset << "(sp)" << std::endl;
    }
    
    // 存入目标地址
    std::cout << "  sw t0, " << dest_offset << "(sp)";
}


void LoadIRValue::To_RiscV() const{
    auto& stack = GenContext::current_ctx->stack;
    int src_offset = stack.getOffset(src);
    int dest_offset = stack.getOffset(result_name);
    
    std::cout << "  lw t0, " << src_offset << "(sp)" << std::endl;
    std::cout << "  sw t0, " << dest_offset << "(sp)";
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
        std::cout << "  lw t0, " << offset << "(sp)" << std::endl;
    }
    
    std::string t_label = true_label;
    if (t_label.length() > 0 && t_label[0] == '%') t_label = t_label.substr(1);
    
    std::string f_label = false_label;
    if (f_label.length() > 0 && f_label[0] == '%') f_label = f_label.substr(1);

    std::cout << "  bnez t0, .L" << t_label << std::endl;
    std::cout << "  j .L" << f_label << std::endl;
}


void CallIRValue::To_RiscV() const {
    // 1. Prepare arguments
    for (size_t i = 0; i < funcrparams.size(); ++i) {
        if (i < 8) {
            // Pass in a0-a7
            if (auto* int_val = dynamic_cast<IntegerIRValue*>(funcrparams[i].get())) {
                std::cout << "  li a" << i << ", " << int_val->value << std::endl;
            } else if (auto* temp = dynamic_cast<TemporaryIRValue*>(funcrparams[i].get())) {
                auto& stack = GenContext::current_ctx->stack;
                int offset = stack.getOffset(temp->temp_name);
                std::cout << "  lw a" << i << ", " << offset << "(sp)" << std::endl;
            }
        } else {
            // Pass on stack (not implemented for now as per common requirements)
        }
    }
    
    // 2. Call
    std::string name = func_name;
    if (name.length() > 0 && name[0] == '@') name = name.substr(1);
    std::cout << "  call " << name << std::endl;
    
    // 3. Handle return value
    if (type == OTHER) {
        auto& stack = GenContext::current_ctx->stack;
        int offset = stack.getOffset(result_name);
        std::cout << "  sw a0, " << offset << "(sp)" << std::endl;
    }
}




