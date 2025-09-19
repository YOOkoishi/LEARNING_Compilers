#include "include.h"
#include "Ir.h"

void ReturnIRValue::Dump() const{
    std::cout<<"ret "<<return_value<<" ";
}

void IntegerIRValue::Dump() const{
    std::cout<<value<<" ";
}



void IRBasicBlock::ADD_Value(std::unique_ptr<BaseIRValue> val){
    ir_value.push_back(std::move(val));
}

void IRBasicBlock::DumpValue() const{
    std::cout << block_name << " :" << std::endl << "  ";
    for(const auto &val : ir_value){
        val -> Dump();
    }    
}

void IRFunction::ADD_Block(std::unique_ptr<IRBasicBlock> block){
    ir_basicblock.push_back(std::move(block));
}

void IRFunction::DumpBlock() const{
    std::cout << "fun @" << function_name << "() : " << functype << " {\n";
    for(const auto &block : ir_basicblock){
        block -> DumpValue();
    }
    std::cout << "\n}";
}

void IRProgram::ADD_Function(std::unique_ptr<IRFunction> func){
    ir_function.push_back(std::move(func));
}

void IRProgram::DumpFunction() const{
    for(const auto &fun : ir_function){
        fun -> DumpBlock();
    }
}

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
    std::cout<<"  li a0, "<<return_value<<std::endl;
    std::cout<<"  ret";
}

void IntegerIRValue::To_RiscV() const{
    std::cout<<"  li a0, "<<value;
}