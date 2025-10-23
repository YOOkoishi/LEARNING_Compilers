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
    virtual void To_RiscV() const = 0;
};



class ReturnIRValue : public BaseIRValue{
public:
    std::unique_ptr<BaseIRValue> return_value;  
    void Dump() const override;
    void To_RiscV() const override;
};



class IntegerIRValue : public BaseIRValue{
public:
    int value;
    IntegerIRValue(int v) : value(v){};
    IntegerIRValue(){};
    void Dump() const override;
    void To_RiscV() const override;
};



class BinaryIRValue : public BaseIRValue{
public:
    enum Operation {
        ADD , SUB , MUL , DIV , MOD,
        NE , EQ , GT , LT , GE ,LE ,
        AND , OR , XOR , SHL , SHR , SAR
    } operation;
    std::unique_ptr<BaseIRValue> left;
    std::unique_ptr<BaseIRValue> right;
    std::string result_name;
    BinaryIRValue(){};
    BinaryIRValue(
        Operation op,
        std::unique_ptr<BaseIRValue> l ,
        std::unique_ptr<BaseIRValue> r ,
        const std::string& result = ""
    )
    :    
        operation(op),
        left(std::move(l)) , 
        right(std::move(r)) , 
        result_name(result)
    {};

    void Dump() const override;
    void To_RiscV() const override;


private:
    // 辅助方法：生成不同类型的二元运算指令
    void emitBinaryOp(const std::string& op_name, char reg_num,
                      IntegerIRValue* left_int, IntegerIRValue* right_int) const;
    void emitTwoImmediates(const std::string& op_name, char reg_num,
                           int left_val, int right_val) const;
    void emitLeftImmediate(const std::string& op_name, char reg_num, int left_val) const;
    void emitRightImmediate(const std::string& op_name, char reg_num, int right_val) const;
    void emitTwoRegisters(const std::string& op_name, char reg_num) const;
    void emitComparisonOp(Operation op, char reg_num,
                          IntegerIRValue* left_int, IntegerIRValue* right_int) const;
};



class TemporaryIRValue : public BaseIRValue{
public :
    std::string temp_name;
    void Dump() const override;
    void To_RiscV() const override;
};



class IRBasicBlock {
public:
    std::string block_name;
    std::vector<std::unique_ptr<BaseIRValue>> ir_value;
    void ADD_Value(std::unique_ptr<BaseIRValue> val);
    void DumpValue() const;
    void To_RiscV() const;

};



class IRFunction {
public:
    std::string function_name;
    std::string functype;
    std::vector<std::unique_ptr<IRBasicBlock>> ir_basicblock;
    void ADD_Block(std::unique_ptr<IRBasicBlock> block);
    void DumpBlock() const;
    void To_RiscV() const;
};



class IRProgram {
public:
    std::vector<std::unique_ptr<BaseIRValue>> golbal_value;
    std::vector<std::unique_ptr<IRFunction>> ir_function;
    void ADD_Function(std::unique_ptr<IRFunction> func);
    void DumpFunction() const;
    void To_RiscV() const;
};



