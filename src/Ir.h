#pragma once

#include <memory>
#include <vector>
#include <string>

// enum class IRValue {
//     Interger,
//     ZeroInit,
//     FuncArgRef,
//     GlobalAlloc,
//     Alloc,
//     Load,
//     Store,
//     GetPtr,
//     GetElemPtr,
//     Binary,
//     Branch,
//     Jump,
//     Call,
//     Return
// };

class BaseIRValue {
public:
    virtual ~BaseIRValue() = default;
    virtual void Dump() const = 0;
};

class ReturnIRValue : public BaseIRValue{
public:
    void Dump() const override;
};

class IntegerIRValue : public BaseIRValue{
public:
    int value;
    void Dump() const override;
};

class IRBasicBlock {
public:
    std::string block_name;
    std::vector<std::unique_ptr<BaseIRValue>> ir_value;
    void ADD_Value(std::unique_ptr<BaseIRValue> val);
    void DumpValue() const;
};

class IRFunction {
public:
    std::string function_name;
    std::string functype;
    std::vector<std::unique_ptr<IRBasicBlock>> ir_basicblock;
    void ADD_Block(std::unique_ptr<IRBasicBlock> block);
    void DumpBlock() const;
};

class IRProgram {
public:
    std::vector<std::unique_ptr<BaseIRValue>> golbal_value;
    std::vector<std::unique_ptr<IRFunction>> ir_function;
    void ADD_Function(std::unique_ptr<IRFunction> func);
    void DumpFunction() const;
};



