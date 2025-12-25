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
    enum Type {
        FUNCDEF,
        DECL,
        COMPFUNC,
        COMPDECL
    } type;

    CompUnitAST(Type t) : type(t){};
    CompUnitAST() = default;
    std::unique_ptr<BaseAST> compunit;
    std::unique_ptr<BaseAST> fun_def;
    std::unique_ptr<BaseAST> decl;
    void Dump() const override;
};


//8.1


class FunDefAST : public BaseAST {
public:
    enum Type {
        NOFUNCF,
        FUNCF
    } type;

    FunDefAST()=default;
    FunDefAST(Type t) :type(t){};
       
    std::unique_ptr<BaseAST> fun_type;
    std::string ident;
    std::unique_ptr<BaseAST> funcfparams;
    std::unique_ptr<BaseAST> block;
    void Dump() const override;
};


class FunTypeAST : public BaseAST {
public:
    enum Type{
        INT,
        VOID
    } type;
    FunTypeAST(Type t): type(t){};
    FunTypeAST() = default;
    void Dump() const override;
};




class FuncFParamsAST : public BaseAST {
public:
    std::vector<std::unique_ptr<BaseAST>> funcflist;    
    void Dump() const override;
};




class FuncFParamAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> btype;
    std::string ident;
    void Dump() const override;
};



class FuncRParamsAST : public BaseAST{
public:
    std::vector<std::unique_ptr<BaseAST>> explist;

    void Dump() const override;
};


//new

class BlockAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> blockitems; 
    void Dump() const override;
};




class BlockItemsAST : public BaseAST{
public:
    std::vector<std::unique_ptr<BaseAST>> item;
    void Dump() const override;
};




class BlockItemAST : public BaseAST {
public:
    enum Type {
        DECL,
        STMT
    } type;
    
    BlockItemAST(Type t) : type(t){};
    BlockItemAST(){};

    std::unique_ptr<BaseAST> decl;
    std::unique_ptr<BaseAST> stmt;
    void Dump() const override;
};


//4.1


class StmtAST : public BaseAST {
public:
    enum Type {
        RETURNEXP,
        LVALEXP,
        RETURNNULL,
        EXP,
        NUL,
        BLOCK,
        IF,
        IFELSE,
        WHILE,
        BREAK,
        CONTINUE
    }type;

    StmtAST(Type t) : type(t){};
    StmtAST(){};
    std::unique_ptr<BaseAST> if_stmt;    
    std::unique_ptr<BaseAST> else_stmt;
    std::unique_ptr<BaseAST> while_stmt;
    std::unique_ptr<BaseAST> lval;
    std::unique_ptr<BaseAST> exp;
    std::unique_ptr<BaseAST> block;
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
    enum Type { EXP, NUMBER , LVAL} type;
    std::unique_ptr<BaseAST> exp;
    std::unique_ptr<BaseAST> number;
    std::unique_ptr<BaseAST> lval;

    PrimaryExpAST(Type t) : type(t){};

    void Dump() const override;
};


class UnaryExpAST : public BaseAST {
public:
    enum Type {PRIMARYEXP , 
                UNARYEXP , 
                FUNCVOID,
                FUNCRPARAMS} type;

    UnaryExpAST(Type t) : type(t){};
    UnaryExpAST() = default;

    std::string ident;
    std::unique_ptr<BaseAST> funcrparams;
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
    enum Type {
        CONST,
        VAR
    } type;


    DeclAST(Type t) : type(t) {};
    DeclAST(){};
    std::unique_ptr<BaseAST> vardecl;
    std::unique_ptr<BaseAST> constdecl;
    void Dump() const override;
};


class ConstDeclAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> btype;
    std::unique_ptr<BaseAST> constdefs;
    void Dump() const override;
};



class BTypeAST : public BaseAST {
public:
    std::string val ;
    void Dump() const override;
};


class ConstDefsAST : public BaseAST {
public: 
    std::vector<std::unique_ptr<BaseAST>> constdef ;
    void Dump() const override;

};


class ConstDefAST : public BaseAST {
public:
    enum Type {
        CONST,
        ARRAY
    } type;


    ConstDefAST() = default;
    ConstDefAST(Type t): type(t){};
    
    std::string ident;
    std::unique_ptr<BaseAST> dimlist;
    std::unique_ptr<BaseAST> constinitval;
    void Dump() const override;
    
};


class ConstInitValAST : public BaseAST {
public:
    enum Type {
        CONSTEXP,
        CONSTLIST,
        ZEROINIT
    }type;

    ConstInitValAST() =default;
    ConstInitValAST(Type t) : type(t){};
    std::unique_ptr<BaseAST> constinitlist;  // 改为 ConstInitValListAST
    std::unique_ptr<BaseAST> constexp;
    void Dump() const override;
};



class ConstExpListAST : public BaseAST {
public:
    std::vector<std::unique_ptr<BaseAST>> constexplist;
    void Dump() const override;
};


class ExpListAST : public BaseAST {
public:
    std::vector<std::unique_ptr<BaseAST>> explist;
    void Dump() const override;
};



class InitValListAST : public BaseAST {
public:
    std::vector<std::unique_ptr<BaseAST>> initlist;
    void Dump() const override;
};



class ConstInitValListAST : public BaseAST {
public:
    std::vector<std::unique_ptr<BaseAST>> constinitlist;
    void Dump() const override;
};



class ConstExpAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> exp;
    void Dump() const override;
};


class LValAST : public BaseAST {
public:
    enum Type{
        VAR,
        ARRAY
    } type;

    LValAST() = default;
    LValAST(Type t) : type(t){};
     
    std::string ident;
    std::unique_ptr<BaseAST> address;
    void Dump() const override;
};


// 下面是变量相关的内容

class VarDeclAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> btype;
    std::unique_ptr<BaseAST> vardefs;

    void Dump() const override;    
};




class VarDefsAST : public BaseAST {
public:
    std::vector<std::unique_ptr<BaseAST>> vardef;
    void Dump() const override;
};


class VarDefAST : public BaseAST {
public:
    enum Type {
        IDENT,
        IDENTDEF,
        ARRAY,
        ARRAYDEF
    } type;

    VarDefAST(Type t) : type( t) {};
    VarDefAST() {};

    std::string ident;
    std::unique_ptr<BaseAST> initval;
    std::unique_ptr<BaseAST> dimlist;

    void Dump() const override;
};




class InitValAST : public BaseAST {
public:
    enum Type {
        EXP,
        ARRAY,
        ZEROINIT
    } type;


    InitValAST() = default;
    InitValAST(Type t): type(t) {};


    std::unique_ptr<BaseAST> exp;
    std::unique_ptr<BaseAST> initlist;  // 改为 InitValListAST

    void Dump() const override;
};


