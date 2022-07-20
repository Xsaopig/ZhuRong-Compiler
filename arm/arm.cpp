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
        } else if(ir->op==IR::_FUNC_END){
            generate_function_arm(IRList,func_begin,iter,out);//生成函数内的汇编代码
        }

    }
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

string out_opn(vector<IR*> IRList,vector<IR*>::iterator begin,vector<IR*>::iterator end,Opn& opn){
    string out;
    if(opn.kind==Opn::Imm){
        if(opn.is_int)
            out="\t.word "+to_string(opn.imm_int)+"\n";
        else{
            string hex_float;
            unsigned char* cc = (unsigned char*)&opn.imm_float;
            char temp_str[3];
            for (int j = 3; j >=0 ; j--){
                sprintf(temp_str,"%02x",*(cc + j));
                hex_float=hex_float+string(temp_str);
            }
            out="\t.word "+to_string(stoi(hex_float,0,16))+"\n";
        }
        return out;
    }
    auto iter=end;
    auto ptr=(*iter);
    for(;iter!=begin;iter--){
        ptr=(*iter);
        if(ptr->result==opn) break; 
    }
    if(ptr->op==IR::_ASSIGN) {
        if(ptr->opn1.kind==Opn::Imm){
            if(ptr->opn1.is_int)
                out="\t.word "+to_string(ptr->opn1.imm_int)+"\n";
            else{
                string hex_float;
                unsigned char* cc = (unsigned char*)&ptr->opn1.imm_float;
                char temp_str[3];
                for (int j = 3; j >=0 ; j--){
                    sprintf(temp_str,"%02x",*(cc + j));
                    hex_float=hex_float+string(temp_str);
                }
                out="\t.word "+to_string(stoi(hex_float,0,16))+"\n";
            }
        } 
        else if(ptr->opn1.kind==Opn::Block){
            for(int i=0;i<ptr->opn1.Block_args.size();i++){
                out=out+out_opn(IRList,IRList.begin(),end,*ptr->opn1.Block_args[i]);
            }
        }
        else if(ptr->opn1.kind==Opn::Var)
            out=out_opn(IRList,IRList.begin(),end,ptr->opn1);
    }
    else if(ptr->op==IR::_ASSIGN_Arr){
        out=out_opn(IRList,IRList.begin(),end,ptr->opn1);
        string int_str=out_opn(IRList,IRList.begin(),end,ptr->opn2);
        int_str.erase(int_str.begin(),int_str.begin()+7);
        int_str.erase(int_str.find("\n"),1);
        int offset=atoi(int_str.c_str());
        for(int i=0;i<offset;i++){
            int j=out.find("\n");
            out=out.substr(j+1,out.size()-j-1);
        }
        int j=out.find("\n");
        out=out.substr(0,j+1);
        
    }
    else if(ptr->op==IR::_ADDR){
        out=out_opn(IRList,IRList.begin(),end,ptr->opn1);
    }
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
    size=(*iter)->result.imm_int;
    is_int=(*iter)->opn1.is_int;
    out<<"\t.global "<<name<<endl;
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
    auto ptr=find_opn_ASSIGN(data_begin,data_end,(*iter)->opn1);
    if (ptr==nullptr){//没有初始化的全局变量
        for(int i=0;i<size/4;i++){
            out<<"\t.word 0"<<endl;
        }
    }else{
        out<<out_opn(IRList,data_begin,data_end,ptr->opn1)<<endl;
    }
    
}

void generate_function_arm(vector<IR*> IRList,vector<IR*>::iterator function_begin,vector<IR*>::iterator function_end,std::ostream& out)
{

}