#ifndef    HEADER_IR
#define    HEADER_IR  
 
#include <string>
#include <stdarg.h>
#include <vector>
#include "../AST/ast.hpp"
#include "../Type/Type.hpp"
using namespace std;

class Opn//操作数
{
public:
    enum opn_kind {
        Var,   
        Imm,   
        Label,  
        Func,   
        Array,  // opn[offset]
        Block,  // {t1,t2,t3,t4,...}，主要是数组赋初值的时候有该操作数
        Null    // 无操作数
    };
    enum opn_kind kind;
    std::string name;

    bool is_int=1;
    int imm_int=-1;
    float imm_float=-2;

    Opn *array_offset;          //当操作数为Array类型时，指向表示偏移量的操作数

    vector<Opn*> Block_args;    //当操作数为Block类型时，指向{}内的各操作数
    int bytes_occupied;              //当操作数为Block类型时，该占用的字节数

    int level;                  //变量的层号，0表示是全局变量，数据保存在静态数据区
    int offset;                 //变量单元偏移量，目标代码生成时用
    int place;                  //变量在符号表中的位置
    Opn():kind(Null),name("-"){}
    Opn(enum opn_kind k,string n):kind(k),name(n) {}

    Opn(enum opn_kind k,int n):kind(k),imm_int(n) {}
    Opn(enum opn_kind k,float n):kind(k),imm_float(n) {}
    Opn(enum opn_kind k,vector<Opn*> m):kind(k),Block_args(m) {}

    Opn(enum opn_kind k,string n,int l,int o,int p):kind(k),name(n),level(l),offset(o),place(p){}
    bool int_float_convert()
    {
        if(kind==Imm){
            if(is_int) imm_float=imm_int;
            else imm_int=imm_float;
            is_int=!is_int;
            return true;
        }
        else if(kind==Var){
            is_int=!is_int;
            return true;
        }else
            return false;
    }
    bool operator==(const Opn& a)
    {
        // if(a.kind!=kind) return false;
        if(kind==Var || kind==Label || kind==Array || kind==Func) return !name.compare(a.name);
        else if(kind==Imm) return (is_int!=a.is_int)?false:((is_int)?imm_int==a.imm_int:imm_float==a.imm_float);
        else return false;
    };
    bool is_Int() {return is_int;}
    bool is_Var() {return kind==Var;}
    bool is_Label() {return kind==Label;}
    bool is_Block() {return kind==Block;}
    bool is_Imm() {return kind==Imm;}
};



class IR//中间代码结点
{
public:
    enum op_kind 
    {//指令类型，后面用到再加
        _VOID,  // 无用指令
        _LABEL, // opn1 :
        _ALLOC, // alloc opn1(变量名) : result(字节数)
        _ADDR,  // result = &opn1
        _ADD,   // result = opn1 + opn2
        _SUB,   // result = opn1 - opn2
        _MUL,   // result = opn1 * opn2
        _DIV,   // result = opn1 / opn2
        _MOD,   // result = opn1 % opn2
        _ASSIGN,// result = opn1
        _Arr_ASSIGN,//opn1[opn2]=result
        _ASSIGN_Arr,  //result=opn1[opn2]
        _NOT,   // result = ! opn1
        _POSI,  // result = + opn1
        _NEGA,  // result = - opn1
        _GOTO,  // goto opn1
        _JEQ,   // if opn1 == opn2 goto result
        _JNE,   // if opn1 != opn2 goto result
        _JLT,   // if opn1 < opn2 goto result
        _JGT,   // if opn1 > opn2 goto result
        _JLE,   // if opn1 <= opn2 goto result
        _JGE,   // if opn1 >= opn2 goto result
        _FUNC,  // define function opn1
        _FUNC_END,//函数结束后的一条指令，没有其他含义
        _PARAM, // param opn1
        _CALL,  // [result =] call opn1(函数) , opn2(参数个数)
        _RET,    // return [opn1]
        _DATA_BEGIN,    //.data(全局变量alloc前一条指令)
        _DATA_END   //全局变量结束后的一条指令，没有其他含义
    };

    enum op_kind op;
    Opn opn1,opn2,result;
    IR():op(_VOID){}
    IR(enum op_kind k):op(k){}
    IR(enum op_kind k,const Opn& o1):op(k),opn1(o1){}
    IR(enum op_kind k,const Opn& o1,const Opn& r):op(k),opn1(o1),result(r){}
    IR(enum op_kind k,const Opn& o1,const Opn& o2,const Opn& r):op(k),opn1(o1),opn2(o2),result(r){}
    void IRprint();
};


class IRBuilder
{
private:
    struct node* root;//AST根节点
    Symboltable symboltable;
    int no=0;//用来生成新的temp
    int offset=0;//用来计算变量定义时的offset
    int label=0;//用来生成新的label
    vector<IR*> IRList;  //中间代码列表
public:
    vector<IR*>& getIRList() {return this->IRList;}
    void genIR(struct node *T,Symboltable &symboltable);
    void Build(struct node *T);
    int newtemp(Type *pretype,int level,int offset);
    string& newLabel();
    void MIRprint();
};
vector<int> PreCal_opn_int(vector<IR*>::iterator begin,vector<IR*>::iterator end,Opn& opn);//在begin和end之间提前计算opn的值

#endif   