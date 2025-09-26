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
    switch (operation)
    {
    case ADD:
        
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

void BinaryIRValue::To_RiscV() const{
    char reg_num = result_name[1];
    
    switch (operation) {
    case ADD:
    case SUB: {
        auto left_int = dynamic_cast<IntegerIRValue*>(left.get());
        auto right_int = dynamic_cast<IntegerIRValue*>(right.get());
        
        if(left_int) {
            // 左操作数是立即数
            if(left_int->value == 0) {
                // 左操作数是 0
                if(right_int) {
                    // 右操作数也是立即数
                    if(right_int->value == 0) {
                        // 0 - 0
                        std::cout << "  sub t" << reg_num << ", x0, x0";
                    } else {
                        // 0 - 非零立即数
                        char temp_reg = reg_num;
                        std::cout << "  li t" << temp_reg << ", " << right_int->value << std::endl;
                        std::cout << "  sub t" << reg_num << ", x0, t" << temp_reg;
                    }
                } else {
                    // 0 - 寄存器
                    std::cout << "  sub t" << reg_num << ", x0, ";
                    right->To_RiscV();
                }
            } else {
                // 左操作数是非零立即数
                std::cout << "  li t" << reg_num << ", " << left_int->value << std::endl;
                
                if(right_int) {
                    // 右操作数也是立即数
                    if(right_int->value == 0) {
                        // 非零立即数 - 0
                        std::cout << "  sub t" << reg_num << ", t" << reg_num << ", x0";
                    } else {
                        // 非零立即数 - 非零立即数
                        char temp_reg = reg_num;
                        std::cout << "  li t" << temp_reg << ", " << right_int->value << std::endl;
                        std::cout << "  sub t" << reg_num << ", t" << reg_num << ", t" << temp_reg;
                    }
                } else {
                    // 非零立即数 - 寄存器
                    std::cout << "  sub t" << reg_num << ", t" << reg_num << ", ";
                    right->To_RiscV();
                }
            }
        } else {
            // 左操作数是寄存器
            if(right_int) {
                // 右操作数是立即数
                if(right_int->value == 0) {
                    // 寄存器 - 0
                    std::cout << "  sub t" << reg_num << ", ";
                    left->To_RiscV();
                    std::cout << ", x0";
                } else {
                    // 寄存器 - 非零立即数
                    char temp_reg = (reg_num == '0') ? '9' : (char)(reg_num + 1);
                    std::cout << "  li t" << temp_reg << ", " << right_int->value << std::endl;
                    std::cout << "  sub t" << reg_num << ", ";
                    left->To_RiscV();
                    std::cout << ", t" << temp_reg;
                }
            } else {
                // 寄存器 - 寄存器
                std::cout << "  sub t" << reg_num << ", ";
                left->To_RiscV();
                std::cout << ", ";
                right->To_RiscV();
            }
        }
        break;
    }
    
    case EQ: {
        // EQ 操作逻辑
        if(auto left_int = dynamic_cast<IntegerIRValue*>(left.get())) {
            std::cout << "  li t" << reg_num << ", " << left_int->value;
        } else {
            std::cout << "  mv t" << reg_num << ", ";
            left->To_RiscV();
        }
        std::cout << std::endl;
        std::cout << "  xor t" << reg_num << ", t" << reg_num << ", x0" << std::endl;
        std::cout << "  seqz t" << reg_num << ", t" << reg_num;
        break;
    }
    
    default:
        break;
    }
}

void TemporaryIRValue::To_RiscV() const{
    std::cout<<"t" << temp_name[1];
}