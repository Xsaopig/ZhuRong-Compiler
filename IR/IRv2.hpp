#ifndef    HEADER_IR
#define    HEADER_IR  
 



#include <string>
#include <stdarg.h>
#include <vector>
#include "../AST/ast.hpp"
using namespace std;
void genIR(struct node *T);
class Opn//操作数
{
public:
    int kind;                   //标识操作的类型,如IDENT、
    string type;                //操作数类型，包括 iN , float , void
                                //在LLVM IR中，布尔型使用i1表示，int使用i32表示
    union {
        int     const_int;      //整常数值，立即数
        float   const_float;    //浮点常数值，立即数
        string    id;         //变量或临时变量的别名或标号字符串
    };
    int level;                  //变量的层号，0表示是全局变量，数据保存在静态数据区
    int offset;                 //变量单元偏移量，或函数在符号表的定义位置序号，目标代码生成时用
public:
    Opn(){};
    Opn(int kind,string type,int level,int offset){
        this->kind=kind;
        this->type=type;
        this->level=level;
        this->offset=offset;
    };
};

enum op_kind {//LLVM IR指令类型，后面用到再加
    VAR,  //变量
    CONSTANT,  //常量
    LABEL,//特殊，标号，实际上没有这一条指令
    DEFINE,//函数定义 
    ALLOCA,//从栈中分配空间
    STORE,//存储到栈中
    LOAD,//从栈中取
    ICMP,//比较
    BR,//跳转
    PHI,//根据分支赋值
    RET,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    NEG,
    AUTOADD,
    AUTOSUB,
    FUNCTION,
    
    ERROR
};

struct codenode//中间代码结点
{
    enum op_kind op;
    Opn* result;       //部分指令有返回值，如: %7 = icmp sgt i32 %6, 1
    vector<Opn*> opns;//LLVM IR有的指令的操作数数量是不确定的，如phi指令
    struct codenode *pre,*next;//双向链表
};

class IRBuilder
{
private:
    struct node* root;//AST根节点
    int no=0;//用来生成新的alias
    int no_gloabl=0;
public:
    void genIR(struct node *T);
    string newAlias(){ return string("%")+to_string(no++);};
    string newGloabl(){ return string("@")+to_string(no_gloabl++);};
    string newlabel(){ return string("label %")+to_string(no++);};

    struct codenode* codegen(enum op_kind kind,vector<Opn*>& opns);
    struct codenode *merge(int num,...);

    void Build(struct node *T);
    
};



#endif   