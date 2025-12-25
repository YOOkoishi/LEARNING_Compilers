%code requires {
  #include <memory>
  #include <string>
  #include "include/ast.h"

}

%{

#include <iostream>
#include <memory>
#include <string>
#include "include/ast.h"




// 声明 lexer 函数和错误处理函数
int yylex();
void yyerror(std::unique_ptr<BaseAST> &ast, const char *s);

extern int yylineno;
extern char* yytext;
extern FILE* yyin;

using namespace std;

%}

// 使用 GLR 解析器处理语法冲突
// %glr-parser

%define parse.error verbose

%define parse.trace 

// 定义 parser 函数和错误处理函数的附加参数
// 我们需要返回一个字符串作为 AST, 所以我们把附加参数定义成字符串的智能指针
// 解析完成后, 我们要手动修改这个参数, 把它设置成解析得到的字符串
%parse-param { std::unique_ptr<BaseAST> &ast }

// yylval 的定义, 我们把它定义成了一个联合体 (union)
// 因为 token 的值有的是字符串指针, 有的是整数
// 之前我们在 lexer 中用到的 str_val和 int_val 就是在这里被定义的
// 至于为什么要用字符串指针而不直接用 string 或者 unique_ptr<string>?
// 请自行 STFW 在 union 里写一个带析构函数的类会出现什么情况
%union {
  std::string *str_val;
  int int_val;
  BaseAST *ast_val;
}

// lexer 返回的所有 token 种类的声明
// 注意 IDENT 和 INT_CONST 会返回 token 的值, 分别对应 str_val 和 int_val
%token INT RETURN VOID
%token <str_val> IDENT CONST
%token <int_val> INT_CONST

%token LE GE EQ NE LT GT  '='
%token LOR LAND
%token IF ELSE WHILE BREAK CONTINUE 


%left '='
%left LOR
%left LAND
%left EQ NE
%left LT LE GT GE
%left '+' '-' 
%left '*' '/' '%'

// 非终结符的类型定义
%type <ast_val> FuncDef Block Stmt Number CompUnit 
%type <ast_val> AddExp MulExp LOrExp EqExp RelExp LAndExp Exp UnaryExp PrimaryExp ConstExp ArrayDeclarator ArrayAddress
%type <ast_val> Decl ConstDecl ConstDef ConstInitVal BlockItem BlockItems Lval ConstDefs BType
%type <ast_val> VarDecl VarDef VarDefs InitVal FuncFParams FuncFParam FuncRParams ConstList ExpList
%type <ast_val> InitValList ConstInitValList 
/* %type <int_val>  */
%type <str_val> UnaryOp


%start Root

%%



Root 
  : CompUnit {
    ast = unique_ptr<BaseAST>($1);
  }
  ;


CompUnit
  : FuncDef {
    auto comp_unit = make_unique<CompUnitAST>(CompUnitAST::FUNCDEF);
    comp_unit -> fun_def = unique_ptr<BaseAST>($1);
    $$ = comp_unit.release();
  }
  | Decl {
    auto comp_unit = make_unique<CompUnitAST>(CompUnitAST::DECL);
    comp_unit -> decl = unique_ptr<BaseAST>($1);
    $$ = comp_unit.release();
  }
  | CompUnit FuncDef {
    auto comp_unit = make_unique<CompUnitAST>(CompUnitAST::COMPFUNC);
    comp_unit -> fun_def = unique_ptr<BaseAST>($2);
    comp_unit -> compunit = unique_ptr<BaseAST>($1);
    $$ = comp_unit.release();
  }
  | CompUnit Decl {
    auto comp_unit = make_unique<CompUnitAST>(CompUnitAST::COMPDECL);
    comp_unit -> decl = unique_ptr<BaseAST>($2);
    comp_unit -> compunit = unique_ptr<BaseAST>($1);
    $$ = comp_unit.release();
  }
  ;


FuncDef
  : BType IDENT '(' ')' Block {
    auto ast = make_unique<FunDefAST>(FunDefAST::NOFUNCF);
    auto fun_type = make_unique<FunTypeAST>(FunTypeAST::INT);
    ast -> fun_type = unique_ptr<BaseAST>(fun_type.release());
    ast -> ident = *unique_ptr<string>($2);
    ast -> block = unique_ptr<BaseAST>($5);
    $$ = ast.release();
  } 
  | BType IDENT '(' FuncFParams ')' Block {
    auto ast = make_unique<FunDefAST>(FunDefAST::FUNCF);
    auto fun_type = make_unique<FunTypeAST>(FunTypeAST::INT);
    ast -> fun_type = unique_ptr<BaseAST>(fun_type.release());
    ast -> ident = *unique_ptr<string>($2);
    ast -> funcfparams = unique_ptr<BaseAST>($4);
    ast -> block = unique_ptr<BaseAST>($6);
    $$ = ast.release();
  }
  | VOID IDENT '(' ')' Block {
    auto ast = make_unique<FunDefAST>(FunDefAST::NOFUNCF);
    auto fun_type = make_unique<FunTypeAST>(FunTypeAST::VOID);
    ast -> fun_type = unique_ptr<BaseAST>(fun_type.release());
    ast -> ident = *unique_ptr<string>($2);
    ast -> block = unique_ptr<BaseAST>($5);
    $$ = ast.release();
  } 
  | VOID IDENT '(' FuncFParams ')' Block {
    auto ast = make_unique<FunDefAST>(FunDefAST::FUNCF);
    auto fun_type = make_unique<FunTypeAST>(FunTypeAST::VOID);
    ast -> fun_type = unique_ptr<BaseAST>(fun_type.release());
    ast -> ident = *unique_ptr<string>($2);
    ast -> funcfparams = unique_ptr<BaseAST>($4);
    ast -> block = unique_ptr<BaseAST>($6);
    $$ = ast.release();
  }
  ;



FuncFParams
  : FuncFParam {
    auto ast = make_unique<FuncFParamsAST>();
    ast -> funcflist.push_back(unique_ptr<BaseAST>($1));
    $$ = ast.release();
  }
  | FuncFParams ',' FuncFParam {
    auto ast = dynamic_cast<FuncFParamsAST*>($1);
    ast -> funcflist.push_back(unique_ptr<BaseAST>($3));
    $$ = ast;
  }




FuncFParam
  : BType IDENT {
    auto ast = make_unique<FuncFParamAST>();
    ast -> btype = unique_ptr<BaseAST>($1);
    ast -> ident = *unique_ptr<std::string>($2);
    $$ = ast.release();
  }






Block
  : '{' BlockItems '}' {
    auto block = make_unique<BlockAST>();
    block -> blockitems = unique_ptr<BaseAST>($2);
    $$ = block.release();
  }
  | '{' '}' {
    auto block = make_unique<BlockAST>();
    $$ = block.release();
  }
  ;




BlockItems
  : BlockItem {
    auto blockitems = make_unique<BlockItemsAST>();
    blockitems -> item.push_back(unique_ptr<BaseAST>($1));
    $$ = blockitems.release();    

  }
  | BlockItems BlockItem {
    auto blockitems = dynamic_cast<BlockItemsAST*>($1);
    blockitems -> item.push_back(unique_ptr<BaseAST>($2));
    $$ = blockitems;
  }
  ;




BlockItem 
  : Decl {
    auto blockitem = make_unique<BlockItemAST>();
    blockitem -> type = BlockItemAST::DECL;
    blockitem -> decl = unique_ptr<BaseAST>($1);
    $$ = blockitem.release();

  }
  | Stmt {
    auto blockitem = make_unique<BlockItemAST>();
    blockitem -> type = BlockItemAST::STMT;
    blockitem -> stmt = unique_ptr<BaseAST>($1);
    $$ = blockitem.release();
  }
  ;




Decl
  : ConstDecl {
    auto decl = make_unique<DeclAST>(DeclAST::CONST);
    decl -> constdecl = unique_ptr<BaseAST>($1);
    $$ = decl.release();
  }
  | VarDecl {
    auto decl = make_unique<DeclAST>(DeclAST::VAR);
    decl -> vardecl = unique_ptr<BaseAST>($1);
    $$ = decl.release();
  }
  ;



VarDecl
  : BType VarDefs ';'{
    auto vardecl = make_unique<VarDeclAST>();
    vardecl -> vardefs = unique_ptr<BaseAST>($2);
    $$ = vardecl.release();
  }
  ;



VarDefs
  : VarDef {
    auto vardefs = make_unique<VarDefsAST>();
    vardefs -> vardef.push_back(unique_ptr<BaseAST>($1));
    $$ = vardefs.release();
  }
  | VarDefs ',' VarDef {
    auto vardefs = dynamic_cast<VarDefsAST*>($1);
    vardefs -> vardef.push_back(unique_ptr<BaseAST>($3));
    $$ = vardefs;
     
  }
  ;



VarDef
  : IDENT {
    auto vardef = make_unique<VarDefAST>(VarDefAST::IDENT);
    vardef -> ident = *unique_ptr<string>($1);
    $$ = vardef.release();
  }
  | IDENT '=' InitVal {
    auto vardef = make_unique<VarDefAST>(VarDefAST::IDENTDEF);
    vardef -> ident = *unique_ptr<string>($1);
    vardef -> initval = unique_ptr<BaseAST>($3);
    $$ = vardef.release();
  }
  | IDENT ArrayDeclarator {
    auto vardef = make_unique<VarDefAST>(VarDefAST::ARRAY);
    vardef -> ident = *unique_ptr<string>($1);
    vardef -> dimlist = unique_ptr<BaseAST>($2);
    $$ = vardef.release();
  } 
  | IDENT ArrayDeclarator '=' InitVal {
    auto vardef = make_unique<VarDefAST>(VarDefAST::ARRAYDEF);
    vardef -> ident = *unique_ptr<string>($1);
    vardef -> dimlist = unique_ptr<BaseAST>($2);
    vardef -> initval = unique_ptr<BaseAST>($4);
    $$ = vardef.release();
  }
  ;



InitVal
  : Exp {
    auto initval = make_unique<InitValAST>(InitValAST::EXP);
    initval -> exp = unique_ptr<BaseAST>($1);
    $$ = initval.release();
  }
  | '{' InitValList '}'{
    auto initval = make_unique<InitValAST>(InitValAST::ARRAY);
    initval -> initlist = unique_ptr<BaseAST>($2);
    $$ = initval.release();
  }
  | '{' '}' {
    auto initval = make_unique<InitValAST>(InitValAST::ZEROINIT);
    $$ = initval.release();
  }
  ;

InitValList
  : InitVal {
    auto list = make_unique<InitValListAST>();
    list -> initlist.push_back(unique_ptr<BaseAST>($1));
    $$ = list.release();
  }
  | InitValList ',' InitVal {
    auto list = dynamic_cast<InitValListAST*>($1);
    list -> initlist.push_back(unique_ptr<BaseAST>($3));
    $$ = list;
  }
  ;


ConstInitValList
  : ConstInitVal {
    auto list = make_unique<ConstInitValListAST>();
    list -> constinitlist.push_back(unique_ptr<BaseAST>($1));
    $$ = list.release();
  }
  | ConstInitValList ',' ConstInitVal {
    auto list = dynamic_cast<ConstInitValListAST*>($1);
    list -> constinitlist.push_back(unique_ptr<BaseAST>($3));
    $$ = list;
  }
  ;


ConstDecl 
  : CONST BType ConstDefs ';' {
    auto constdecl = make_unique<ConstDeclAST>();
    constdecl -> btype = unique_ptr<BaseAST>($2);
    constdecl -> constdefs = unique_ptr<BaseAST>($3);
    $$ = constdecl.release();
  }
  ;


BType
  : INT {
    auto btype = make_unique<BTypeAST>();
    btype -> val = "int";
    $$ = btype.release();
  }
  ;



ConstDefs 
  : ConstDef {
    auto constdefs = make_unique<ConstDefsAST>();
    constdefs -> constdef.push_back(unique_ptr<BaseAST>($1));
    $$ = constdefs.release();

  }
  | ConstDefs ',' ConstDef {
    auto constdefs = dynamic_cast<ConstDefsAST*>($1);
    constdefs -> constdef.push_back(unique_ptr<BaseAST>($3));
    $$ = constdefs;

  }
  ;




ConstDef
  : IDENT '=' ConstInitVal {
    auto constdef = make_unique<ConstDefAST>(ConstDefAST::CONST);
    constdef -> ident = *unique_ptr<string>($1);
    constdef -> constinitval = unique_ptr<BaseAST>($3); 
    $$ = constdef.release();    

  }
  | IDENT ArrayDeclarator '=' ConstInitVal {
    auto constdef = make_unique<ConstDefAST>(ConstDefAST::ARRAY);
    constdef -> ident = *unique_ptr<string>($1);
    constdef -> dimlist = unique_ptr<BaseAST>($2);
    constdef -> constinitval = unique_ptr<BaseAST>($4);
    $$ = constdef.release();    
  }
  ;



ConstInitVal
  : ConstExp {
    auto constinitval = make_unique<ConstInitValAST>(ConstInitValAST::CONSTEXP);
    constinitval -> constexp = unique_ptr<BaseAST>($1);
    $$ = constinitval.release();
  }
  | '{' ConstInitValList '}' {
    auto constinitval = make_unique<ConstInitValAST>(ConstInitValAST::CONSTLIST);
    constinitval -> constinitlist = unique_ptr<BaseAST>($2);
    $$ = constinitval.release();
  }
  | '{' '}' {
    auto constinitval = make_unique<ConstInitValAST>(ConstInitValAST::ZEROINIT);
    $$ = constinitval.release();
  }
  ;





ConstExp
  : Exp {
    auto constexp = make_unique<ConstExpAST>();
    constexp -> exp = unique_ptr<BaseAST>($1);
    $$ = constexp.release();
  }
  ;




Stmt
  : RETURN Exp ';' {
    auto stmt = make_unique<StmtAST>(StmtAST::RETURNEXP);
    stmt -> exp = unique_ptr<BaseAST>($2);
    $$ = stmt.release();
  }
  | RETURN ';' {
    auto stmt = make_unique<StmtAST>(StmtAST::RETURNNULL);
    $$ = stmt.release();
  }
  | Exp ';'{
    auto stmt = make_unique<StmtAST>(StmtAST::EXP);
    stmt -> exp = unique_ptr<BaseAST>($1);
    $$ = stmt.release();
  }
  | ';' {
    auto stmt = make_unique<StmtAST>(StmtAST::NUL);
    $$ = stmt.release();
  }
  | Block {
    auto stmt = make_unique<StmtAST>(StmtAST::BLOCK);
    stmt -> block = unique_ptr<BaseAST>($1);
    $$ = stmt.release();
  }
  | Lval '=' Exp ';'{
    auto stmt = make_unique<StmtAST>(StmtAST::LVALEXP);
    stmt -> lval = unique_ptr<BaseAST>($1);
    stmt -> exp = unique_ptr<BaseAST>($3);
    $$ = stmt.release();
  }
  | IF '(' Exp ')' Stmt {
    auto stmt = make_unique<StmtAST>(StmtAST::IF);
    stmt -> exp = unique_ptr<BaseAST>($3);
    stmt -> if_stmt = unique_ptr<BaseAST>($5);
    $$ = stmt.release();
  }
  | IF '(' Exp ')' Stmt ELSE Stmt{
    auto stmt = make_unique<StmtAST>(StmtAST::IFELSE);
    stmt -> exp = unique_ptr<BaseAST>($3);
    stmt -> if_stmt = unique_ptr<BaseAST>($5);
    stmt -> else_stmt = unique_ptr<BaseAST>($7);
    $$ = stmt.release();
  } 
  | WHILE '(' Exp ')' Stmt {
    auto stmt = make_unique<StmtAST>(StmtAST::WHILE);
    stmt -> exp = unique_ptr<BaseAST>($3);
    stmt -> while_stmt = unique_ptr<BaseAST>($5);
    $$ = stmt.release();
  }
  | BREAK ';' {
    auto stmt = make_unique<StmtAST>(StmtAST::BREAK);
    $$ = stmt.release();
  }
  | CONTINUE ';' {
    auto stmt = make_unique<StmtAST>(StmtAST::CONTINUE);
    $$ = stmt.release();
  }
  ;


ConstList
  : ConstExp {
    auto constexplist = make_unique<ConstExpListAST>();
    constexplist -> constexplist.push_back(unique_ptr<BaseAST>($1));
    $$ = constexplist.release();
  }
  | ConstList ',' ConstExp {
    auto constexplist = dynamic_cast<ConstExpListAST*>($1);
    constexplist -> constexplist.push_back(unique_ptr<BaseAST>($3));
    $$ = constexplist;
  }
  ;


ArrayDeclarator
  : '[' ConstExp ']' {
    auto constexplist = make_unique<ConstExpListAST>();
    constexplist -> constexplist.push_back(unique_ptr<BaseAST>($2));
    $$ = constexplist.release();
  }
  | ArrayDeclarator '[' ConstExp ']' {
    auto constexplist = dynamic_cast<ConstExpListAST*>($1);
    constexplist -> constexplist.push_back(unique_ptr<BaseAST>($3));
    $$ = constexplist;
  }




ExpList
  : Exp {
    auto explist = make_unique<ExpListAST>();
    explist -> explist.push_back(unique_ptr<BaseAST>($1));
    $$ = explist.release();
  }
  | ExpList ',' Exp {
    auto explist = dynamic_cast<ExpListAST*>($1);
    explist -> explist.push_back(unique_ptr<BaseAST>($3));
    $$ = explist;
  }


ArrayAddress
  : '[' Exp ']'{
    auto explist = make_unique<ExpListAST>();
    explist -> explist.push_back(unique_ptr<BaseAST>($2));
    $$ = explist.release();
  }
  | ArrayAddress '[' Exp ']'{
    auto explist = dynamic_cast<ExpListAST*>($1);
    explist -> explist.push_back(unique_ptr<BaseAST>($3));
    $$ = explist;
  }


Exp 
  : LOrExp {
    auto exp = make_unique<ExpAST>();
    exp -> lorexp = unique_ptr<BaseAST>($1);
    $$ = exp.release();
  }
  ;




LOrExp
  : LAndExp {
    auto lor = make_unique<LOrExpAST>(LOrExpAST::LANDEXP);
    lor -> landexp = unique_ptr<BaseAST>($1);
    $$ = lor.release();
  }
  | LOrExp LOR LAndExp {
    auto lor = make_unique<LOrExpAST>(LOrExpAST::LORLAND);
    lor -> lorexp = unique_ptr<BaseAST>($1);
    lor -> landexp = unique_ptr<BaseAST>($3);
    $$ = lor.release();
  }
  ;



  
LAndExp
  : EqExp {
    auto land = make_unique<LAndExpAST>(LAndExpAST::EQEXP);
    land -> eqexp = unique_ptr<BaseAST>($1);
    $$ = land.release();
  }
  | LAndExp LAND EqExp {
    auto land = make_unique<LAndExpAST>(LAndExpAST::LANDEQ);
    land -> landexp = unique_ptr<BaseAST>($1);
    land -> eqexp = unique_ptr<BaseAST>($3);
    $$ = land.release();
  }
  ;




EqExp
  : RelExp {
    auto eq = make_unique<EqExpAST>(EqExpAST::RELEXP);
    eq -> relexp = unique_ptr<BaseAST>($1);
    $$ = eq.release();
  }
  | EqExp EQ RelExp{
    auto eq = make_unique<EqExpAST>(EqExpAST::EQREL);
    eq -> eqexp = unique_ptr<BaseAST>($1);
    eq -> op = "==";
    eq -> relexp = unique_ptr<BaseAST>($3);
    $$ = eq.release();
  }
  | EqExp NE RelExp{
    auto eq = make_unique<EqExpAST>(EqExpAST::EQREL);
    eq -> eqexp = unique_ptr<BaseAST>($1);
    eq -> op = "!=";
    eq -> relexp = unique_ptr<BaseAST>($3);
    $$ = eq.release();
  }
  ;




RelExp
  : AddExp {
    auto rel = make_unique<RelExpAST>(RelExpAST::ADDEXP);
    rel -> addexp = unique_ptr<BaseAST>($1);
    $$ = rel.release();
  }
  | RelExp LT AddExp {
    auto rel = make_unique<RelExpAST>(RelExpAST::RELADD);
    rel -> relexp = unique_ptr<BaseAST>($1);
    rel -> op = "<";
    rel -> addexp = unique_ptr<BaseAST>($3);
    $$ = rel.release();
  }
  | RelExp GT AddExp {
    auto rel = make_unique<RelExpAST>(RelExpAST::RELADD);
    rel -> relexp = unique_ptr<BaseAST>($1);
    rel -> op = ">";
    rel -> addexp = unique_ptr<BaseAST>($3);
    $$ = rel.release();
  }
  | RelExp LE AddExp {
    auto rel = make_unique<RelExpAST>(RelExpAST::RELADD);
    rel -> relexp = unique_ptr<BaseAST>($1);
    rel -> op = "<=";
    rel -> addexp = unique_ptr<BaseAST>($3);
    $$ = rel.release();
  }
  | RelExp GE AddExp {
    auto rel = make_unique<RelExpAST>(RelExpAST::RELADD);
    rel -> relexp = unique_ptr<BaseAST>($1);
    rel -> op = ">=";
    rel -> addexp = unique_ptr<BaseAST>($3);
    $$ = rel.release();
  }
  ;
  



AddExp
  : MulExp {
    auto addexp = make_unique<AddExpAST>(AddExpAST::MULONLY);
    addexp -> mulexp = unique_ptr<BaseAST>($1);
    $$ = addexp.release();
  }
  | AddExp '+' MulExp {
    auto addexp = make_unique<AddExpAST>(AddExpAST::ADDOPMUL);
    addexp -> addexp = unique_ptr<BaseAST>($1);
    addexp -> mulexp = unique_ptr<BaseAST>($3);
    addexp -> op = "+";
    $$ = addexp.release();
  }
  | AddExp '-' MulExp {
    auto addexp = make_unique<AddExpAST>(AddExpAST::ADDOPMUL);
    addexp -> addexp = unique_ptr<BaseAST>($1);
    addexp -> mulexp = unique_ptr<BaseAST>($3);
    addexp -> op = "-";
    $$ = addexp.release();
  }
  ;




MulExp
  : UnaryExp {
    auto mulexp = make_unique<MulExpAST>(MulExpAST::UNARYEXP);
    mulexp -> unrayexp = unique_ptr<BaseAST>($1);
    $$ = mulexp.release();
  }
  | MulExp '*' UnaryExp {
    auto mulexp = make_unique<MulExpAST>(MulExpAST::MULOPUNRAY);
    mulexp -> mulexp = unique_ptr<BaseAST>($1);
    mulexp -> unrayexp = unique_ptr<BaseAST>($3);
    mulexp -> op = "*";
    $$ = mulexp.release();
  } 
  | MulExp '/' UnaryExp {
    auto mulexp = make_unique<MulExpAST>(MulExpAST::MULOPUNRAY);
    mulexp -> mulexp = unique_ptr<BaseAST>($1);
    mulexp -> unrayexp = unique_ptr<BaseAST>($3);
    mulexp -> op = "/";
    $$ = mulexp.release();
  }
  | MulExp '%' UnaryExp {
    auto mulexp = make_unique<MulExpAST>(MulExpAST::MULOPUNRAY);
    mulexp -> mulexp = unique_ptr<BaseAST>($1);
    mulexp -> unrayexp = unique_ptr<BaseAST>($3);
    mulexp -> op = "%";
    $$ = mulexp.release();
  }
  ;




PrimaryExp
  : '(' Exp ')'{
    auto primaryexp = make_unique<PrimaryExpAST>(PrimaryExpAST::EXP);
    primaryexp -> exp = unique_ptr<BaseAST>($2);
    $$ = primaryexp.release();
  } 
  | Number {
    auto primaryexp = make_unique<PrimaryExpAST>(PrimaryExpAST::NUMBER);
    primaryexp -> number = unique_ptr<BaseAST>($1);
    $$ = primaryexp.release();
  }
  | Lval {
    auto primaryexp = make_unique<PrimaryExpAST>(PrimaryExpAST::LVAL);
    primaryexp -> lval = unique_ptr<BaseAST>($1);
    $$ = primaryexp.release();
  }
  ;




UnaryExp
  : PrimaryExp {
    auto unaryexp = make_unique<UnaryExpAST>(UnaryExpAST::PRIMARYEXP);
    unaryexp -> primaryexp = unique_ptr<BaseAST>($1);
    $$ = unaryexp.release();
  }
  | UnaryOp UnaryExp{
    auto unaryexp = make_unique<UnaryExpAST>(UnaryExpAST::UNARYEXP);
    unaryexp -> unaryop = *unique_ptr<std::string>($1);
    unaryexp -> unaryexp = unique_ptr<BaseAST>($2);
    $$ = unaryexp.release();
  }
  | IDENT '(' ')' {
    auto unaryexp = make_unique<UnaryExpAST>(UnaryExpAST::FUNCVOID);
    unaryexp -> ident = *unique_ptr<std::string>($1);
    $$ = unaryexp.release();
  }
  | IDENT '(' FuncRParams ')' {
    auto unaryexp = make_unique<UnaryExpAST>(UnaryExpAST::FUNCRPARAMS);
    unaryexp -> ident = *unique_ptr<std::string>($1);
    unaryexp -> funcrparams = unique_ptr<BaseAST>($3);
    $$ = unaryexp.release();
  }
  ;



FuncRParams 
  : Exp {
    auto ast = make_unique<FuncRParamsAST>();
    ast -> explist.push_back(unique_ptr<BaseAST>($1));
    $$ = ast.release();
  }
  | FuncRParams ',' Exp {
    auto ast = dynamic_cast<FuncRParamsAST*>($1);
    ast -> explist.push_back(unique_ptr<BaseAST>($3));
    $$ = ast;
  }




UnaryOp
  : '+'{
    $$ = new std::string("+");
  }
  | '-'{
    $$ = new std::string("-");
  }
  | '!'{
    $$ = new std::string("!");
  }
  ;




Number
  : INT_CONST {
    auto int_con = make_unique<NumberAST>();
    int_con -> int_const = ($1);
    $$ = int_con.release();
  }
  ;


Lval
  : IDENT {
    auto lval = make_unique<LValAST>(LValAST::VAR);
    lval -> ident = *unique_ptr<string>($1);
    $$ = lval.release();

  }
  | IDENT ArrayAddress {
    auto lval = make_unique<LValAST>(LValAST::ARRAY);
    lval -> ident = *unique_ptr<string>($1);
    lval -> address = unique_ptr<BaseAST>($2);
    $$ = lval.release();
  }
  ;






%%

// 定义错误处理函数, 其中第二个参数是错误信息
// parser 如果发生错误 (例如输入的程序出现了语法错误), 就会调用这个函数
void yyerror(unique_ptr<BaseAST> &ast, const char *s) {
    // ✅ 输出详细错误信息
    std::cerr << "Error at line " << yylineno << ": " << s << std::endl;
    std::cerr << "Near token: '" << yytext << "'" << std::endl;
}