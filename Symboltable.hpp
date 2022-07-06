#include <string>
#include <vector>
#include <stack>
#include "sysy.tab.hh"
using namespace std;

class Symbol
{
public:
    Symbol(string name);
    Symbol(const Symbol &symbol);//复制构造函数
    ~Symbol();
    bool Clone(const Symbol &symbol);//复制函数
public:            
    //这里只列出了一个符号表项的部分属性，没考虑属性间的互斥
    string name;  //变量或函数名
    int level;      //层号，外部变量名或函数名层号为0，形参名为1，每到1个复合语句层号加1，退出减1
    int type;       //变量类型或函数返回值类型
    int paramnum;   //形式参数个数
    string alias; //别名，为解决嵌套层次使用，使得每一个数据名称唯一
    char flag;  //符号标记，函数：'F'  变量：'V'   参数：'P'  临时变量：'T'
    char offset;    //外部变量和局部变量在其静态数据区或活动记录中的偏移量
                    //或函数活动记录大小，目标代码生成时使用
    //其它...
    //添加属性的时候别忘了修改 复制构造函数 与 复制函数Clone()！

};

class Symboltable   //符号表，是一个顺序栈，index初值为0
{
public:
    Symboltable();
    ~Symboltable();
private:
    vector<Symbol> symbols; //符号表
    int index;  //符号表索引
    stack<int> scope_begin;  //符号表作用域辅助栈
public:
    Symbol* getSymbol(int index);  //根据index得到对应符号的引用,没有则返回空指针
    int Push(Symbol& symbol);  //插入符号到栈顶，返回当前栈中已有的符号数
    int Pop();     //删除栈顶符号，返回当前栈中剩余符号数
    int Pop_until(int index);   //从删除符号符号，直到剩余符号数=index
    int Search(string name);    //根据name查找符号,返回index
    bool Replace(int index,Symbol& symbol);    //替换索引为index的符号

    int Push_index() {scope_begin.push(index);return index;};//把当前index压入作用域辅助栈scope_begin中
    int Pop_index() {//从作用域辅助栈scope_begin弹出上一个index
        int temp=scope_begin.top();
        scope_begin.pop();
        return temp;
    };

};