#pragma once

#include "include.h"
#include "Ir.h"
#include "ast.h"
#include "symbol.h"
#include <unordered_map>





// 栈帧管理器
// 栈布局 (从低地址到高地址):
//   sp+0 ~ sp+A-1:          传参空间 (第9+参数)
//   sp+A ~ sp+A+S-1:        局部变量
//   sp+frame_size-4:        ra (如果有call)
class StackFrameManager {
private:
    std::unordered_map<std::string, int> var_offsets;  // 存储的是相对于 S 区域的偏移
    std::unordered_map<std::string, int> param_regs;  // 参数名 -> 寄存器编号 (0-7 for a0-a7)
    std::unordered_map<std::string, int> param_stack_offsets;  // 第9+参数 -> 调用者栈上的偏移 (0, 4, 8, ...)
    int S = 0;  // 局部变量空间
    int R = 0;  // ra 空间 (4 if has call, else 0)
    int A = 0;  // 传参空间 max(max_args - 8, 0) * 4
    int total_frame_size = 0;
    
public:
    // 分配局部变量，返回相对于 S 区域的偏移
    int allocate(const std::string& name, int size = 4) {
        int offset = S;
        var_offsets[name] = offset;  // 存储相对于 S 区域的偏移
        S += size;
        return offset;
    }
    
    // 获取局部变量的实际栈偏移 (sp + A + offset)
    int getOffset(const std::string& name) const {
        auto it = var_offsets.find(name);
        if (it != var_offsets.end()) {
            return A + it->second;  // 实际偏移 = A + S区域内偏移
        }
        return -1;
    }
    
    // 注册参数到寄存器的映射 (前8个参数)
    void registerParam(const std::string& name, int reg_num) {
        param_regs[name] = reg_num;
    }
    
    // 注册栈上参数 (第9个及以后)
    // 被调用者读取时，这些参数在 sp + frame_size + stack_offset
    void registerStackParam(const std::string& name, int stack_offset) {
        param_stack_offsets[name] = stack_offset;
    }
    
    // 获取参数对应的寄存器编号，-1 表示不是寄存器参数
    int getParamReg(const std::string& name) const {
        auto it = param_regs.find(name);
        if (it != param_regs.end()) {
            return it->second;
        }
        return -1;
    }
    
    // 获取栈上参数的偏移，-1 表示不是栈上参数
    int getStackParamOffset(const std::string& name) const {
        auto it = param_stack_offsets.find(name);
        if (it != param_stack_offsets.end()) {
            return it->second;
        }
        return -1;
    }
    
    void setR(int r) { R = r; }
    void setA(int a) { A = a; }
    int getR() const { return R; }
    int getA() const { return A; }
    int getS() const { return S; }
    
    // 计算总栈帧大小: (S + R + A) 向上取整到 16
    void calculateFrameSize() {
        total_frame_size = (S + R + A + 15) / 16 * 16;
    }
    
    int getFrameSize() const {
        return total_frame_size;
    }
    
    // ra 存储位置: sp + S' - 4 (即栈帧顶部)
    int getRaOffset() const {
        return total_frame_size - 4;
    }
    
    void reset() {
        var_offsets.clear();
        param_regs.clear();
        param_stack_offsets.clear();
        S = 0;
        R = 0;
        A = 0;
        total_frame_size = 0;
    }
};


struct LoopInfo {
    std::string entry_label;
    std::string end_label;
};


struct GenContext
{
    IRBasicBlock* current_block = nullptr;
    SymbolTable* symbol_table = nullptr;
    IRProgram* program = nullptr;
    StackFrameManager stack;
    std::vector<LoopInfo> loop_stack;

    static GenContext* current_ctx;
};



class IRGenerator {
private:
    std::unique_ptr<IRProgram> program;
    static int blockcount;
    std::unique_ptr<SymbolTable> symbol_table;

    GenContext ctx;

    int temp_counter = 0;
    std::string generate_temp_name() {
        return "%" + std::to_string(temp_counter++);
    }
    
    int evaluateConstExp(const BaseAST* ast);
public:
    IRGenerator();
    void visitCompUnit(const CompUnitAST* ast);
    void visitFunDef(const FunDefAST* ast);
    void visitFunType(const FunTypeAST* ast);
    void visitBlock(const BlockAST* ast );
    void visitStmt(const StmtAST* ast );
    void visitBlockItems(const BlockItemsAST* ast);
    void visitBlockItem(const BlockItemAST* ast);
    void visitDecl(const DeclAST* ast);
    void visitConstDecl(const ConstDeclAST* ast);
    void visitConstDefs(const ConstDefsAST* ast);
    void visitConstDef(const ConstDefAST* ast);
    std::vector<int> visitConstInitVal(const ConstInitValAST* ast,int array_size);
    void visitVarDecl(const VarDeclAST* ast);
    void visitVarDefs(const VarDefsAST* ast);
    void visitVarDef(const VarDefAST* ast);
    void visitInitValAST(const InitValAST* ast);
    std::vector<int> evaluateGlobalInitVal(const InitValAST* ast, int array_size);
    void visitArrayInit(const std::string& base_addr, const InitValAST* ast, int array_size);
    


    std::unique_ptr<BaseIRValue> visitExp(const ExpAST* ast );
    std::unique_ptr<BaseIRValue> visitAddExp(const AddExpAST* ast );
    std::unique_ptr<BaseIRValue> visitMulExp(const MulExpAST* ast );
    std::unique_ptr<BaseIRValue> visitLOrExp(const LOrExpAST* ast );
    std::unique_ptr<BaseIRValue> visitLAndExp(const LAndExpAST* ast );
    std::unique_ptr<BaseIRValue> visitEqExp(const EqExpAST* ast );
    std::unique_ptr<BaseIRValue> visitRelExp(const RelExpAST* ast );
    std::unique_ptr<BaseIRValue> visitPrimaryExp(const PrimaryExpAST* ast );
    std::unique_ptr<BaseIRValue> visitUnaryExp(const UnaryExpAST* ast );
    std::unique_ptr<BaseIRValue> visitNumber(const NumberAST* ast );
    std::unique_ptr<BaseIRValue> visitConstExp(const ConstExpAST* ast);

    std::unique_ptr<IRProgram> get_irprogram();


    std::string generate_data_type(const std::vector<int>& dim);
    std::vector<int> generate_float_array(const InitValAST* ast, const std::vector<int>& dim);

    void setCurrentBlock(IRBasicBlock* block){
        ctx.current_block = block;
    }

    IRBasicBlock* getCurrentBlock() const {
        return ctx.current_block;
    }

};