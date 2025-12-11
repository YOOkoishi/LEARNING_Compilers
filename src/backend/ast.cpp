
#include "../include/include.h"
#include "../include/ast.h"

void CompUnitAST :: Dump() const {
    std::cout << "CompUnitAST { ";
    if (type == COMPFUNC) {
        compunit->Dump();
        fun_def -> Dump();
    } else if (type == COMPDECL) {
        compunit->Dump();
        decl -> Dump();
    } else if (type == FUNCDEF) {
        fun_def -> Dump();
    } else if (type == DECL) {
        decl -> Dump();
    }
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
    std::cout << "BlockAST { ";
    if (blockitems) blockitems->Dump();
    std::cout << " }";
}

void StmtAST :: Dump() const{
    std::cout << "StmtAST { ";
    switch (type) {
        case LVALEXP:
            lval->Dump();
            std::cout << " = ";
            exp->Dump();
            break;
        case EXP:
            if (exp) exp->Dump();
            break;
        case BLOCK:
            block->Dump();
            break;
        case IF:
            std::cout << "if ( ";
            exp->Dump();
            std::cout << " ) ";
            if_stmt->Dump();
            break;
        case IFELSE:
            std::cout << "if ( ";
            exp->Dump();
            std::cout << " ) ";
            if_stmt->Dump();
            std::cout << " else ";
            else_stmt->Dump();
            break;
        case WHILE:
            std::cout << "while ( ";
            exp->Dump();
            std::cout << " ) ";
            while_stmt->Dump(); 
            break;
        case BREAK:
            std::cout << "break";
            break;
        case CONTINUE:
            std::cout << "continue";
            break;
        case RETURNEXP:
            std::cout << "return ";
            exp->Dump();
            break;
        case RETURNNULL:
            std::cout << "return";
            break;
        default:
            break;
    }
    std::cout << " } ";
}

void NumberAST :: Dump() const{
    std::cout << "NumberAST { " << int_const << " }";
}

void ExpAST :: Dump() const{
    std::cout<<"ExpAST { ";
    if (lorexp) lorexp ->Dump();
    std::cout<<" }";
}

void PrimaryExpAST::Dump() const{
    std::cout << "PrimaryExpAST { ";
    if (type == EXP) {
        std::cout << "( ";
        exp->Dump();
        std::cout << " )";
    } else if (type == LVAL) {
        lval->Dump();
    } else if (type == NUMBER) {
        number->Dump();
    }
    std::cout << " }";
}

void UnaryExpAST :: Dump() const{
    std::cout << "UnaryExpAST { ";
    if (type == PRIMARYEXP) {
        primaryexp->Dump();
    } else if (type == UNARYEXP) {
        std::cout << unaryop << " ";
        unaryexp->Dump();
    } else if (type == FUNCVOID) {
        std::cout << ident << "()";
    } else if (type == FUNCRPARAMS) {
        std::cout << ident << "( ";
        if (funcrparams) funcrparams->Dump();
        std::cout << " )";
    }
    std::cout << " }";
}

void UnaryOpAST :: Dump() const{
    std::cout << op;
}

void AddExpAST :: Dump() const{
    std::cout << "AddExpAST { ";
    if (type == MULONLY) {
        mulexp->Dump();
    } else {
        addexp->Dump();
        std::cout << " " << op << " ";
        mulexp->Dump();
    }
    std::cout << " }";
}

void MulExpAST :: Dump() const{
    std::cout << "MulExpAST { ";
    if (type == UNARYEXP) {
        unrayexp->Dump();
    } else {
        mulexp->Dump();
        std::cout << " " << op << " ";
        unrayexp->Dump();
    }
    std::cout << " }";
}

void LOrExpAST :: Dump() const{
    std::cout << "LOrExpAST { ";
    if (type == LANDEXP) {
        landexp->Dump();
    } else {
        lorexp->Dump();
        std::cout << " || ";
        landexp->Dump();
    }
    std::cout << " }";
}

void EqExpAST :: Dump() const{
    std::cout << "EqExpAST { ";
    if (type == RELEXP) {
        relexp->Dump();
    } else {
        eqexp->Dump();
        std::cout << " " << op << " ";
        relexp->Dump();
    }
    std::cout << " }";
}

void RelExpAST :: Dump() const{
    std::cout << "RelExpAST { ";
    if (type == ADDEXP) {
        addexp->Dump();
    } else {
        relexp->Dump();
        std::cout << " " << op << " ";
        addexp->Dump();
    }
    std::cout << " }";
}

void LAndExpAST:: Dump() const{
    std::cout << "LAndExpAST { ";
    if (type == EQEXP) {
        eqexp->Dump();
    } else {
        landexp->Dump();
        std::cout << " && ";
        eqexp->Dump();
    }
    std::cout << " }";
}

void BlockItemsAST :: Dump() const{
    std::cout << "BlockItemsAST { ";
    for (const auto& it : item) {
        it->Dump();
        std::cout << "; ";
    }
    std::cout << " }";
}

void BlockItemAST :: Dump() const{
    std::cout << "BlockItemAST { ";
    if (type == DECL) decl->Dump();
    else stmt->Dump();
    std::cout << " }";
}

void DeclAST :: Dump() const{
    std::cout << "DeclAST { ";
    if (type == CONST) constdecl->Dump();
    else vardecl->Dump();
    std::cout << " }";
}

void ConstDeclAST :: Dump() const {
    std::cout << "ConstDeclAST { const ";
    btype->Dump();
    std::cout << " ";
    constdefs->Dump();
    std::cout << " ; }";
}

void BTypeAST :: Dump() const {
    std::cout << val;
}

void ConstDefsAST :: Dump() const {
    std::cout << "ConstDefsAST { ";
    for (size_t i = 0; i < constdef.size(); ++i) {
        constdef[i]->Dump();
        if (i < constdef.size() - 1) std::cout << ", ";
    }
    std::cout << " }";
}

void ConstDefAST :: Dump() const {
    std::cout << "ConstDefAST { " << ident << " = ";
    constinitval->Dump();
    std::cout << " }";
}

void ConstInitValAST :: Dump() const {
    std::cout << "ConstInitValAST { ";
    constexp->Dump();
    std::cout << " }";
}

void ConstExpAST :: Dump() const{
    std::cout << "ConstExpAST { ";
    exp->Dump();
    std::cout << " }";
}

void LValAST :: Dump() const{
    std::cout << "LValAST { " << ident;
    // TODO: Add array indices dump if implemented
    std::cout << " }";
}


void VarDeclAST :: Dump() const{
    std::cout << "VarDeclAST { ";
    btype->Dump();
    std::cout << " ";
    vardefs->Dump();
    std::cout << " ; }";
}


void VarDefsAST :: Dump() const{
    std::cout << "VarDefsAST { ";
    for (size_t i = 0; i < vardef.size(); ++i) {
        vardef[i]->Dump();
        if (i < vardef.size() - 1) std::cout << ", ";
    }
    std::cout << " }";
}


void VarDefAST :: Dump() const{
    std::cout << "VarDefAST { " << ident;
    if (type == IDENTDEF) {
        std::cout << " = ";
        initval->Dump();
    }
    std::cout << " }";
}


void InitValAST :: Dump() const{
    std::cout << "InitValAST { ";
    exp->Dump();
    std::cout << " }";
}



