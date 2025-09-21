#pragma once

#include "include.h"

class BaseAST {
public:
    virtual ~BaseAST() = default;
    virtual void Dump() const = 0;
};

class CompUnitAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> fun_def;
    void Dump() const override;
};

class FunDefAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> fun_type;
    std::string ident;
    std::unique_ptr<BaseAST> block;
    void Dump() const override;
};

class FunTypeAST : public BaseAST {
public:
    std::string tp;
    void Dump() const override;
};

class BlockAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> stmt;
    void Dump() const override;
};

class StmtAST : public BaseAST {
public:
    std::string retrn = "return";
    std::unique_ptr<BaseAST> number;
    std::string fenhao = ";";
    void Dump() const override;
};

class NumberAST : public BaseAST {
public:
    int int_const;
    void Dump() const override;
};

class ExpAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> unaryexp;
    void Dump() const override;   
};

class PrimaryExpAST : public BaseAST{
public:
    std::unique_ptr<BaseAST> exp;
    std::unique_ptr<BaseAST> number;
    void Dump() const override;
};

class UnaryExpAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> primaryexp;
    std::unique_ptr<BaseAST> unaryop;
    std::unique_ptr<BaseAST> unaryexp;
    void Dump() const override;
};

class UnaryOpAST : public BaseAST {
public:
    std::string op;
    void Dump() const override;
};
