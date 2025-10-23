#pragma once

#include "include.h"

//  根据 EBNF 定义 AST 中的 各种类型



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
    std::unique_ptr<BaseAST> exp;
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
    std::unique_ptr<BaseAST> lorexp;
    void Dump() const override;   
};


class PrimaryExpAST : public BaseAST{
public:
    enum Type { EXP, NUMBER } type;
    std::unique_ptr<BaseAST> exp;
    std::unique_ptr<BaseAST> number;

    PrimaryExpAST(Type t) : type(t){};

    void Dump() const override;
};


class UnaryExpAST : public BaseAST {
public:
    enum Type {PRIMARYEXP , UNARYEXP} type;

    UnaryExpAST(Type t) : type(t){};
    std::unique_ptr<BaseAST> primaryexp;
    std::string unaryop;
    std::unique_ptr<BaseAST> unaryexp;
    void Dump() const override;
};


class UnaryOpAST : public BaseAST {
public:
    char op;
    void Dump() const override;
};


class MulExpAST : public BaseAST {
public:
    enum Type {UNARYEXP , MULOPUNRAY} type;

    MulExpAST(Type t) : type(t){};
    std::unique_ptr<BaseAST> unrayexp;
    std::unique_ptr<BaseAST> mulexp;
    std::string op;

    void Dump() const override;
};


class AddExpAST : public BaseAST {
public:
    enum Type {MULONLY , ADDOPMUL} type;

    AddExpAST(Type t) : type(t){};
    std::unique_ptr<BaseAST> mulexp;
    std::unique_ptr<BaseAST> addexp;
    std::string op;

    void Dump() const override;
};


class LOrExpAST : public BaseAST {
public:
    enum Type {LANDEXP , LORLAND} type;

    LOrExpAST(Type t) : type(t){};

    std::unique_ptr<BaseAST> landexp;
    std::unique_ptr<BaseAST> lorexp;

    void Dump() const override;
};


class LAndExpAST : public BaseAST {
public:
    enum Type {EQEXP, LANDEQ} type;

    LAndExpAST(Type t) : type(t){};

    std::unique_ptr<BaseAST> eqexp;
    std::unique_ptr<BaseAST> landexp;

    void Dump() const override;
};


class EqExpAST : public BaseAST {
public:
    enum Type {RELEXP, EQREL} type;

    EqExpAST(Type t) : type(t){};

    std::unique_ptr<BaseAST> relexp;
    std::unique_ptr<BaseAST> eqexp;
    std::string op;
    void Dump() const override;
    
};


class RelExpAST : public BaseAST {
public:
    enum Type {ADDEXP, RELADD} type;

    RelExpAST(Type t) : type(t){};

    std::unique_ptr<BaseAST> relexp;
    std::unique_ptr<BaseAST> addexp;
    std::string op;
    void Dump() const override;
    
};



/*

下面是常量与变量相关AST

*/




class DeclAST : public BaseAST {
public:

    std::unique_ptr<BaseAST> constdecl;

};


class ConstDeclAST : public BaseAST {
public:

    std::string btype = "int";
    std::vector<std::unique_ptr<BaseAST>> constdef ;

};



class ConstDefAST : public BaseAST {
public:

    std::string ident;
    std::unique_ptr<BaseAST> constinitval;
    
};