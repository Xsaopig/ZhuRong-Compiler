#include "Symboltable.hpp"
#include <iostream>

/*  Symbol类    */

Symbol::Symbol(string name)
{
    this->name=name;
    level=0;
    flag='V';
    type=INT;
    paramnum=0;
    offset=0x00;
}
Symbol::Symbol(const Symbol &symbol)
{
    this->name=symbol.name;
    this->alias=symbol.alias;
    this->flag=symbol.flag;
    this->type=symbol.type;
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
    return symbols.size();
}

//删除符号，返回当前栈中已有的符号数
int Symboltable::Pop() 
{
    symbols.pop_back();
    index--;
    return symbols.size();
}

//从删除符号符号，直到剩余符号数=index
int Symboltable::Pop_until(int index)  
{
    while(Pop()!=index);
    return index;

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