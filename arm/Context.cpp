#include "Context.h"
void Context::init_ir_to_time(IR* ir){
    ir_to_time.insert({ir,time++});
}
void Context::init_var_define_to_time(IR* ir)
{
    //以下是可能出现新变量的指令
    //     _ALLOC, // alloc opn1(变量名) : result(字节数)
    //     _ADDR,  // result = &opn1
    //     _ADD,   // result = opn1 + opn2
    //     _SUB,   // result = opn1 - opn2
    //     _MUL,   // result = opn1 * opn2
    //     _DIV,   // result = opn1 / opn2
    //     _MOD,   // result = opn1 % opn2
    //     _ASSIGN,// result = opn1
    //     _ASSIGN_Arr,  //result=opn1[opn2]
    //     _NOT,   // result = ! opn1
    //     _POSI,  // result = + opn1
    //     _NEGA,  // result = - opn1
    //     _CALL,  // [result =] call opn1(函数) , opn2(参数个数)
    switch (ir->op){
        case IR::_VOID:     case IR::_LABEL:    case IR::_GOTO:     case IR::_JEQ:
        case IR::_JGE:      case IR::_JGT:      case IR::_JLE:      case IR::_JLT:
        case IR::_JNE:      case IR::_FUNC:     case IR::_FUNC_END: case IR::_PARAM:
        case IR::_RET:      case IR::_DATA_BEGIN:   case IR::_DATA_END:     case IR::_Arr_ASSIGN:
            return ;
            break;
        default:
            break;
    }
    int cur_time=ir_to_time[ir];
    if(ir->op!=IR::_ALLOC){
        //新变量只会出现在result操作数中
        if(ir->result.is_Var() || ir->result.is_Array()){
            if(var_define_to_time.count(ir->result.name)==0){
                var_define_to_time[ir->result.name]=cur_time;
                time_to_var_define.insert(
                    make_pair(cur_time,ir->result.name)
                );
            }
        }
    }
    else{//ALLOC指令中opn1才是新变量
        if(ir->opn1.is_Var() || ir->opn1.is_Array()){
            if(var_define_to_time.count(ir->opn1.name)==0){
                var_define_to_time[ir->opn1.name]=cur_time;
                time_to_var_define.insert(
                    make_pair(cur_time,ir->opn1.name)
                );
            }
        }
    } 
}

void Context::update_lastused_time(int time,Opn& opn)
{
    if(opn.is_Array() || opn.is_Var()){
        var_lastused_to_time[opn.name]=time;
        time_to_var_lastused.insert(
            make_pair(time,opn.name)
        );
    }else if(opn.is_Block()){
        for(auto& ptr:opn.Block_args){
            update_lastused_time(time,*ptr);
        }
    }

}

void Context::init_var_lastused_to_time(IR* ir)
{
    int cur_time=ir_to_time[ir];
    switch (ir->op){
        case IR::_DATA_BEGIN://这些指令不会涉及到变量的使用
        case IR::_DATA_END:
        case IR::_FUNC:     
        case IR::_FUNC_END: 
        case IR::_VOID:
        case IR::_LABEL: 
        case IR::_GOTO:   
            return ;
            break;  
        case IR::_ADD://这些指令的opn1、opn2、reuslt可能会使用到变量
        case IR::_SUB:
        case IR::_DIV:
        case IR::_MUL:
        case IR::_MOD:
        case IR::_Arr_ASSIGN:
        case IR::_ASSIGN_Arr:
            update_lastused_time(cur_time,ir->opn1);
            update_lastused_time(cur_time,ir->opn2);
            update_lastused_time(cur_time,ir->result);
            break;
        case IR::_JEQ://这几个指令的opn1和opn2可能会使用到变量
        case IR::_JGE:      
        case IR::_JGT:      
        case IR::_JLE:      
        case IR::_JLT:
        case IR::_JNE:
            update_lastused_time(cur_time,ir->opn1);
            update_lastused_time(cur_time,ir->opn2);
            break;
        case IR::_PARAM://这几个指令的opn1可能会使用到变量
        case IR::_RET:          
            update_lastused_time(cur_time,ir->opn1);
            break;
        case IR::_NOT://这些指令的opn1、reuslt可能会使用到变量
        case IR::_POSI:
        case IR::_NEGA:
        case IR::_ADDR:
        case IR::_ASSIGN:
        case IR::_ALLOC:
            update_lastused_time(cur_time,ir->opn1);
            update_lastused_time(cur_time,ir->result);
            break;
        case IR::_CALL://这些指令opn2、result可能会使用到变量
            update_lastused_time(cur_time,ir->opn2);
            update_lastused_time(cur_time,ir->result);
            break;
        default:
            break;
    }
}