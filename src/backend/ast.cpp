
#include "../include/include.h"
#include "../include/ast.h"

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
 
}

void StmtAST :: Dump() const{
    std::cout << "StmtAST { ";
    std::cout << retrn;
    exp -> Dump();
    std::cout << fenhao;
    std::cout << " } ";
}

void NumberAST :: Dump() const{
    std::cout << int_const;
}

void ExpAST :: Dump() const{
    std::cout<<"ExpAST { ";
    lorexp ->Dump();
    std::cout<<" }";
}

void PrimaryExpAST::Dump() const{

}

void UnaryExpAST :: Dump() const{

}

void UnaryOpAST :: Dump() const{

}

void AddExpAST :: Dump() const{

}

void MulExpAST :: Dump() const{
    
}

void LOrExpAST :: Dump() const{
    
}

void EqExpAST :: Dump() const{
    
}

void RelExpAST :: Dump() const{
    
}

void LAndExpAST:: Dump() const{
    
}