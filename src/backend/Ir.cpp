#include "../include/include.h"
#include "../include/Ir.h"
#include "../include/Ir_generator.h"



GenContext* GenContext::current_ctx = nullptr;








// 各类型IR Dump() 函数的定义





void ReturnIRValue::Dump() const{
    if(type == ReturnIRValue::VALUE){
        std::cout<<"  ret ";
        if(auto result = dynamic_cast<TemporaryIRValue* >(return_value.get())){
            result -> Dump() ;
        }
        else if(auto result = dynamic_cast<IntegerIRValue* >(return_value.get())){
            result -> Dump() ;   
        }
        else {
            std::cout<<"<unknown> ";
        }
        std::cout<<std::endl;
    }
    else {
        std::cout<<"  ret "<<std::endl;
    }
}



void AllocIRValue::Dump() const {
    std::cout << "  " << var_name << " = alloc " << data_type << std::endl;
}


void StoreIRValue::Dump() const {
    std::cout << "  store ";
    if (value) {
        value->Dump();  
    }
    std::cout << ", " << dest << std::endl;
}


void LoadIRValue::Dump() const {
    std::cout << "  " << result_name << " = load " << src << std::endl;
}




void IntegerIRValue::Dump() const{
    std::cout<<value;
}




void BinaryIRValue::Dump() const{
    std::string operation_name;
    switch (operation)
    {
        case ADD:
        std::cout<<"  "<<result_name << " = " << "add " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;  
        
        case SUB:
        std::cout<<"  "<<result_name << " = " << "sub " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case EQ:
        std::cout<<"  "<<result_name << " = " << "eq " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case MUL:
        std::cout<<"  "<<result_name << " = " << "mul " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case DIV:
        std::cout<<"  "<<result_name << " = " << "div " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case MOD:
        std::cout<<"  "<<result_name << " = " << "mod " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case NE:
        std::cout<<"  "<<result_name << " = " << "ne " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case LT:
        std::cout<<"  "<<result_name << " = " << "lt " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case GT:
        std::cout<<"  "<<result_name << " = " << "gt " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case GE:
        std::cout<<"  "<<result_name << " = " << "ge " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case LE:
        std::cout<<"  "<<result_name << " = " << "le " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case AND:
        std::cout<<"  "<<result_name << " = " << "and " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        case OR:
        std::cout<<"  "<<result_name << " = " << "or " ; 
        left->Dump();
        std::cout<<", ";
        right->Dump(); 
        std::cout<< std::endl;
        break;
        
        
        default:
        break;
    }
}

void TemporaryIRValue ::Dump() const{
    std::cout<< temp_name;
}


void JumpIRValue::Dump() const {
    std::cout << "  jump " << target_label << std::endl;
}




void CallIRValue::Dump() const {
    std::cout << "  ";
    if (type == OTHER) {
        std::cout << result_name << " = ";
    }
    std::cout << "call " << func_name << "(";
    for (size_t i = 0; i < funcrparams.size(); ++i) {
        funcrparams[i]->Dump();
        if (i < funcrparams.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << ")" << std::endl;
}




void GlobalAllocIRValue::Dump() const {
    std::cout << "global " << var_name << " = alloc " << data_type << ", ";
    if (type == VAR) {
        if (auto val = dynamic_cast<IntegerIRValue*>(value.get())) {
            if (val->value == 0) {
                std::cout << "zeroinit";
            } else {
                std::cout << val->value;
            }
        } else {
            std::cout << "zeroinit";
        }
    } else if (type == ARRAY) {
        if (init_list.empty()) {
            std::cout << "zeroinit";
        } else {
            std::cout << "{";
            for (size_t i = 0; i < init_list.size(); ++i) {
                std::cout << init_list[i];
                if (i < init_list.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "}";
        }
    }
    std::cout << std::endl;
}


void GetElemPtrIRValue::Dump() const {
    std::cout << "  " << result_name << " = getelemptr " << src << ", ";
    index->Dump();
    std::cout << std::endl;
}


void GetPtrIRValue::Dump() const {
    std::cout << "  " << result_name << " = getptr " << src << ", ";
    index->Dump();
    std::cout << std::endl;
}





void BranchIRValue::Dump() const {
    std::cout << "  br ";
    if (condition) condition->Dump();
    std::cout << ", " << true_label << ", " << false_label << std::endl;
}


void IRBasicBlock::DumpValue() const{
    std::cout << "\n" << block_name << ":" << std::endl;
    for(const auto &val : ir_value){
        val -> Dump();
    }    
}


void IRFunction::DumpBlock() const{
    std::cout << "fun " << function_name << "(";
    for(size_t i = 0; i < funcfparams.size(); ++i) {
        std::cout << funcfparams[i].first << ": " << funcfparams[i].second;
        if(i < funcfparams.size() - 1) std::cout << ", ";
    }
    std::cout << ")";
    if(functype != "void") std::cout << ": " << functype;
    std::cout << " {\n";
    for(const auto &block : ir_basicblock){
        block -> DumpValue();
    }
    std::cout << "\n}\n\n";
}


void IRProgram::DumpFunction() const{
    std::cout << "decl @getint(): i32" << std::endl;
    std::cout << "decl @getch(): i32" << std::endl;
    std::cout << "decl @getarray(*i32): i32" << std::endl;
    std::cout << "decl @putint(i32)" << std::endl;
    std::cout << "decl @putch(i32)" << std::endl;
    std::cout << "decl @putarray(i32, *i32)" << std::endl;
    std::cout << "decl @starttime()" << std::endl;
    std::cout << "decl @stoptime()" << std::endl;
    std::cout << std::endl;

    for(const auto &gv : global_value){
        gv -> Dump();
    }
    
    if(!global_value.empty()){
        std::cout<<std::endl;
    }

    for(const auto &fun : ir_function){
        fun -> DumpBlock();
    }
}



//  IR 中的 ADD 函数定义



void IRFunction::ADD_Block(std::unique_ptr<IRBasicBlock> block){
    ir_basicblock.push_back(std::move(block));
}


void IRBasicBlock::ADD_Value(std::unique_ptr<BaseIRValue> val){
    ir_value.push_back(std::move(val));
}


void IRProgram::ADD_Function(std::unique_ptr<IRFunction> func){
    ir_function.push_back(std::move(func));
}


void IRProgram::ADD_Globalvalue(std::unique_ptr<BaseIRValue> val){
    global_value.push_back(std::move(val));
}
