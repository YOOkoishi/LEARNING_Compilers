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

using namespace std;

%}

// 定义 parser 函数和错误处理函数的附加参数
// 我们需要返回一个字符串作为 AST, 所以我们把附加参数定义成字符串的智能指针
// 解析完成后, 我们要手动修改这个参数, 把它设置成解析得到的字符串
%parse-param { std::unique_ptr<BaseAST> &ast }

// yylval 的定义, 我们把它定义成了一个联合体 (union)
// 因为 token 的值有的是字符串指针, 有的是整数
// 之前我们在 lexer 中用到的 str_val 和 int_val 就是在这里被定义的
// 至于为什么要用字符串指针而不直接用 string 或者 unique_ptr<string>?
// 请自行 STFW 在 union 里写一个带析构函数的类会出现什么情况
%union {
  std::string *str_val;
  int int_val;
  BaseAST *ast_val;
}

// lexer 返回的所有 token 种类的声明
// 注意 IDENT 和 INT_CONST 会返回 token 的值, 分别对应 str_val 和 int_val
%token INT RETURN 
%token <str_val> IDENT
%token <int_val> INT_CONST

%token LE GE EQ NE LT GT 
%token LOR LAND

%left LOR
%left LAND
%left EQ NE
%left LT LE GT GE
%left '+' '-'
%left '*' '/' '%'

// 非终结符的类型定义
%type <ast_val> FuncDef FuncType Block Stmt Number Exp UnaryExp PrimaryExp AddExp MulExp LOrExp EqExp RelExp LAndExp  
/* %type <int_val>  */
%type <str_val> UnaryOp

%%


CompUnit
  : FuncDef {
    auto comp_unit = make_unique<CompUnitAST>();
    comp_unit -> fun_def = unique_ptr<BaseAST>($1);
    ast = move(comp_unit);
  }
  ;


FuncDef
  : FuncType IDENT '(' ')' Block {
    auto ast = new FunDefAST();
    ast -> fun_type = unique_ptr<BaseAST>($1);
    ast -> ident = *unique_ptr<string>($2);
    ast -> block = unique_ptr<BaseAST>($5);
    $$ = ast;
  }
  ;


FuncType
  : INT {
    auto ast = new FunTypeAST();
    ast -> tp = "int";
    $$ = ast;

  }
  ;


Block
  : '{' Stmt '}' {
    auto block = new BlockAST();
    block -> stmt = unique_ptr<BaseAST>($2);
    $$ = block;
  }
  ;


Stmt
  : RETURN Exp ';' {
    auto stmt = new StmtAST();
    stmt -> exp = unique_ptr<BaseAST>($2);
    $$ = stmt;
  }
  ;


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
  ;


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

%%

// 定义错误处理函数, 其中第二个参数是错误信息
// parser 如果发生错误 (例如输入的程序出现了语法错误), 就会调用这个函数
void yyerror(unique_ptr<BaseAST> &ast, const char *s) {
  cerr << "error: " << s << endl;
}