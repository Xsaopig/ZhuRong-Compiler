#include <string>
#include <vector>
using namespace std;
void genIR(struct node *T);
class Opn//操作数
{
    int kind;                   //标识操作的类型（暂时不知道啥用）
    string type;                //操作数类型，包括 iN , float , void
                                //在LLVM IR中，布尔型使用i1表示，int使用i32表示
    union {
        int     const_int;      //整常数值，立即数
        float   const_float;    //浮点常数值，立即数
        char    id[32];         //变量或临时变量的别名或标号字符串
    };
    int level;                  //变量的层号，0表示是全局变量，数据保存在静态数据区
    int offset;                 //变量单元偏移量，或函数在符号表的定义位置序号，目标代码生成时用


};

enum op_kind {//LLVM IR指令类型，后面用到再加
    ALLOCA,STORE,LOAD,ICMP,BR,PHI
};

struct codenode//中间代码结点
{
    enum op_kind op;
    vector<Opn> opns;//LLVM IR有的指令的操作数数量是不确定的，如phi指令
    struct codenode *pre,*next;//双向链表
};


