# Compiler TODO List

## 🚧

对于 3.1

- [x] 基本 AST 结构设计完成
- [x] Bison 语法规则完成（一元表达式）
- [x] Flex 词法分析完成
- [x] 基本 IR 类框架搭建
- [x] IR 生成器实现 (unaryEXP的visit(重点))
- [x] IR 输出格式实现
- [x] RISC-V 代码生成

对于 3.2

- [x] ast
- [x] bison
- [x] IR Generator
- [x] Koopa IR output
- [x] To_RiscV

对于 3.3

- [x] ast
- [x] bison
- [x] IR Generator
- [x] Koopa IR output
- [x] To_RiscV

对于 4.1

- [x] ast
- [x] bison
- [x] 符号表
- [x] IR Generator (关于 常量的生成， 新的IR value的定义 ...)
- [ ] Koopa IR output
- [ ] To_RiscV

孩子们，这上面两个好像不用做。


关于 多个 block 和 多个 function 的可能情况，现在还没有做出处理，暂时mark一下。

对于 4.2


- [x] ast
- [x] bison
- [ ] 符号表
- [ ] IR Generator (关于 常量的生成， 新的IR value的定义 ...)
- [ ] Koopa IR output
- [ ] To_RiscV

