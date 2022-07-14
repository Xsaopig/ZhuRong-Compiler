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
    int kind;                   //VAR、IMM、NULL
    string type;                //操作数类型，包括 int , float , void

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

enum op_kind {//指令类型，后面用到再加

};

struct codenode//中间代码结点
{
    enum op_kind op;
    Opn opn1,opn2,result;
    struct codenode *pre,*next;//双向链表
};

class IRBuilder
{
private:
    struct node* root;//AST根节点
    Symboltable symboltable;
    int no=0;//用来生成新的alias
    int no_gloabl=0;
public:
    void genIR(struct node *T,Symboltable &symboltable);
    string newAlias(){ return string("%")+to_string(no++);};
    string newGloabl(){ return string("@")+to_string(no_gloabl++);};
    string newlabel(){ return string("label %")+to_string(no++);};

    struct codenode* codegen(enum op_kind kind,vector<Opn*>& opns);
    struct codenode *merge(int num,...);

    void Build(struct node *T);
    
};



#endif   