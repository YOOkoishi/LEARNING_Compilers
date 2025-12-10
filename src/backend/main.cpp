#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include "../include/ast.h"
#include "../include/Ir.h"
#include "../include/Ir_generator.h"

using namespace std;

// 声明 lexer 的输入, 以及 parser 函数
// 为什么不引用 sysy.tab.hpp 呢? 因为首先里面没有 yyin 的定义
// 其次, 因为这个文件不是我们自己写的, 而是被 Bison 生成出来的
// 你的代码编辑器/IDE 很可能找不到这个文件, 然后会给你报错 (虽然编译不会出错)
// 看起来会很烦人, 于是干脆采用这种看起来 dirty 但实际很有效的手段
extern int yydebug;
extern int yy_flex_debug;  // Flex 的调试开关
extern FILE *yyin;
extern int yyparse(unique_ptr<BaseAST> &ast);

int main(int argc, const char *argv[]) {
  // 默认关闭调试模式
  yydebug = 0;
  yy_flex_debug = 0;  // 关闭 Flex 调试
  bool debug_mode = false;

  // 检查是否有 -debug 参数
  for (int i = 1; i < argc; i++) {
    if (string(argv[i]) == "-debug") {
      debug_mode = true;
      yydebug = 1;           // 开启 Bison 调试
      yy_flex_debug = 1;     // 开启 Flex 调试
      break;
    }
  }

  // 解析命令行参数. 测试脚本/评测平台要求你的编译器能接收如下参数:
  // compiler 模式 输入文件 -o 输出文件
  // 或者：compiler 模式 输入文件 -o 输出文件 -debug
  assert(argc == 5 || argc == 6);
  auto mode = argv[1];
  auto input = argv[2];
  auto output = argv[4];

  // 打开输入文件, 并且指定 lexer 在解析的时候读取这个文件
  yyin = fopen(input, "r");
  assert(yyin);
  
  // 如果不是调试模式，重定向输出到文件
  if (!debug_mode) {
    freopen(output, "w", stdout);
  }
  
  // 调用 parser 函数, parser 函数会进一步调用 lexer 解析输入文件的
  unique_ptr<BaseAST> ast;
  auto ret = yyparse(ast);
  if(ret != 0){
    fprintf(stderr, "ERROR: yyparse returned %d\n", ret);
    fprintf(stderr, "Parse failed!\n");
    return ret;
  }
  assert(!ret);
  
  std::string mode_str(mode);
  if(mode_str == "-ast"){
    ast -> Dump();
    return 0;
  }
  
  IRGenerator ir;
  if(ast){
    if(auto tp_ast = dynamic_cast<const CompUnitAST*>(ast.get())){
      ir.visitCompUnit(tp_ast);
    }
  }
  
  auto ir_fin = ir.get_irprogram();
  if(mode_str == "-koopa")ir_fin -> DumpFunction();
  else if(mode_str == "-riscv")ir_fin -> To_RiscV();
  
  cout << endl;
  return 0;
}