#include "include.h"
#include "Ir.h"

void ReturnIRValue::Dump() const{
    std::cout<<"ret ";
}

void IntegerIRValue::Dump() const{
    std::cout<<value<<" ";
}



void IRBasicBlock::ADD_Value(std::unique_ptr<BaseIRValue> val){
    ir_value.push_back(val);
}

void IRBasicBlock::DumpValue() const{
    std::cout << block_name << " :/n/t";
    for(const auto &val : ir_value){
        val -> Dump();
    }    
}

void IRFunction::ADD_Block(std::unique_ptr<IRBasicBlock> block){
    ir_basicblock.push_back(block);
}

void IRFunction::DumpBlock() const{
    std::cout << "fun" << function_name << " : " << functype << "{/n/t";
    for(const auto &block : ir_basicblock){
        block -> DumpValue();
    }
    std::cout << "}/n";
}

void IRProgram::ADD_Function(std::unique_ptr<IRFunction> func){
    ir_function.push_back(func);
}

void IRProgram::DumpFunction() const{
    for(const auto &fun : ir_function){
        fun -> DumpBlock();
    }
}