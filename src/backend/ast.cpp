
#include "../include/include.h"
#include "../include/ast.h"

void CompUnitAST :: Dump() const {
    std::cout << "CompUnitAST { ";
    if (type == COMPFUNC) {
        compunit->Dump();
    }
    fun_def -> Dump();
    std::cout << " } ";
}

void FunDefAST :: Dump() const{
    std::cout << "FuncDefAST { ";
    fun_type -> Dump();
    std::cout << " " << ident << " ( ";
    if (type == FUNCF && funcfparams) {
        funcfparams->Dump();
    }
    std::cout << " ) ";
    block -> Dump();
    std::cout << " } " ;
}

void FunTypeAST :: Dump() const{
    std::cout << "FuncTypeAST { ";
    if (type == INT) std::cout << "int";
    else if (type == VOID) std::cout << "void";
    std::cout << " } ";
}

void FuncFParamsAST::Dump() const {
    std::cout << "FuncFParamsAST { ";
    for (const auto& param : funcflist) {
        param->Dump();
        std::cout << ", ";
    }
    std::cout << " }";
}

void FuncFParamAST::Dump() const {
    std::cout << "FuncFParamAST { ";
    btype->Dump();
    std::cout << " " << ident;
    std::cout << " }";
}

void FuncRParamsAST::Dump() const {
    std::cout << "FuncRParamsAST { ";
    for (const auto& exp : explist) {
        exp->Dump();
        std::cout << ", ";
    }
    std::cout << " }";
}


void BlockAST :: Dump() const{
 
}

void StmtAST :: Dump() const{
    std::cout << "StmtAST { ";
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

void BlockItemsAST :: Dump() const{

}

void BlockItemAST :: Dump() const{

}

void DeclAST :: Dump() const{

}

void ConstDeclAST :: Dump() const {

}

void BTypeAST :: Dump() const {

}

void ConstDefsAST :: Dump() const {

}

void ConstDefAST :: Dump() const {

}

void ConstInitValAST :: Dump() const {

}

void ConstExpAST :: Dump() const{

}

void LValAST :: Dump() const{
    
}


void VarDeclAST :: Dump() const{

}


void VarDefsAST :: Dump() const{

}


void VarDefAST :: Dump() const{

}


void InitValAST :: Dump() const{

}



