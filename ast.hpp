#include <cstdio>
#include "sysy.tab.hh"
#include <cstdlib>
#include <cstring>
#include "Symboltable.hpp"
#include "Type.hpp"
extern char last[50];
extern Symboltable symboltable;
enum node_kind  {   
    Root, CompUnit, Decl, ConstDecl, BType, BlockItems, ConstDef, VarDecl, VarDef, FuncCall,
    InitVal, FuncDef, FuncFParams, FuncFParam, Block, Stmt, Cond, LVal,
    PrimaryExp, Number, InitVals, UnaryExp, UnaryOp, FuncRParams, Idents, FuncFParamArray,
    MulExp, AddExp, RelExp, EqExp, LAndExp, LOrExp
};

struct node {    //以下对结点属性定义没有考虑存储效率，只是简单地列出要用到的一些属性
	enum node_kind kind;               //结点类型
	union {
		  char type_id[32];             //由标识符生成的叶结点
		  int  type_int;                 //由整常数生成的叶结点
		  float type_float;              //由浮点常数生成的叶结点
	      };
    struct node *ptr[3];                   //子树指针，由kind确定有多少棵子树
    int level;                    //层号
    int place;                    //表示结点对应的变量或运算结果临时变量在符号表的位置序号
    char Etrue[15],Efalse[15];      //对布尔表达式的翻译时，真假转移目标的标号
    char Snext[15];               //该结点对应语句执行后的下一条语句位置标号
    struct codenode *code; //该结点中间代码链表头指针
    char op[10];
    Type* pretype;
    int type;                    //结点对应值的类型
    int pos;                     //语法单位所在位置行号
    int offset;                   //偏移量
    int width;                   //各种数据占用的字节数
};

class AST
{
private:
    struct node *root;
public:
    int lev;
    void printAST(struct node *T, int indent, int deep);
    void ASTtoSymtab(struct node *T);   //AST转符号表
    string& getTypeClass(Type pretype); //得到符号类型

    struct node* setroot(struct node *root);
    struct node* getroot() {return root;}
   
};

struct node *mknode(int kind, struct node *first, struct node *second, struct node *third, int pos);
