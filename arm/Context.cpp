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

void Context::clear_inactive_intervals(int cur_time){
    for(int i=0;i<reg_count;i++){
        if(!used_reg[i]) continue;
        //寄存器i正在被使用
        if( reg_to_var.find(i) == reg_to_var.end() 
            || var_lastused_to_time.find(reg_to_var[i]) == var_lastused_to_time.end()
            || cur_time > var_lastused_to_time[reg_to_var[i]])//当前时间大于变量最后一次使用的时间
        {
            used_reg[i]=0;
            reg_to_var.erase(i);
        }
    }
}

int Context::find_free_reg(int begin)
{
    // 检测空闲寄存器
    for (int i = begin; i < reg_count; i++){
        if (used_reg[i] == 0)  return i;
    }
    return -1;
}

int Context::get_new_reg(int begin)
{
    // 检测空闲寄存器
    for (int i = begin; i < reg_count; i++){
        if (used_reg[i] == 0)  return i;
    }
    //没有检测到空闲寄存器，选择一个寄存器溢出
    //选择的是当前寄存器中所有变量最后一次使用时间最晚的那个
    int id=begin;
    for(int i=begin;i<reg_count;i++){
        if( var_lastused_to_time[reg_to_var[i]] >
            var_lastused_to_time[reg_to_var[id]])
        id=i;
    }
    overflow_reg(id);
    used_reg[id]=1;
    return id;
}

void Context::get_specified_reg(int reg_id) 
{
    assert(used_reg[reg_id]==0);
    used_reg[reg_id]=1;
}


void Context::overflow_reg(int id)
{
    if(!used_reg[id])//寄存器id不在使用
        return;
    overflow_var(reg_to_var[id],4);
}

void Context::overflow_var(string name,int bytes)
{
    if(var_to_reg.count(name)){//变量在寄存器中
        int reg_id=var_to_reg[name];
        used_reg[reg_id]=0;
        var_to_reg.erase(name);
        reg_to_var.erase(reg_id);
    }
    stack_offset[name]=stack_occupied[2];
    stack_occupied[2]+=bytes;
}

int Context::get_specified_reg_for(string name, int reg_id) {
    if (var_to_reg.find(name) != var_to_reg.end()) {
        if (var_to_reg[name] == reg_id) {
        return var_to_reg[name];
        }
    }
    if (used_reg[reg_id] == 1) {
        overflow_reg(reg_id);
    }
    get_specified_reg(reg_id);
    bind_var_to_reg(name, reg_id);
    return reg_id;
}

int Context::get_new_reg_for(string name) {
    if (var_to_reg.find(name) != var_to_reg.end()) {
        return var_to_reg[name];
    }
    int reg_id = get_new_reg(0);
    bind_var_to_reg(name, reg_id);
    return reg_id;
}

void Context::bind_var_to_reg(string name, int reg_id) {
    if (name.empty()) {
        throw runtime_error("Var name is empty.");
    }
    assert(used_reg[reg_id] == 1);
    if (reg_to_var.find(reg_id) != reg_to_var.end())
        var_to_reg.erase(reg_to_var[reg_id]);
    reg_to_var.insert({reg_id, name});
    var_to_reg.insert({name, reg_id});
}

string Context::select_var_to_overflow(int begin) {
    string var = "";
    int end = -1;
    for (const auto& i : reg_to_var) {
        if (i.first < begin) continue;
        if (var_lastused_to_time[i.second] > end) {
        var = i.second;
        end = var_lastused_to_time[i.second];
        }
    }
    return var;
}

void Context::load_imm_int(string reg, int value, ostream& out)
{
    if (value >= 0 && value < 65536)
        out << "\tMOV " << reg << ", #" << value << endl;
    else
        out << "\tMOV32 " << reg << ", " << value << endl;
}

void Context::load(string reg, Opn& op, ostream& out)
{
    if(op.is_Imm()){
        if(op.is_int){
            load_imm_int(reg,op.imm_int,out);
        }
        else{

        }
    }
    else if(op.is_Var()){
        if(op.level!=0){//局部变量
            if(var_in_reg(op.name)){
                if(reg.compare("r"+var_to_reg[op.name])==0) return;//变量已经在该寄存器中了
                if(stack_offset.find(op.name)!=stack_offset.end()){//变量正在栈中
                    int offset=stack_offset[op.name];
                    out<<"\tLDR "<<reg<<", "<<"[ r13, #"+to_string(offset)+" ]"<<endl;
                }
                else{//不知道变量在哪
                    throw runtime_error("where is "+op.name);
                }

            }
        }else{//全局变量
            out<<"\tMOVW r11, #:lower16:"<<op.name<<endl;
            out<<"\tMOVT r11, #:upper16:"<<op.name<<endl;
            // out<<"\tMOV32 r11, "<<op.name<<endl;
            out<<"\tLDR r11, [r11, #0]"<<endl;
        }
    }
}

// void Context::store_to_stack(string reg, Opn& op, ostream& out,string op_code = "STR")
// {
//     if(op.level==0){//全局变量

//     }
//     else if(op.is_Var()){

//     }
// }

// void Context::store_to_stack_offset(string reg, int offset, ostream& out,string op = "STR")
// {

// }