#include "arm.h"
void generate_arm(vector<IR*> IRList,std::ostream& out)
{
    out<<"\t.arch armv7"<<endl;
    out<<"\t.file	\""<<filename<<"\""<<endl;
    vector<IR*>::iterator func_begin,data_begin;
    for(auto iter=IRList.begin();iter!=IRList.end();iter++){
        auto &ir=*iter;
        if(ir->op==IR::_DATA_BEGIN){
            data_begin=iter;
        } else if(ir->op==IR::_DATA_END){
            generate_data_arm(IRList,data_begin,iter,out);//生成全局变量的汇编代码
        } else if(ir->op==IR::_FUNC){
            func_begin=iter;
        } else if(ir->op==IR::_RET){
            generate_function_arm(IRList,func_begin,iter,out);//生成函数内的汇编代码
        }

    }
}

void generate_data_arm(vector<IR*> IRList,vector<IR*>::iterator data_begin,vector<IR*>::iterator data_end,std::ostream& out)
{
    out<<endl;
    int size;//符号的字节数
    string name;//符号名
    auto iter=data_begin;
    for(;iter!=data_end;iter++)//找到ALLOC语句
        if((*iter)->op==IR::_ALLOC) break;
    name=(*iter)->opn1.name;
    size=(*iter)->result.imm_int;
    out<<"\t.global "<<name<<endl;
    out<<"\t.data"<<endl;//好像sysy要求没有显示初始化的全局变量，要初始化为0，所以放在.data区
    out<<"\t.size "<<name<<", "<<size<<endl;
    //接下来是赋值,下例中word就是字(32位，4个字节),“.word”代表这个字的值,.space代表未被赋值的字节数()
    // not:
	// .word	0
	// .word	1073741824
	// .word	1077936128
	// .space	4
    out<<name<<":"<<endl;
    
}

void generate_function_arm(vector<IR*> IRList,vector<IR*>::iterator function_begin,vector<IR*>::iterator function_end,std::ostream& out)
{

}