#ifndef    HEADER_IR
#define    HEADER_IR  
 



#include <string>
#include <stdarg.h>
#include <vector>
#include "../AST/ast.hpp"
using namespace std;
void genIR(struct node *T);
enum opn_kind {Var,Imm,Void};
class Opn//操作数
{
public:
    enum opn_kind kind;                   //Var、Imm、Void
    string type;                //操作数类型，包括 int , float , void

    int     const_int;      //整常数值，立即数
    float   const_float;    //浮点常数值，立即数
    string    id;         //变量或临时变量的别名或标号字符串

    int level;                  //变量的层号，0表示是全局变量，数据保存在静态数据区
    int offset;                 //变量单元偏移量，或函数在符号表的定义位置序号，目标代码生成时用
public:
    Opn(){};
    Opn(enum opn_kind kind,string type,int level,int offset){
        this->kind=kind;
        this->type=type;
        this->level=level;
        this->offset=offset;
    };
    Opn(enum opn_kind kind,string type,int level,int offset,int const_int,float const_float,string id){
        this->kind=kind;
        this->type=type;
        this->level=level;
        this->offset=offset;
        this->const_float=const_float;
        this->const_int=const_int;
        this->id=id;
    };
};

enum op_kind {//指令类型，后面用到再加
    _ADD,
    _SUB,
    _ASSIGN,
    _MUL
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
    int no=0;//用来生成新的temp
    int offset=0;//用来计算变量定义时的offset
public:
    void genIR(struct node *T,Symboltable &symboltable);
    
    struct codenode* codegen(enum op_kind kind,Opn& opn1,Opn& opn2,Opn& result);
    struct codenode *merge(int num,...);

    void Build(struct node *T);
    
    int newtemp(Type *pretype,int level,int offset);
};



#endif   