#include "arm.h"
void generate_arm(vector<IR*> IRList,std::ostream& out)
{
    out<<"\t.arch armv7"<<endl;
    out<<"\t.file \""<<filename<<"\""<<endl;
    out<<"\t.text"<<endl;
    vector<IR*>::iterator func_begin,data_begin;
    for(auto iter=IRList.begin();iter!=IRList.end();iter++){
        auto &ir=*iter;
        if(ir->op==IR::_DATA_BEGIN){
            data_begin=iter;
        } else if(ir->op==IR::_DATA_END){
            generate_data_arm(IRList,data_begin,iter,out);//生成全局变量的汇编代码
        } else if(ir->op==IR::_FUNC){
            func_begin=iter;
        } else if(ir->op==IR::_FUNC_END){
            generate_function_arm(IRList,func_begin,iter,out);//生成函数内的汇编代码
        }

    }
}


void generate_function_arm(vector<IR*> IRList,vector<IR*>::iterator function_begin,vector<IR*>::iterator function_end,std::ostream& out)
{
    Context ctx(IRList,function_begin,function_end);

    //初始化每条指令对应的时间，从begin到end依次是1,2,3......
    //即给每条指令编号
    for (auto it = function_begin; it != function_end; it++) {
        ctx.init_ir_to_time(*it);
    }

    //初始化每个变量定义的时间
    for (auto it = function_begin; it != function_end; it++) {
        ctx.init_var_define_to_time(*it);
    }

    //初始化每个变量最后使用的时间
    for (auto it = function_begin; it != function_end; it++) {
        ctx.init_var_lastused_to_time(*it);
    }

    //在栈中分配空间
    for (auto it = function_begin; it != function_end; it++) {
        auto &ir=*it;
        if(ir->op==IR::_ALLOC && ir->result.is_Imm() /* && ir->result.imm_int>4 */){
            ctx.stack_offset[ir->opn1.name]=ctx.stack_occupied[2];
            ctx.stack_occupied[2] += ir->result.imm_int;
        }
    }

    //寄存器分配
    for(const auto & i :ctx.time_to_var_define)//按Var定义的时间从小到大遍历
    {
        int cur_time=i.first;
        auto var_name=i.second;
        ctx.clear_inactive_intervals(cur_time);//清楚非活跃区间

        if(ctx.var_in_reg(var_name)){//变量如果已经在寄存器中了？？
            continue;
        } else {
            bool conflict = false;
            int latest=ctx.var_lastused_to_time[var_name];//当前变量的最后一次使用时间
            // for(auto &j :ctx.time_to_var_define){//需要遍历的是：定义时间<=当前时间 且>latest 之间的变量
            //     if(j.first<=cur_time) continue;
            //     if(j.first>latest) break;
            // }
            if (ctx.find_free_reg(0) != -1) {
                ctx.get_specified_reg_for(var_name,
                                            ctx.find_free_reg(0));
            } else {
                string cur_max = ctx.select_var_to_overflow(0);
            if (ctx.var_lastused_to_time[var_name] <
                ctx.var_lastused_to_time[cur_max]) {
                ctx.get_specified_reg_for(var_name, ctx.var_to_reg[cur_max]);
            } else {
                ctx.overflow_var(var_name,4);
            }
            }
        }

    }



    string funcname;//函数名
//开始逐条翻译指令
for (auto it = function_begin; it != function_end; it++) {
    auto& ir = *it;
    auto& stack_size=ctx.stack_occupied;

    if(ir->op==IR::_FUNC){// define function opn1
        funcname=(*function_begin)->opn1.name;
        out<<"\t.text"<<endl;
        out<<"\t.global "<<funcname<<endl;
        out<<"\t.arch armv7"<<endl;
        out<<"\t.syntax unified"<<endl;
        out<<"\t.thumb"<<endl;
        out<<"\t.thumb_func"<<endl;
        out<<"\t.fpu vfp"<<endl;
        out<<"\t.type "<<funcname<<", %function"<<endl;
        out<<funcname<<":"<<endl;

        //保护现场,目前不考虑函数调用，就保存这两个
        out<<"\tPUSH { r12, lr }"<<endl;
        if(stack_size[0]+stack_size[1]+stack_size[2]+stack_size[3]>256){
            ctx.load_imm_int("r12",stack_size[0]+stack_size[1]+stack_size[2]+stack_size[3],out);
            out<<"\tSUB sp, sp, r12"<<endl;
        }else {
            out<<"\tSUB sp, sp, #"<<stack_size[0]+stack_size[1]+stack_size[2]+stack_size[3]<<endl;
        }
        out<<"\tADD r12, sp, #0"<<endl;
        
    }else if(ir->op==IR::_VOID){// 无用指令
        //do nothing
    }else if(ir->op==IR::_ALLOC){// alloc opn1(变量名) : result(字节数)
        if(ir->opn1.is_Var() && ctx.var_in_reg(ir->opn1.name)){
            ctx.load("r"+to_string(ctx.var_to_reg[ir->opn1.name]),ir->opn1,out);
        }
        //do nothing
    }else if(ir->op==IR::_LABEL){// opn1 :
        out<<"\t"<<ir->opn1.name<<":"<<endl;
    }else if(ir->op==IR::_ADDR){// result = &opn1
        
    }else if(ir->op==IR::_ADD){// result = opn1 + opn2
        bool result_in_reg = ctx.var_in_reg(ir->result.name);
        bool op1_in_reg = ir->opn1.is_Var() && ctx.var_in_reg(ir->opn1.name);
        bool op2_in_reg = ir->opn2.is_Var() && ctx.var_in_reg(ir->opn2.name);
        int reg1 = op1_in_reg ? ctx.var_to_reg[ir->opn1.name] : 11;
        int reg2 = op2_in_reg ? ctx.var_to_reg[ir->opn2.name] : 14;
        int reg3 = result_in_reg ? ctx.var_to_reg[ir->result.name] : 11;
        if(!op1_in_reg){
            ctx.load("r"+to_string(reg1),ir->opn1,out);
        }
        if(!op2_in_reg){
            ctx.load("r"+to_string(reg2),ir->opn2,out);
        }
        out<<"\tADD "<<"r"+to_string(reg3)<<", "
                        <<"r"+to_string(reg1)<<", "<<"r"+to_string(reg2)<<endl;
        if(!result_in_reg){
            //存回栈中
        }

    }else if(ir->op==IR::_SUB){// result = opn1 - opn2
        bool result_in_reg = ctx.var_in_reg(ir->result.name);
        bool op1_in_reg = ir->opn1.is_Var() && ctx.var_in_reg(ir->opn1.name);
        bool op2_in_reg = ir->opn2.is_Var() && ctx.var_in_reg(ir->opn2.name);
        int reg1 = op1_in_reg ? ctx.var_to_reg[ir->opn1.name] : 11;
        int reg2 = op2_in_reg ? ctx.var_to_reg[ir->opn2.name] : 14;
        int reg3 = result_in_reg ? ctx.var_to_reg[ir->result.name] : 11;
        if(!op1_in_reg){
            ctx.load("r"+to_string(reg1),ir->opn1,out);
        }
        if(!op2_in_reg){
            ctx.load("r"+to_string(reg2),ir->opn2,out);
        }
        out<<"\tSUB "<<"r"+to_string(reg3)<<", "
                        <<"r"+to_string(reg1)<<", "<<"r"+to_string(reg2)<<endl;
        if(!result_in_reg){
            //存回栈中
        }
        
    }else if(ir->op==IR::_MUL){// result = opn1 * opn2
        
    }else if(ir->op==IR::_DIV){// result = opn1 / opn2
        
    }else if(ir->op==IR::_MOD){// result = opn1 % opn2
        
    }else if(ir->op==IR::_ASSIGN){// result = opn1
        bool result_in_reg = ctx.var_in_reg(ir->result.name);
        bool op1_in_reg = ir->opn1.is_Var() && ctx.var_in_reg(ir->opn1.name);
        int reg1 = op1_in_reg ? ctx.var_to_reg[ir->opn1.name] : 11;
        int reg3 = result_in_reg ? ctx.var_to_reg[ir->result.name] : 11;
        if(!op1_in_reg){
            ctx.load("r"+to_string(reg1),ir->opn1,out);
        }
        out<<"\tMOV "<<"r"+to_string(reg3)<<", "
                        <<"r"+to_string(reg1)<<endl;
        if(!result_in_reg){
            //存回栈中
        }
        
    }else if(ir->op==IR::_Arr_ASSIGN){//opn1[opn2]=result
        
    }else if(ir->op==IR::_ASSIGN_Arr){//result=opn1[opn2]
        
    }else if(ir->op==IR::_NOT){// result = ! opn1
        
    }else if(ir->op==IR::_POSI){// result = + opn1
        
    }else if(ir->op==IR::_NEGA){// result = - opn1
        
    }else if(ir->op==IR::_GOTO){// goto opn1
        
    }else if(ir->op==IR::_JEQ){// if opn1 == opn2 goto result
        
    }else if(ir->op==IR::_JNE){// if opn1 != opn2 goto result
        
    }else if(ir->op==IR::_JLT){// if opn1 < opn2 goto result
        
    }else if(ir->op==IR::_JGT){// if opn1 > opn2 goto result
        
    }else if(ir->op==IR::_JLE){// if opn1 <= opn2 goto result
        
    }else if(ir->op==IR::_JGE){// if opn1 >= opn2 goto result
        
    }else if(ir->op==IR:: _FUNC_END){//函数结束后的一条指令，没有其他含义
        
    }else if(ir->op==IR::_PARAM){// param opn1
        
    }else if(ir->op==IR::_CALL){// [result =] call opn1(函数) , opn2(参数个数)
        
    }else if(ir->op==IR::_RET){// return [opn1]
        if(ir->opn1.is_Imm()){
            if(ir->opn1.is_int)
                out<<"\t"<<"MOV r0, #"<<ir->opn1.imm_int<<endl;
        }
        else if(ir->opn1.is_Var()){
            if(ctx.var_in_reg(ir->opn1.name) && ctx.var_to_reg[ir->opn1.name]!=0)
                out<<"\t"<<"MOV r0, r"<<ctx.var_to_reg[ir->opn1.name]<<endl;
        }
        out<<"\tADDS r12, r12, #"<<stack_size[0]+stack_size[1]+stack_size[2]+stack_size[3]<<endl;
        out<<"\tMOV sp, r12"<<endl;
        out<<"\tPOP { r12, lr }"<<endl;
        out<<"\tBX lr"<<endl;  
    }
        
}
    out<<"\t.size "<<funcname<<", .-"<<funcname<<endl;
    out<<endl;
}


IR* find_opn_ASSIGN(vector<IR*>::iterator data_begin,vector<IR*>::iterator data_end,Opn& opn)
{
    auto iter=data_end;
    for(;iter!=data_begin;iter--){
        auto ptr=(*iter);
        if(ptr->result==opn) return ptr;
    }
    return nullptr;
}

string floattostring(float x){
    string out;
    string hex_float;
    unsigned char* cc = (unsigned char*)&x;
    char temp_str[3];
    for (int j = 3; j >=0 ; j--){
        sprintf(temp_str,"%02x",*(cc + j));
        hex_float=hex_float+string(temp_str);
    }
    out=to_string(stoi(hex_float,0,16));
    return out;
}

void generate_data_arm(vector<IR*> IRList,vector<IR*>::iterator data_begin,vector<IR*>::iterator data_end,std::ostream& out)
{
    if(data_begin+1==data_end)
        return;
    out<<endl;
    int size;//符号的字节数
    string name;//符号名
    bool is_int;//是否为int类型
    auto iter=data_begin;
    for(;iter!=data_end;iter++)//找到ALLOC语句
        if((*iter)->op==IR::_ALLOC) break;
    name=(*iter)->opn1.name;
    if((*iter)->result.kind==Opn::Imm)
        size=(*iter)->result.imm_int;
    else{
        string buffer=out_opn(IRList,IRList.begin(),data_end,(*iter)->result);
        int j=buffer.find("\n");
        buffer=buffer.substr(j+1,buffer.size()-j-1);
        j=buffer.find("\n");
        buffer=buffer.substr(0,j+1);
    }
    is_int=(*iter)->opn1.is_int;
    out<<"\t.global "<<name<<endl;
    out<<"\t.align 2"<<endl;
    out<<"\t.type "<<name<<",\%object"<<endl;
    out<<"\t.data"<<endl;//好像sysy要求没有显示初始化的全局变量，要初始化为0，所以放在.data区
    out<<"\t.size "<<name<<", "<<size<<endl;
    //接下来是赋值,下例中word就是字(32位，4个字节),“.word”代表这个字的值,.space代表未被赋值的字节数()
    // not:
	// .word	0
	// .word	1073741824
	// .word	1077936128
	// .space	4
    out<<name<<":"<<endl;
    // out<<out_opn(IRList,data_begin,data_end,(*iter)->opn1)<<endl;
    auto ptr=find_opn_ASSIGN(data_begin,data_end,(*iter)->opn1);
    if (ptr==nullptr || (ptr->opn1.kind==Opn::Block && ptr->opn1.Block_args.size()==0) ){//没有初始化的全局变量
        out<<"\t.space "<<size<<endl;
    }else{
        string buffer=out_opn(IRList,data_begin,data_end,ptr->opn1);
        int n=count(buffer.begin(),buffer.end(),'\n');
        if(n>size/4){
            while(n*4>size){
                int j=buffer.find_last_of("\t");
                buffer=buffer.substr(0,j);
                n--;
            }
        }
        else if(n<size/4){
            buffer+="\t.space "+to_string(size-4*n)+"\n";
        }
        out<<buffer<<endl;
    }
    
}

string out_opn(vector<IR*> IRList,vector<IR*>::iterator begin,vector<IR*>::iterator end,Opn& opn){
    string out;
    if(opn.kind==Opn::Imm){
        if(opn.is_int)
            out="\t.word "+to_string(opn.imm_int)+"\n";
        else
            out="\t.word "+floattostring(opn.imm_float)+"\n";
        return out;
    }
    auto iter=end;
    auto ptr=(*iter);
    for(;iter!=begin;iter--){
        ptr=(*iter);
        if(ptr->result==opn && ptr->op!=IR::_ALLOC) break; 
    }
    int j,offset;
    string int_str;
    vector<int> int_ret;
    vector<float> float_ret;
    switch(ptr->op)
    {
    case IR::_ASSIGN:
        if(ptr->opn1.kind==Opn::Imm){
            if(ptr->opn1.is_int)
                out="\t.word "+to_string(ptr->opn1.imm_int)+"\n";
            else
                out="\t.word "+floattostring(opn.imm_float)+"\n";
        } 
        else if(ptr->opn1.kind==Opn::Block){
            for(int i=0;i<ptr->opn1.Block_args.size();i++){
                if(ptr->opn1.Block_args[i]->is_int){
                    int_ret=PreCal_opn_int(IRList.begin(),end,*ptr->opn1.Block_args[i]);
                    for(auto x:int_ret)
                        out=out+"\t.word "+to_string(x)+"\n";
                }
                else{
                    float_ret=PreCal_opn_float(IRList.begin(),end,*ptr->opn1.Block_args[i]);
                    for(auto x:float_ret){
                        out=out+"\t.word "+floattostring(x)+"\n";
                    }
                }
            }
        }
        else if(ptr->opn1.kind==Opn::Var)
            out=out_opn(IRList,IRList.begin(),end,ptr->opn1);
        break;
    case IR::_ASSIGN_Arr:
        out=out_opn(IRList,IRList.begin(),end,ptr->opn1);
        int_str=out_opn(IRList,IRList.begin(),end,ptr->opn2);
        int_str.erase(int_str.begin(),int_str.begin()+7);
        int_str.erase(int_str.find("\n"),1);
        offset=atoi(int_str.c_str());
        for(int i=0;i<offset;i++){
            j=out.find("\n");
            out=out.substr(j+1,out.size()-j-1);
        }
        j=out.find("\n");
        out=out.substr(0,j+1);
        if(out.empty())
            out="\t.word 0\n";
        break;
    case IR::_ADDR:
        out=out_opn(IRList,IRList.begin(),end,ptr->opn1);
        break;
    case IR::_NOT:
        if(ptr->opn1.is_int)
        break;
    default:
        break;
    }
    return out;
}