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
%type <ast_val> AddExp MulExp LOrExp EqExp RelExp LAndExp Exp UnaryExp PrimaryExp ConstExp
%type <ast_val> Decl ConstDecl ConstDef ConstInitVal BlockItem BlockItems Lval ConstDefs BType
%type <ast_val> VarDecl VarDef VarDefs InitVal FuncFParams FuncFParam FuncRParams ConstList ExpList
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
    auto comp_unit = new CompUnitAST(CompUnitAST::FUNCDEF);
    comp_unit -> fun_def = unique_ptr<BaseAST>($1);
    $$ = comp_unit;
  }
  | Decl {
    auto comp_unit = new CompUnitAST(CompUnitAST::DECL);
    comp_unit -> decl = unique_ptr<BaseAST>($1);
    $$ = comp_unit;
  }
  | CompUnit FuncDef {
    auto comp_unit = new CompUnitAST(CompUnitAST::COMPFUNC);
    comp_unit -> fun_def = unique_ptr<BaseAST>($2);
    comp_unit -> compunit = unique_ptr<BaseAST>($1);
    $$ = comp_unit;
  }
  | CompUnit Decl {
    auto comp_unit = new CompUnitAST(CompUnitAST::COMPDECL);
    comp_unit -> decl = unique_ptr<BaseAST>($2);
    comp_unit -> compunit = unique_ptr<BaseAST>($1);
    $$ = comp_unit;
  }
  ;


FuncDef
  : BType IDENT '(' ')' Block {
    auto ast = new FunDefAST(FunDefAST::NOFUNCF);
    auto fun_type = new FunTypeAST(FunTypeAST::INT);
    ast -> fun_type = unique_ptr<BaseAST>(fun_type);
    ast -> ident = *unique_ptr<string>($2);
    ast -> block = unique_ptr<BaseAST>($5);
    $$ = ast;
  } 
  | BType IDENT '(' FuncFParams ')' Block {
    auto ast = new FunDefAST(FunDefAST::FUNCF);
    auto fun_type = new FunTypeAST(FunTypeAST::INT);
    ast -> fun_type = unique_ptr<BaseAST>(fun_type);
    ast -> ident = *unique_ptr<string>($2);
    ast -> funcfparams = unique_ptr<BaseAST>($4);
    ast -> block = unique_ptr<BaseAST>($6);
    $$ = ast;
  }
  | VOID IDENT '(' ')' Block {
    auto ast = new FunDefAST(FunDefAST::NOFUNCF);
    auto fun_type = new FunTypeAST(FunTypeAST::VOID);
    ast -> fun_type = unique_ptr<BaseAST>(fun_type);
    ast -> ident = *unique_ptr<string>($2);
    ast -> block = unique_ptr<BaseAST>($5);
    $$ = ast;
  } 
  | VOID IDENT '(' FuncFParams ')' Block {
    auto ast = new FunDefAST(FunDefAST::FUNCF);
    auto fun_type = new FunTypeAST(FunTypeAST::VOID);
    ast -> fun_type = unique_ptr<BaseAST>(fun_type);
    ast -> ident = *unique_ptr<string>($2);
    ast -> funcfparams = unique_ptr<BaseAST>($4);
    ast -> block = unique_ptr<BaseAST>($6);
    $$ = ast;
  }
  ;



FuncFParams
  : FuncFParam {
    auto ast = new FuncFParamsAST();
    ast -> funcflist.push_back(unique_ptr<BaseAST>($1));
    $$ = ast;
  }
  | FuncFParams ',' FuncFParam {
    auto ast = dynamic_cast<FuncFParamsAST*>($1);
    ast -> funcflist.push_back(unique_ptr<BaseAST>($3));
    $$ = ast;
  }




FuncFParam
  : BType IDENT {
    auto ast = new FuncFParamAST();
    ast -> btype = unique_ptr<BaseAST>($1);
    ast -> ident = *unique_ptr<std::string>($2);
    $$ = ast;
  }






Block
  : '{' BlockItems '}' {
    auto block = new BlockAST();
    block -> blockitems = unique_ptr<BaseAST>($2);
    $$ = block;
  }
  | '{' '}' {
    auto block = new BlockAST();
    $$ = block;
  }
  ;




BlockItems
  : BlockItem {
    auto blockitems = new BlockItemsAST();
    blockitems -> item.push_back(unique_ptr<BaseAST>($1));
    $$ = blockitems;    

  }
  | BlockItems BlockItem {
    auto blockitems = dynamic_cast<BlockItemsAST*>($1);
    blockitems -> item.push_back(unique_ptr<BaseAST>($2));
    $$ = blockitems;
  }
  ;




BlockItem 
  : Decl {
    auto blockitem = new BlockItemAST();
    blockitem -> type = BlockItemAST::DECL;
    blockitem -> decl = unique_ptr<BaseAST>($1);
    $$ = blockitem;

  }
  | Stmt {
    auto blockitem = new BlockItemAST();
    blockitem -> type = BlockItemAST::STMT;
    blockitem -> stmt = unique_ptr<BaseAST>($1);
    $$ = blockitem;
  }
  ;




Decl
  : ConstDecl {
    auto decl = new DeclAST(DeclAST::CONST);
    decl -> constdecl = unique_ptr<BaseAST>($1);
    $$ = decl;
  }
  | VarDecl {
    auto decl = new DeclAST(DeclAST::VAR);
    decl -> vardecl = unique_ptr<BaseAST>($1);
    $$ = decl;
  }
  ;



VarDecl
  : BType VarDefs ';'{
    auto vardecl = new VarDeclAST();
    vardecl -> vardefs = unique_ptr<BaseAST>($2);
    $$ = vardecl;
  }
  ;



VarDefs
  : VarDef {
    auto vardefs = new VarDefsAST();
    vardefs -> vardef.push_back(unique_ptr<BaseAST>($1));
    $$ = vardefs;
  }
  | VarDefs ',' VarDef {
    auto vardefs = dynamic_cast<VarDefsAST*>($1);
    vardefs -> vardef.push_back(unique_ptr<BaseAST>($3));
    $$ = vardefs;
     
  }
  ;



VarDef
  : IDENT {
    auto vardef = new VarDefAST(VarDefAST::IDENT);
    vardef -> ident = *unique_ptr<string>($1);
    $$ = vardef;
  }
  | IDENT '=' InitVal {
    auto vardef = new VarDefAST(VarDefAST::IDENTDEF);
    vardef -> ident = *unique_ptr<string>($1);
    vardef -> initval = unique_ptr<BaseAST>($3);
    $$ = vardef;
  }
  | IDENT '[' ConstExp ']' {
    auto vardef = new VarDefAST(VarDefAST::ARRAY);
    vardef -> ident = *unique_ptr<string>($1);
    vardef -> constexp = unique_ptr<BaseAST>($3);
    $$ = vardef;
  } 
  | IDENT '[' ConstExp ']' '=' InitVal {
    auto vardef = new VarDefAST(VarDefAST::ARRAYDEF);
    vardef -> ident = *unique_ptr<string>($1);
    vardef -> constexp = unique_ptr<BaseAST>($3);
    vardef -> initval = unique_ptr<BaseAST>($6);
    $$ = vardef;
  }
  ;



InitVal
  : Exp {
    auto initval = new InitValAST(InitValAST::EXP);
    initval -> exp = unique_ptr<BaseAST>($1);
    $$ = initval;
  }
  | '{' ExpList '}'{
    auto initval = new InitValAST(InitValAST::ARRAY);
    initval -> explist = unique_ptr<BaseAST>($2);
    $$ = initval;
  }
  | '{' '}' {
    auto initval = new InitValAST(InitValAST::ZEROINIT);
    $$ = initval;
  }
  ;






ConstDecl 
  : CONST BType ConstDefs ';' {
    auto constdecl = new ConstDeclAST();
    constdecl -> btype = unique_ptr<BaseAST>($2);
    constdecl -> constdefs = unique_ptr<BaseAST>($3);
    $$ = constdecl;
  }
  ;


BType
  : INT {
    auto btype = new BTypeAST();
    btype -> val = "int";
    $$ = btype;
  }
  ;



ConstDefs 
  : ConstDef {
    auto constdefs = new ConstDefsAST();
    constdefs -> constdef.push_back(unique_ptr<BaseAST>($1));
    $$ = constdefs;

  }
  | ConstDefs ',' ConstDef {
    auto constdefs = dynamic_cast<ConstDefsAST*>($1);
    constdefs -> constdef.push_back(unique_ptr<BaseAST>($3));
    $$ = constdefs;

  }
  ;




ConstDef
  : IDENT '=' ConstInitVal {
    auto constdef = new ConstDefAST(ConstDefAST::CONST);
    constdef -> ident = *unique_ptr<string>($1);
    constdef -> constinitval = unique_ptr<BaseAST>($3); 
    $$ = constdef;    

  }
  | IDENT '[' ConstExp ']' '=' ConstInitVal {
    auto constdef = new ConstDefAST(ConstDefAST::ARRAY);
    constdef -> ident = *unique_ptr<string>($1);
    constdef -> constexp = unique_ptr<BaseAST>($3);
    constdef -> constinitval = unique_ptr<BaseAST>($6);
    $$ = constdef;    
  }
  ;



ConstInitVal
  : ConstExp {
    auto constinitval = new ConstInitValAST(ConstInitValAST::CONSTEXP);
    constinitval -> constexp = unique_ptr<BaseAST>($1);
    $$ = constinitval ;
  }
  | '{' ConstList '}' {
    auto constinitval = new ConstInitValAST(ConstInitValAST::CONSTLIST);
    constinitval -> constlist = unique_ptr<BaseAST>($2);
    $$ = constinitval ;
  }
  | '{' '}' {
    auto constinitval = new ConstInitValAST(ConstInitValAST::ZEROINIT);
    $$ = constinitval ;
  }
  ;





ConstExp
  : Exp {
    auto constexp = new ConstExpAST();
    constexp -> exp = unique_ptr<BaseAST>($1);
    $$ = constexp;
  }
  ;




Stmt
  : RETURN Exp ';' {
    auto stmt = new StmtAST(StmtAST::RETURNEXP);
    stmt -> exp = unique_ptr<BaseAST>($2);
    $$ = stmt;
  }
  | RETURN ';' {
    auto stmt = new StmtAST(StmtAST::RETURNNULL);
    $$ = stmt;
  }
  | Exp ';'{
    auto stmt = new StmtAST(StmtAST::EXP);
    stmt -> exp = unique_ptr<BaseAST>($1);
    $$ = stmt;
  }
  | ';' {
    auto stmt = new StmtAST(StmtAST::NUL);
    $$ = stmt;
  }
  | Block {
    auto stmt = new StmtAST(StmtAST::BLOCK);
    stmt -> block = unique_ptr<BaseAST>($1);
    $$ = stmt;
  }
  | Lval '=' Exp ';'{
    auto stmt = new StmtAST(StmtAST::LVALEXP);
    stmt -> lval = unique_ptr<BaseAST>($1);
    stmt -> exp = unique_ptr<BaseAST>($3);
    $$ = stmt;
  }
  | IF '(' Exp ')' Stmt {
    auto stmt = new StmtAST(StmtAST::IF);
    stmt -> exp = unique_ptr<BaseAST>($3);
    stmt -> if_stmt = unique_ptr<BaseAST>($5);
    $$ = stmt;
  }
  | IF '(' Exp ')' Stmt ELSE Stmt{
    auto stmt = new StmtAST(StmtAST::IFELSE);
    stmt -> exp = unique_ptr<BaseAST>($3);
    stmt -> if_stmt = unique_ptr<BaseAST>($5);
    stmt -> else_stmt = unique_ptr<BaseAST>($7);
    $$ = stmt;
  } 
  | WHILE '(' Exp ')' Stmt {
    auto stmt = new StmtAST(StmtAST::WHILE);
    stmt -> exp = unique_ptr<BaseAST>($3);
    stmt -> while_stmt = unique_ptr<BaseAST>($5);
    $$ = stmt;
  }
  | BREAK ';' {
    auto stmt = new StmtAST(StmtAST::BREAK);
    $$ = stmt;
  }
  | CONTINUE ';' {
    auto stmt = new StmtAST(StmtAST::CONTINUE);
    $$ = stmt;
  }
  ;


ConstList
  : ConstExp {
    auto constexplist = new ConstExpListAST();
    constexplist -> constexplist.push_back(unique_ptr<BaseAST>($1));
    $$ = constexplist;
  }
  | ConstList ',' ConstExp {
    auto constexplist = dynamic_cast<ConstExpListAST*>($1);
    constexplist -> constexplist.push_back(unique_ptr<BaseAST>($3));
    $$ = constexplist;
  }
  ;



ExpList
  : Exp {
    auto explist = new ExpListAST();
    explist -> explist.push_back(unique_ptr<BaseAST>($1));
    $$ = explist;
  }
  | ExpList ',' Exp {
    auto explist = dynamic_cast<ExpListAST*>($1);
    explist -> explist.push_back(unique_ptr<BaseAST>($3));
    $$ = explist;
  }





Exp 
  : LOrExp {
    auto exp = new ExpAST();
    exp -> lorexp = unique_ptr<BaseAST>($1);
    $$ = exp;
  }
  ;




LOrExp
  : LAndExp {
    auto lor = new LOrExpAST(LOrExpAST::LANDEXP);
    lor -> landexp = unique_ptr<BaseAST>($1);
    $$ = lor;
  }
  | LOrExp LOR LAndExp {
    auto lor = new LOrExpAST(LOrExpAST::LORLAND);
    lor -> lorexp = unique_ptr<BaseAST>($1);
    lor -> landexp = unique_ptr<BaseAST>($3);
    $$ = lor;
  }
  ;



  
LAndExp
  : EqExp {
    auto land = new LAndExpAST(LAndExpAST::EQEXP);
    land -> eqexp = unique_ptr<BaseAST>($1);
    $$ = land;
  }
  | LAndExp LAND EqExp {
    auto land = new LAndExpAST(LAndExpAST::LANDEQ);
    land -> landexp = unique_ptr<BaseAST>($1);
    land -> eqexp = unique_ptr<BaseAST>($3);
    $$ = land;
  }
  ;




EqExp
  : RelExp {
    auto eq = new EqExpAST(EqExpAST::RELEXP);
    eq -> relexp = unique_ptr<BaseAST>($1);
    $$ = eq;
  }
  | EqExp EQ RelExp{
    auto eq = new EqExpAST(EqExpAST::EQREL);
    eq -> eqexp = unique_ptr<BaseAST>($1);
    eq -> op = "==";
    eq -> relexp = unique_ptr<BaseAST>($3);
    $$ = eq;
  }
  | EqExp NE RelExp{
    auto eq = new EqExpAST(EqExpAST::EQREL);
    eq -> eqexp = unique_ptr<BaseAST>($1);
    eq -> op = "!=";
    eq -> relexp = unique_ptr<BaseAST>($3);
    $$ = eq;
  }
  ;




RelExp
  : AddExp {
    auto rel = new RelExpAST(RelExpAST::ADDEXP);
    rel -> addexp = unique_ptr<BaseAST>($1);
    $$ = rel;
  }
  | RelExp LT AddExp {
    auto rel = new RelExpAST(RelExpAST::RELADD);
    rel -> relexp = unique_ptr<BaseAST>($1);
    rel -> op = "<";
    rel -> addexp = unique_ptr<BaseAST>($3);
    $$ = rel;
  }
  | RelExp GT AddExp {
    auto rel = new RelExpAST(RelExpAST::RELADD);
    rel -> relexp = unique_ptr<BaseAST>($1);
    rel -> op = ">";
    rel -> addexp = unique_ptr<BaseAST>($3);
    $$ = rel;
  }
  | RelExp LE AddExp {
    auto rel = new RelExpAST(RelExpAST::RELADD);
    rel -> relexp = unique_ptr<BaseAST>($1);
    rel -> op = "<=";
    rel -> addexp = unique_ptr<BaseAST>($3);
    $$ = rel;
  }
  | RelExp GE AddExp {
    auto rel = new RelExpAST(RelExpAST::RELADD);
    rel -> relexp = unique_ptr<BaseAST>($1);
    rel -> op = ">=";
    rel -> addexp = unique_ptr<BaseAST>($3);
    $$ = rel;
  }
  ;
  



AddExp
  : MulExp {
    auto addexp = new AddExpAST(AddExpAST::MULONLY);
    addexp -> mulexp = unique_ptr<BaseAST>($1);
    $$ = addexp;
  }
  | AddExp '+' MulExp {
    auto addexp = new AddExpAST(AddExpAST::ADDOPMUL);
    addexp -> addexp = unique_ptr<BaseAST>($1);
    addexp -> mulexp = unique_ptr<BaseAST>($3);
    addexp -> op = "+";
    $$ = addexp;
  }
  | AddExp '-' MulExp {
    auto addexp = new AddExpAST(AddExpAST::ADDOPMUL);
    addexp -> addexp = unique_ptr<BaseAST>($1);
    addexp -> mulexp = unique_ptr<BaseAST>($3);
    addexp -> op = "-";
    $$ = addexp;
  }
  ;




MulExp
  : UnaryExp {
    auto mulexp = new MulExpAST(MulExpAST::UNARYEXP);
    mulexp -> unrayexp = unique_ptr<BaseAST>($1);
    $$ = mulexp;
  }
  | MulExp '*' UnaryExp {
    auto mulexp = new MulExpAST(MulExpAST::MULOPUNRAY);
    mulexp -> mulexp = unique_ptr<BaseAST>($1);
    mulexp -> unrayexp = unique_ptr<BaseAST>($3);
    mulexp -> op = "*";
    $$ = mulexp;
  } 
  | MulExp '/' UnaryExp {
    auto mulexp = new MulExpAST(MulExpAST::MULOPUNRAY);
    mulexp -> mulexp = unique_ptr<BaseAST>($1);
    mulexp -> unrayexp = unique_ptr<BaseAST>($3);
    mulexp -> op = "/";
    $$ = mulexp;
  }
  | MulExp '%' UnaryExp {
    auto mulexp = new MulExpAST(MulExpAST::MULOPUNRAY);
    mulexp -> mulexp = unique_ptr<BaseAST>($1);
    mulexp -> unrayexp = unique_ptr<BaseAST>($3);
    mulexp -> op = "%";
    $$ = mulexp;
  }
  ;




PrimaryExp
  : '(' Exp ')'{
    auto primaryexp = new PrimaryExpAST(PrimaryExpAST::EXP);
    primaryexp -> exp = unique_ptr<BaseAST>($2);
    $$ = primaryexp;
  } 
  | Number {
    auto primaryexp = new PrimaryExpAST(PrimaryExpAST::NUMBER);
    primaryexp -> number = unique_ptr<BaseAST>($1);
    $$ = primaryexp;
  }
  | Lval {
    auto primaryexp = new PrimaryExpAST(PrimaryExpAST::LVAL);
    primaryexp -> lval = unique_ptr<BaseAST>($1);
    $$ = primaryexp;
  }
  ;




UnaryExp
  : PrimaryExp {
    auto unaryexp = new UnaryExpAST(UnaryExpAST::PRIMARYEXP);
    unaryexp -> primaryexp = unique_ptr<BaseAST>($1);
    $$ = unaryexp;
  }
  | UnaryOp UnaryExp{
    auto unaryexp = new UnaryExpAST(UnaryExpAST::UNARYEXP);
    unaryexp -> unaryop = *unique_ptr<std::string>($1);
    unaryexp -> unaryexp = unique_ptr<BaseAST>($2);
    $$ = unaryexp;
  }
  | IDENT '(' ')' {
    auto unaryexp = new UnaryExpAST(UnaryExpAST::FUNCVOID);
    unaryexp -> ident = *unique_ptr<std::string>($1);
    $$ = unaryexp;
  }
  | IDENT '(' FuncRParams ')' {
    auto unaryexp = new UnaryExpAST(UnaryExpAST::FUNCRPARAMS);
    unaryexp -> ident = *unique_ptr<std::string>($1);
    unaryexp -> funcrparams = unique_ptr<BaseAST>($3);
    $$ = unaryexp;
  }
  ;



FuncRParams 
  : Exp {
    auto ast = new FuncRParamsAST();
    ast -> explist.push_back(unique_ptr<BaseAST>($1));
    $$ = ast;
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
    auto int_con = new NumberAST();
    int_con -> int_const = ($1);
    $$ = int_con;
  }
  ;


Lval
  : IDENT {
    auto lval = new LValAST(LValAST::VAR);
    lval -> ident = *unique_ptr<string>($1);
    $$ = lval;

  }
  | IDENT '[' Exp ']' {
    auto lval = new LValAST(LValAST::ARRAY);
    lval -> ident = *unique_ptr<string>($1);
    lval -> address = unique_ptr<BaseAST>($3);
    $$ = lval;
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