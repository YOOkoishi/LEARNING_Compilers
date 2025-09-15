
#include "include.h"
#include "ast.h"

void CompUnitAST :: Dump() const {
    std::cout << "CompUnitAST { ";
    fun_def -> Dump();
    std::cout << " } ";
}

void FunDefAST :: Dump() const{
    std::cout << "FuncDefAST { ";
    fun_type -> Dump();
    std::cout << ident << " ( ) ";
    block -> Dump();
    std::cout << " } " ;
}

void FunTypeAST :: Dump() const{
    std::cout << "FuncTypeAST { ";
    std::cout << tp ;
    std::cout << " } ";
}


void BlockAST :: Dump() const{
    std::cout << "BlockAST { ";
    stmt -> Dump() ;
    std::cout << " } ";    
}

void StmtAST :: Dump() const{
    std::cout << "StmtAST { ";
    std::cout << retrn;
    number -> Dump();
    std::cout << fenhao;
    std::cout << " } ";
}

void NumberAST :: Dump() const{
    std::cout << int_const;
}