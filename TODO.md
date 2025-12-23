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
- [x] 符号表
- [x] ir generator (关于 常量的生成， 新的ir value的定义 ...)
- [x] koopa ir output
- [x] to_riscv

对于 5.1

- [x] ast
- [x] bison  (new stmt)
- [x] 符号表
- [x] ir generator

✅

对于 6.1

- [x] ast
- [x] bison  (new stmt)
- [x] 符号表
- [x] ir generator
- [x] koopa ir output
- [x] to_riscv

对于 6.2

做过了

对于 7.1


- [x] ast
- [x] bison  (new stmt)
- [x] ir generator

仿照 if 去做即可

对于 7.2

- [x] ast
- [x] bison  (new stmt)
- [x] 符号表
- [x] ir generator

对于 8.1

- [x] ast
- [x] bison  
- [x] 符号表
- [x] ir generator
- [x] koopa ir output
- [x] to_riscv


对于 9.1 

为了实现数组相关的 IR 生成及最终的 RISC-V 代码生成，你需要按照以下步骤进行修改和扩展。这是一个涉及前端 AST 解析、中间代码结构定义、IR 生成逻辑以及后端汇编生成的完整流程。

以下是你需要做的事情清单：

### 1. 修改 IR 数据结构定义 (Ir.h)

你需要定义新的 IR 指令类来表示数组操作。

*   **新增 `GetElemPtrIRValue` 类**：
    *   用于表示 `getelemptr` 指令。
    *   **成员**：
        *   `result_name`: 结果保存的临时变量名（如 `%0`）。
        *   src: 源指针变量名（如 `@arr` 或 `%arr_ptr`）。
        *   `index`: 索引值（可以是 `IntegerIRValue` 或 `TemporaryIRValue`）。
    *   **功能**：计算 `src + index * element_size` 的地址。

*   **扩展 `AllocIRValue` 类**：
    *   目前的 `alloc` 只支持标量。需要修改它以支持数组类型字符串，例如 `[i32, 5]`。
    *   或者添加一个 `size` 成员来表示数组大小。

*   **扩展 `GlobalAllocIRValue` 类**：
    *   目前的全局变量初始化只支持单个 `IntegerIRValue`。
    *   需要支持 **聚合类型（Aggregate）** 初始化，即一个 `std::vector<int>` 或类似的结构，用于存储 `{10, 20, 0, ...}` 这样的初始化列表。

### 2. 扩展符号表 (symbol.h)

*   **记录数组维度**：
    *   修改 `Symbol` 类，增加一个成员（如 `std::vector<int> dims`）来存储数组的维度信息。
    *   如果是标量，`dims` 为空；如果是 `int a[2][3]`，`dims` 为 `{2, 3}`。
*   **辅助函数**：
    *   可能需要 helper 函数来计算数组的总大小（元素个数）。

### 3. 实现 IR 生成逻辑 (Ir_generator.cpp)

这是工作量最大的部分。

*   **处理数组声明 (`visitVarDef` / `visitConstDef`)**：
    *   **全局数组**：
        *   解析 `InitValAST`（初始化列表）。
        *   将嵌套的初始化列表（如 `{{1, 2}, {3, 4}}`）**扁平化** 为一维列表。
        *   处理 **零填充**：如果初始化列表长度小于数组总大小，剩余部分补 0。
        *   生成带有完整初始化数据的 `GlobalAllocIRValue`。
    *   **局部数组**：
        *   生成 `alloc` 指令，类型为数组类型（如 `[i32, 10]`）。
        *   解析初始化列表（同样需要扁平化）。
        *   **生成初始化代码**：
            *   遍历扁平化后的初始化列表。
            *   对于每个非零元素：
                1.  生成 `GetElemPtrIRValue` 指令，计算该元素地址（`%ptr = getelemptr @arr, index`）。
                2.  生成 `StoreIRValue` 指令，将值存入该地址（`store val, %ptr`）。
            *   对于未显式初始化的部分（如果 Koopa IR 要求显式初始化，或者为了安全），生成 `store 0`。通常局部变量未初始化是未定义的，但如果有初始化列表，未提及的部分必须为 0。

*   **处理数组访问 (`visitLVal`)**：
    *   在 `visitLVal` 中，如果检测到 `LValAST` 带有索引（`indices` 不为空）：
        *   需要生成 `getelemptr` 指令。
        *   **多维数组处理**：
            *   Koopa IR 的 `getelemptr` 对多维数组的处理通常是逐层降维，或者你可以手动计算线性偏移量（`offset = i * M + j`），然后做一次 `getelemptr`（视 Koopa 规范和你定义的类型而定，通常逐层 `getelemptr` 更符合 IR 语义）。
            *   对于一维数组 `a[i]`，生成 `%ptr = getelemptr @a, %i`。
    *   **左值 vs 右值**：
        *   如果 `LVal` 是赋值语句的左边（`StmtAST::LVALEXP`），你需要的是 `getelemptr` 算出来的 **地址**。
        *   如果 `LVal` 是表达式的一部分（`PrimaryExpAST::LVAL`），你需要先算出地址，然后生成 `load` 指令取出 **值**。

### 4. 实现 RISC-V 代码生成 (Ir.cpp)

*   **栈空间分配**：
    *   在 `To_RiscV` 的第一遍扫描中，遇到数组类型的 `AllocIRValue`，需要分配 `size * 4` 字节的栈空间，而不仅仅是 4 字节。

*   **实现 `GetElemPtrIRValue::To_RiscV`**：
    *   **逻辑**：`目标地址 = 基地址 + 索引 * 4`。
    *   **基地址获取**：
        *   如果是全局数组（`@` 开头）：使用 `la t0, arr_name` 加载地址。
        *   如果是局部数组（栈上）：计算其相对于 `sp` 的偏移，使用 `addi t0, sp, offset` 获取地址。
        *   如果是指针变量（函数参数传数组）：`lw t0, offset(sp)` 加载指针值。
    *   **索引计算**：
        *   如果是立即数：直接 `addi` 或 `li`。
        *   如果是变量：`lw` 到寄存器，然后 `slli` (左移 2 位，相当于乘 4)，最后 `add` 到基地址上。
    *   **结果保存**：将计算出的地址保存到目标寄存器或栈上的临时变量位置。

*   **实现 `GlobalAllocIRValue::To_RiscV` (数组版)**：
    *   生成 `.data` 段。
    *   使用 `.word val1, val2, ...` 依次输出初始化列表的值。
    *   如果全是 0，使用 `.zero size_in_bytes`。

### 总结

你需要按照 **IR 定义 -> 符号表 -> IR 生成 -> 汇编生成** 的顺序，依次打通数组的声明、初始化和访问流程。重点在于正确处理 **初始化列表的扁平化与补零** 以及 **`getelemptr` 地址计算**。