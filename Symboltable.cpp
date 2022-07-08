#include "Symboltable.hpp"
#include <iostream>

/*  Symbol类    */

Symbol::Symbol(string name)
{
    this->name=name;
    level=0;
    flag='V';
    type=INT;
    type="int";
    paramnum=0;
    offset=0x00;
}
Symbol::Symbol(const Symbol &symbol)
{
    this->name=symbol.name;
    this->alias=symbol.alias;
    this->flag=symbol.flag;
    this->type=symbol.type;
     this->types=symbol.types;
    this->paramnum=symbol.paramnum;
    this->offset=symbol.offset;
    this->level=symbol.level;
}
Symbol::~Symbol() {}
bool Symbol::Clone(const Symbol &symbol)//复制函数
{
    this->name=symbol.name;
    this->alias=symbol.alias;
    this->flag=symbol.flag;
    this->type=symbol.type;
     this->types=symbol.types;
    this->paramnum=symbol.paramnum;
    this->offset=symbol.offset;
    this->level=symbol.level;
    return true;
}

/*  Symboltable类   */

Symboltable::Symboltable()
{
    this->index=0;
}

Symboltable::~Symboltable() {}

//根据index得到对应符号的引用,没有则返回空指针
Symbol* Symboltable::getSymbol(int index)
{
    if(this->index<index || index<0)
        return nullptr;
    else return &symbols[index];
}

//插入符号，返回当前栈中已有的符号数
int Symboltable::Push(Symbol& symbol)
{
    symbols.push_back(symbol);
    index++;
    print();
    return symbols.size();
}

//删除符号，返回当前栈中已有的符号数
int Symboltable::Pop() 
{
    if(symbols.size()<=0) exit(1);  
    symbols.pop_back();
    index--;
    print();
    return symbols.size();
}

//从删除符号符号，直到剩余符号数=index
int Symboltable::Pop_until(int index)  
{
    if(index==this->index) return this->index;
    while(Pop()!=index);
    return this->index;

    /*int prelevel=getSymbol(index).level-1;
    while(getSymbol(Pop()).level!=prelevel);
    return index;*/
}

//根据name查找符号,返回index
int Symboltable::Search(string name)   
{
    for(int i=0;i<=index;i++)
        if(!name.compare(symbols[i].name)) return i;
    return -1;
}

//替换索引为index的符号
bool Symboltable::Replace(int index,Symbol& newsymbol)    
{
    return symbols[index].Clone(newsymbol);
}
string name;  //变量或函数名
int level;      //层号，外部变量名或函数名层号为0，形参名为1，每到1个复合语句层号加1，退出减1
int type;       //变量类型或函数返回值类型
int paramnum;   //形式参数个数
string alias; //别名，为解决嵌套层次使用，使得每一个数据名称唯一
char flag;  //符号标记，函数：'F'  变量：'V'   参数：'P'  临时变量：'T'
char offset;    //外部变量和局部变量在其静态数据区或活动记录中的偏移量

void Symboltable::print()//打印符号表
{
    printf("index\tname\talias\tlevel\ttype\tflag\tparamnum\toffset\t\n");
    for(int i=0;i<index;i++){
        printf("%d\t",i);
        printf("%s\t",symbols[i].name.c_str());
        printf("%s\t",symbols[i].alias.c_str());
        printf("%d\t",symbols[i].level);
        printf("%d\t",symbols[i].type);
        printf("%s\t",symbols[i].types);
        printf("%c\t",symbols[i].flag);
        printf("%d\t",symbols[i].paramnum);
        printf("%c\t",symbols[i].offset);
        printf("\n");
    }
}