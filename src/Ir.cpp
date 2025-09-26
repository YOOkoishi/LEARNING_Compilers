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
    std::cout<<"  mv a0, ";
    return_value ->To_RiscV();
    std::cout<<std::endl;
    std::cout<<"  ret";
}

void IntegerIRValue::To_RiscV() const{
    if(!value){
        std::cout<<value;
    }
    else {
        std::cout<<"x0";
    }
}

void BinaryIRValue::To_RiscV() const{
    switch (operation)
    {
    case ADD:
        
        break;
    case SUB:
        std::cout<<"  sub t"<<result_name[1]  << ", " ; 
        left->To_RiscV();
        std::cout<<", ";
        right->To_RiscV(); 
        std::cout<< std::endl;
        break;
    case EQ:
        std::cout<<"  "<<result_name << " = " << "eq " ; 
        left->To_RiscV();
        std::cout<<", ";
        right->To_RiscV(); 
        std::cout<< std::endl;
        break;
    default:
        break;
    }
}

void TemporaryIRValue::To_RiscV() const{
    std::cout<<"t" + temp_name[1];
}