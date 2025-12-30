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







class ConstIRValue : public BaseIRValue{
public:
    std::string name;
    enum ConstType {
        INT,
        FLOAT
    } const_type;

    union {
        int int_value;
        float float_value;
    } value;

    ConstIRValue(int v) : const_type(INT), value{.int_value = v} {};
    ConstIRValue(float v) : const_type(FLOAT) , value {.float_value = v} {};
    ConstIRValue(){};

    void Dump() const override;
    void To_RiscV() const override;
};





class ReturnIRValue : public BaseIRValue{
public:
    enum Type {
        NUL,
        VALUE
    } type;

    ReturnIRValue() = default;
    ReturnIRValue(Type t): type(t) {};
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





class AllocIRValue : public BaseIRValue{
public:
    enum Type {
        VAR,
        ARRAY
    } type;

    std::string var_name;
    std::string data_type;
    int size;

    AllocIRValue(Type t,const std::string& name, const std::string& ty):type(t), var_name(name),data_type(ty){};
    AllocIRValue(const std::string& name, const std::string& ty): var_name(name),data_type(ty){type = VAR;};
    AllocIRValue() = default;

    void Dump() const override;
    void To_RiscV() const override;
};




class GlobalAllocIRValue : public BaseIRValue {
public:
    enum Type {
        VAR,
        ARRAY
    } type;

    std::vector<int> init_list; 
    std::unique_ptr<BaseIRValue> value;
    std::string var_name;
    std::string data_type;
    int float_size;

    GlobalAllocIRValue() = default;
    GlobalAllocIRValue(Type t,std::unique_ptr<BaseIRValue> v,const std::string& d,const std::string& ty) :type(t), value(std::move(v)), var_name(d),data_type(ty) {};

    void Dump() const override;
    void To_RiscV() const override;
};



class GetElemPtrIRValue : public BaseIRValue {
public :

    std::string result_name;
    std::string  src;
    std::unique_ptr<BaseIRValue> index;

    GetElemPtrIRValue() = default;
    GetElemPtrIRValue(const std::string& r,const std::string& s,std::unique_ptr<BaseIRValue> i): result_name(r),src(s),index(std::move(i)) {};
    

    void Dump() const override;
    void To_RiscV() const override;

};


class GetPtrIRValue : public BaseIRValue {
public:

    std::string result_name;
    std::string src;
    std::unique_ptr<BaseIRValue> index;

    GetPtrIRValue() = default;
    GetPtrIRValue(const std::string& r, const std::string& s,std::unique_ptr<BaseIRValue> i): result_name(r),src(s),index(std::move(i)){};

    void Dump() const override;
    void To_RiscV() const override;
};




class StoreIRValue : public BaseIRValue{
public:
    std::unique_ptr<BaseIRValue> value;  // 要存储的值
    std::string dest;                     // 目标地址变量名，如 @x

    StoreIRValue() = default;
    StoreIRValue(std::unique_ptr<BaseIRValue> v, const std::string& d)
        : value(std::move(v)), dest(d) {}

    void Dump() const override;
    void To_RiscV() const override;
};


class LoadIRValue : public BaseIRValue{
public:
    std::string src;           // 源地址变量名，如 @x
    std::string result_name;   // 结果临时变量，如 %0

    LoadIRValue() = default;
    LoadIRValue(const std::string& s, const std::string& r)
        : src(s), result_name(r) {}

    void Dump() const override;
    void To_RiscV() const override;
};




class JumpIRValue : public BaseIRValue{
public:
    std::string target_label;
    
    JumpIRValue(const std::string& target) : target_label(target) {}
    JumpIRValue() = default;

    void Dump() const override;
    void To_RiscV() const override;
};   


class BranchIRValue : public BaseIRValue{
public:
    std::unique_ptr<BaseIRValue> condition;
    std::string true_label;
    std::string false_label;

    BranchIRValue(std::unique_ptr<BaseIRValue> cond, const std::string& t_label, const std::string& f_label)
        : condition(std::move(cond)), true_label(t_label), false_label(f_label) {}
    BranchIRValue() = default;

    void Dump() const override;
    void To_RiscV() const override;
};



class CallIRValue : public BaseIRValue {
public:
    enum Type {
        VOID,
        OTHER
    } type;
    
    CallIRValue(Type t): type(t) {};
    CallIRValue() = default;

    std::string func_name;
    std::vector<std::unique_ptr<BaseIRValue>> funcrparams;
    std::string result_name;

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
    std::vector<std::pair<std::string,std::string>> funcfparams ;
    std::string functype;
    std::vector<std::unique_ptr<IRBasicBlock>> ir_basicblock;
    void ADD_Block(std::unique_ptr<IRBasicBlock> block);
    void DumpBlock() const;
    void To_RiscV() const;
};



class IRProgram {
public:
    std::vector<std::unique_ptr<BaseIRValue>> global_value;
    std::vector<std::unique_ptr<IRFunction>> ir_function;
    void ADD_Function(std::unique_ptr<IRFunction> func);
    void ADD_Globalvalue(std::unique_ptr<BaseIRValue> val); 
    void DumpFunction() const;
    void To_RiscV() const;
};



