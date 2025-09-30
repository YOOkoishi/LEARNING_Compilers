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


void BinaryIRValue::To_RiscV() const{
    char reg_num = result_name[1];
    
    switch (operation) {
    case ADD: {
        auto left_int = dynamic_cast<IntegerIRValue*>(left.get());
        auto right_int = dynamic_cast<IntegerIRValue*>(right.get());
        
        if(left_int && right_int) {
            // 两个立即数：优化处理
            if(left_int->value == 0 && right_int->value == 0) {
                std::cout << "  add t" << reg_num << ", x0, x0";
            } else if(left_int->value == 0) {
                // 0 + 右立即数 = 右立即数
                std::cout << "  li t" << reg_num << ", " << right_int->value;
            } else if(right_int->value == 0) {
                // 左立即数 + 0 = 左立即数
                std::cout << "  li t" << reg_num << ", " << left_int->value;
            } else {
                // 两个非零立即数：使用固定临时寄存器 t6
                std::cout << "  li t" << reg_num << ", " << left_int->value << std::endl;
                std::cout << "  li t6, " << right_int->value << std::endl;
                std::cout << "  add t" << reg_num << ", t" << reg_num << ", t6";
            }
        } else if(left_int) {
            // 左操作数是立即数，右操作数是寄存器
            if(left_int->value == 0) {
                std::cout << "  add t" << reg_num << ", x0, ";
                right->To_RiscV();
            } else {
                std::cout << "  li t" << reg_num << ", " << left_int->value << std::endl;
                std::cout << "  add t" << reg_num << ", t" << reg_num << ", ";
                right->To_RiscV();
            }
        } else if(right_int) {
            // 左操作数是寄存器，右操作数是立即数
            if(right_int->value == 0) {
                std::cout << "  add t" << reg_num << ", ";
                left->To_RiscV();
                std::cout << ", x0";
            } else {
                // 使用固定临时寄存器 t6
                std::cout << "  li t6, " << right_int->value << std::endl;
                std::cout << "  add t" << reg_num << ", ";
                left->To_RiscV();
                std::cout << ", t6";
            }
        } else {
            // 两个寄存器
            std::cout << "  add t" << reg_num << ", ";
            left->To_RiscV();
            std::cout << ", ";
            right->To_RiscV();
        }
        break;
    }

    case SUB: {
        auto left_int = dynamic_cast<IntegerIRValue*>(left.get());
        auto right_int = dynamic_cast<IntegerIRValue*>(right.get());
        
        if(left_int && right_int) {
            // 两个立即数
            if(left_int->value == 0 && right_int->value == 0) {
                std::cout << "  sub t" << reg_num << ", x0, x0";
            } else if(left_int->value == 0) {
                // 0 - 右立即数 = -右立即数
                std::cout << "  li t6, " << right_int->value << std::endl;
                std::cout << "  sub t" << reg_num << ", x0, t6";
            } else if(right_int->value == 0) {
                // 左立即数 - 0 = 左立即数
                std::cout << "  li t" << reg_num << ", " << left_int->value;
            } else {
                // 两个非零立即数
                std::cout << "  li t" << reg_num << ", " << left_int->value << std::endl;
                std::cout << "  li t6, " << right_int->value << std::endl;
                std::cout << "  sub t" << reg_num << ", t" << reg_num << ", t6";
            }
        } else if(left_int) {
            // 左操作数是立即数
            if(left_int->value == 0) {
                std::cout << "  sub t" << reg_num << ", x0, ";
                right->To_RiscV();
            } else {
                std::cout << "  li t" << reg_num << ", " << left_int->value << std::endl;
                std::cout << "  sub t" << reg_num << ", t" << reg_num << ", ";
                right->To_RiscV();
            }
        } else if(right_int) {
            // 右操作数是立即数
            if(right_int->value == 0) {
                std::cout << "  sub t" << reg_num << ", ";
                left->To_RiscV();
                std::cout << ", x0";
            } else {
                std::cout << "  li t6, " << right_int->value << std::endl;
                std::cout << "  sub t" << reg_num << ", ";
                left->To_RiscV();
                std::cout << ", t6";
            }
        } else {
            // 两个寄存器
            std::cout << "  sub t" << reg_num << ", ";
            left->To_RiscV();
            std::cout << ", ";
            right->To_RiscV();
        }
        break;
    }

    case MUL: {
        auto left_int = dynamic_cast<IntegerIRValue*>(left.get());
        auto right_int = dynamic_cast<IntegerIRValue*>(right.get());
        
        if(left_int && right_int) {
            // 两个立即数
            if(left_int->value == 0 || right_int->value == 0) {
                // 任何数乘以0都是0
                std::cout << "  mul t" << reg_num << ", x0, x0";
            } else if(left_int->value == 1) {
                // 1 * 右立即数 = 右立即数
                std::cout << "  li t" << reg_num << ", " << right_int->value;
            } else if(right_int->value == 1) {
                // 左立即数 * 1 = 左立即数
                std::cout << "  li t" << reg_num << ", " << left_int->value;
            } else {
                // 两个非零非1立即数
                std::cout << "  li t" << reg_num << ", " << left_int->value << std::endl;
                std::cout << "  li t6, " << right_int->value << std::endl;
                std::cout << "  mul t" << reg_num << ", t" << reg_num << ", t6";
            }
        } else if(left_int) {
            // 左操作数是立即数
            if(left_int->value == 0) {
                std::cout << "  mul t" << reg_num << ", x0, ";
                right->To_RiscV();
            } else if(left_int->value == 1) {
                std::cout << "  mv t" << reg_num << ", ";
                right->To_RiscV();
            } else {
                std::cout << "  li t" << reg_num << ", " << left_int->value << std::endl;
                std::cout << "  mul t" << reg_num << ", t" << reg_num << ", ";
                right->To_RiscV();
            }
        } else if(right_int) {
            // 右操作数是立即数
            if(right_int->value == 0) {
                std::cout << "  mul t" << reg_num << ", ";
                left->To_RiscV();
                std::cout << ", x0";
            } else if(right_int->value == 1) {
                std::cout << "  mv t" << reg_num << ", ";
                left->To_RiscV();
            } else {
                std::cout << "  li t6, " << right_int->value << std::endl;
                std::cout << "  mul t" << reg_num << ", ";
                left->To_RiscV();
                std::cout << ", t6";
            }
        } else {
            // 两个寄存器
            std::cout << "  mul t" << reg_num << ", ";
            left->To_RiscV();
            std::cout << ", ";
            right->To_RiscV();
        }
        break;
    }

    case DIV: {
        auto left_int = dynamic_cast<IntegerIRValue*>(left.get());
        auto right_int = dynamic_cast<IntegerIRValue*>(right.get());
        
        if(left_int && right_int) {
            // 两个立即数
            if(left_int->value == 0) {
                // 0 / 任何数 = 0
                std::cout << "  div t" << reg_num << ", x0, ";
                if(right_int->value == 0) {
                    std::cout << "x0";  // 0/0 未定义，但生成指令
                } else {
                    std::cout << std::endl << "  li t6, " << right_int->value << std::endl;
                    std::cout << "  div t" << reg_num << ", x0, t6";
                    return;
                }
            } else if(right_int->value == 1) {
                // 任何数 / 1 = 原数
                std::cout << "  li t" << reg_num << ", " << left_int->value;
            } else {
                // 一般情况
                std::cout << "  li t" << reg_num << ", " << left_int->value << std::endl;
                std::cout << "  li t6, " << right_int->value << std::endl;
                std::cout << "  div t" << reg_num << ", t" << reg_num << ", t6";
            }
        } else if(left_int) {
            // 左操作数是立即数
            if(left_int->value == 0) {
                std::cout << "  div t" << reg_num << ", x0, ";
                right->To_RiscV();
            } else {
                std::cout << "  li t" << reg_num << ", " << left_int->value << std::endl;
                std::cout << "  div t" << reg_num << ", t" << reg_num << ", ";
                right->To_RiscV();
            }
        } else if(right_int) {
            // 右操作数是立即数
            if(right_int->value == 1) {
                std::cout << "  mv t" << reg_num << ", ";
                left->To_RiscV();
            } else {
                std::cout << "  li t6, " << right_int->value << std::endl;
                std::cout << "  div t" << reg_num << ", ";
                left->To_RiscV();
                std::cout << ", t6";
            }
        } else {
            // 两个寄存器
            std::cout << "  div t" << reg_num << ", ";
            left->To_RiscV();
            std::cout << ", ";
            right->To_RiscV();
        }
        break;
    }

    case MOD: {
        auto left_int = dynamic_cast<IntegerIRValue*>(left.get());
        auto right_int = dynamic_cast<IntegerIRValue*>(right.get());
        
        if(left_int && right_int) {
            // 两个立即数
            if(left_int->value == 0) {
                // 0 % 任何数 = 0
                std::cout << "  rem t" << reg_num << ", x0, ";
                if(right_int->value == 0) {
                    std::cout << "x0";  // 0%0 未定义，但生成指令
                } else {
                    std::cout << std::endl << "  li t6, " << right_int->value << std::endl;
                    std::cout << "  rem t" << reg_num << ", x0, t6";
                    return;
                }
            } else {
                // 一般情况
                std::cout << "  li t" << reg_num << ", " << left_int->value << std::endl;
                std::cout << "  li t6, " << right_int->value << std::endl;
                std::cout << "  rem t" << reg_num << ", t" << reg_num << ", t6";
            }
        } else if(left_int) {
            // 左操作数是立即数
            if(left_int->value == 0) {
                std::cout << "  rem t" << reg_num << ", x0, ";
                right->To_RiscV();
            } else {
                std::cout << "  li t" << reg_num << ", " << left_int->value << std::endl;
                std::cout << "  rem t" << reg_num << ", t" << reg_num << ", ";
                right->To_RiscV();
            }
        } else if(right_int) {
            // 右操作数是立即数
            std::cout << "  li t6, " << right_int->value << std::endl;
            std::cout << "  rem t" << reg_num << ", ";
            left->To_RiscV();
            std::cout << ", t6";
        } else {
            // 两个寄存器
            std::cout << "  rem t" << reg_num << ", ";
            left->To_RiscV();
            std::cout << ", ";
            right->To_RiscV();
        }
        break;
    }
    
    case EQ: {
        // EQ 操作：比较是否相等
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