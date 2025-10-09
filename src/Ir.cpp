#include "include.h"
#include "Ir.h"


// 各类型IR Dump() 函数的定义


void ReturnIRValue::Dump() const{
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



void IRBasicBlock::DumpValue() const{
    std::cout << block_name << " :" << std::endl;
    for(const auto &val : ir_value){
        val -> Dump();
    }    
}


void IRFunction::DumpBlock() const{
    std::cout << "fun @" << function_name << "() : " << functype << " {\n";
    for(const auto &block : ir_basicblock){
        block -> DumpValue();
    }
    std::cout << "\n}";
}


void IRProgram::DumpFunction() const{
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
    std::cout<<"  .global "<<function_name<<std::endl;
    std::cout<<function_name<<":\n";
    for(const auto &block : ir_basicblock){
        block ->To_RiscV();
    }
}


void IRBasicBlock::To_RiscV() const{
    for(const auto &val : ir_value){
        val ->To_RiscV();
        std::cout<<std::endl;
    }
}


void ReturnIRValue::To_RiscV() const{
    if(auto int_val = dynamic_cast<IntegerIRValue*>(return_value.get())) {
        if(int_val->value == 0) {
            std::cout << "  mv a0, x0";
        } else {
            std::cout << "  li a0, " << int_val->value;
        }
    } else {
        std::cout << "  mv a0, ";
        return_value->To_RiscV();
    }
    std::cout << std::endl;
    std::cout << "  ret";
}


void IntegerIRValue::To_RiscV() const{
        std::cout<<value;
}


void BinaryIRValue::To_RiscV() const {
    char reg_num = result_name[1];
    reg_num = char('0' + (((reg_num)-'0') % 6));
    auto left_int = dynamic_cast<IntegerIRValue*>(left.get());
    auto right_int = dynamic_cast<IntegerIRValue*>(right.get());
    
    // 根据操作类型选择指令名称
    std::string op_name;
    switch (operation) {
        case ADD: op_name = "add"; break;
        case SUB: op_name = "sub"; break;
        case MUL: op_name = "mul"; break;
        case DIV: op_name = "div"; break;
        case MOD: op_name = "rem"; break;
        case LT:  op_name = "slt"; break;
        case GT:  op_name = "sgt"; break;
        case AND: op_name = "and"; break;
        case OR:  op_name = "or";  break;
        
        // 比较操作需要特殊处理
        case EQ:
        case NE:
        case LE:
        case GE:
            emitComparisonOp(operation, reg_num, left_int, right_int);
            return;
            
        default:
            std::cout << "  # Unknown operation" << std::endl;
            return;
    }
    
    // 统一处理算术和逻辑运算
    emitBinaryOp(op_name, reg_num, left_int, right_int);
}

// 辅助方法：生成二元运算指令
void BinaryIRValue::emitBinaryOp(const std::string& op_name, char reg_num,
                                   IntegerIRValue* left_int, IntegerIRValue* right_int) const {
    // 情况1：两个立即数
    if (left_int && right_int) {
        emitTwoImmediates(op_name, reg_num, left_int->value, right_int->value);
    }
    // 情况2：左立即数，右寄存器
    else if (left_int) {
        emitLeftImmediate(op_name, reg_num, left_int->value);
    }
    // 情况3：左寄存器，右立即数
    else if (right_int) {
        emitRightImmediate(op_name, reg_num, right_int->value);
    }
    // 情况4：两个寄存器
    else {
        emitTwoRegisters(op_name, reg_num);
    }
}

// 处理两个立即数的情况
void BinaryIRValue::emitTwoImmediates(const std::string& op_name, char reg_num,
                                       int left_val, int right_val) const {
    // 零值优化
    if (left_val == 0 && right_val == 0) {
        std::cout << "  " << op_name << " t" << reg_num << ", x0, x0";
        return;
    }
    
    // 加法/减法的零值优化
    if (op_name == "add" || op_name == "sub") {
        if (left_val == 0) {
            if (op_name == "sub") {
                // 0 - right = -right
                std::cout << "  li t6, " << right_val << std::endl;
                std::cout << "  sub t" << reg_num << ", x0, t6";
            } else {
                // 0 + right = right
                std::cout << "  li t" << reg_num << ", " << right_val;
            }
            return;
        }
        if (right_val == 0) {
            // left +/- 0 = left
            std::cout << "  li t" << reg_num << ", " << left_val;
            return;
        }
    }
    
    // 乘法的特殊值优化
    if (op_name == "mul") {
        if (left_val == 0 || right_val == 0) {
            std::cout << "  mul t" << reg_num << ", x0, x0";
            return;
        }
        if (left_val == 1) {
            std::cout << "  li t" << reg_num << ", " << right_val;
            return;
        }
        if (right_val == 1) {
            std::cout << "  li t" << reg_num << ", " << left_val;
            return;
        }
    }
    
    // 除法的特殊值优化
    if (op_name == "div") {
        if (left_val == 0) {
            std::cout << "  li t6, " << right_val << std::endl;
            std::cout << "  div t" << reg_num << ", x0, t6";
            return;
        }
        if (right_val == 1) {
            std::cout << "  li t" << reg_num << ", " << left_val;
            return;
        }
    }
    
    // 取模的特殊值优化
    if (op_name == "rem" && left_val == 0) {
        std::cout << "  li t6, " << right_val << std::endl;
        std::cout << "  rem t" << reg_num << ", x0, t6";
        return;
    }
    
    // 通用情况：加载两个立即数并计算
    std::cout << "  li t" << reg_num << ", " << left_val << std::endl;
    std::cout << "  li t6, " << right_val << std::endl;
    std::cout << "  " << op_name << " t" << reg_num << ", t" << reg_num << ", t6";
}

// 处理左立即数的情况
void BinaryIRValue::emitLeftImmediate(const std::string& op_name, char reg_num, int left_val) const {
    // 零值优化
    if (left_val == 0) {
        std::cout << "  " << op_name << " t" << reg_num << ", x0, ";
        right->To_RiscV();
        return;
    }
    
    // 乘法/除法的1优化
    if ((op_name == "mul" || op_name == "div") && left_val == 1) {
        std::cout << "  mv t" << reg_num << ", ";
        right->To_RiscV();
        return;
    }
    
    // 通用情况
    std::cout << "  li t" << reg_num << ", " << left_val << std::endl;
    std::cout << "  " << op_name << " t" << reg_num << ", t" << reg_num << ", ";
    right->To_RiscV();
}

// 处理右立即数的情况
void BinaryIRValue::emitRightImmediate(const std::string& op_name, char reg_num, int right_val) const {
    // 零值优化
    if (right_val == 0) {
        std::cout << "  " << op_name << " t" << reg_num << ", ";
        left->To_RiscV();
        std::cout << ", x0";
        return;
    }
    
    // 乘法/除法的1优化
    if ((op_name == "mul" || op_name == "div") && right_val == 1) {
        std::cout << "  mv t" << reg_num << ", ";
        left->To_RiscV();
        return;
    }
    
    // 通用情况
    std::cout << "  li t6, " << right_val << std::endl;
    std::cout << "  " << op_name << " t" << reg_num << ", ";
    left->To_RiscV();
    std::cout << ", t6";
}

// 处理两个寄存器的情况
void BinaryIRValue::emitTwoRegisters(const std::string& op_name, char reg_num) const {
    std::cout << "  " << op_name << " t" << reg_num << ", ";
    left->To_RiscV();
    std::cout << ", ";
    right->To_RiscV();
}

// 处理比较操作
void BinaryIRValue::emitComparisonOp(Operation op, char reg_num,
                                      IntegerIRValue* left_int, IntegerIRValue* right_int) const {
    // 加载左操作数
    if (left_int) {
        std::cout << "  li t" << reg_num << ", " << left_int->value;
    } else {
        std::cout << "  mv t" << reg_num << ", ";
        left->To_RiscV();
    }
    std::cout << std::endl;
    
    // 加载右操作数（如果是立即数）
    if (right_int) {
        std::cout << "  li t6, " << right_int->value << std::endl;
    }
    
    // 生成比较指令
    switch (op) {
        case EQ:
            // a == b  =>  xor + seqz
            std::cout << "  xor t" << reg_num << ", t" << reg_num << ", ";
            if (right_int) {
                std::cout << "t6";
            } else {
                right->To_RiscV();
            }
            std::cout << std::endl;
            std::cout << "  seqz t" << reg_num << ", t" << reg_num;
            break;
            
        case NE:
            // a != b  =>  xor + snez
            std::cout << "  xor t" << reg_num << ", t" << reg_num << ", ";
            if (right_int) {
                std::cout << "t6";
            } else {
                right->To_RiscV();
            }
            std::cout << std::endl;
            std::cout << "  snez t" << reg_num << ", t" << reg_num;
            break;
            
        case LE:
            // a <= b  =>  sgt + xori 1
            std::cout << "  sgt t" << reg_num << ", t" << reg_num << ", ";
            if (right_int) {
                std::cout << "t6";
            } else {
                right->To_RiscV();
            }
            std::cout << std::endl;
            std::cout << "  xori t" << reg_num << ", t" << reg_num << ", 1";
            break;
            
        case GE:
            // a >= b  =>  slt + xori 1
            std::cout << "  slt t" << reg_num << ", t" << reg_num << ", ";
            if (right_int) {
                std::cout << "t6";
            } else {
                right->To_RiscV();
            }
            std::cout << std::endl;
            std::cout << "  xori t" << reg_num << ", t" << reg_num << ", 1";
            break;
            
        default:
            break;
    }
}

void TemporaryIRValue::To_RiscV() const{
    std::cout<<"t" << char(((temp_name[1] - '0') % 6) + '0');
}