#include "IR.hpp"
//@代表全局标识符（函数，全局变量）；
//%代表局部标识符（寄存器名称也就是局部变量，类型）。
void IRBuilder::Build(struct node *T)
{
    genIR(T,symboltable);
}

struct codenode *IRBuilder::codegen(enum op_kind kind, vector<Opn *> &opns)
{
    // struct codenode *p = new struct codenode();
    // p->op = kind;
    // p->opns = opns;
    // p->pre = p->next = p;
    // return p;
}

struct codenode *genLabel(string label)
{
    // struct codenode *p = new struct codenode();
    // p->op = LABEL;
    // Opn *opn = new Opn();
    // opn->id = label;
    // vector<Opn *> opns = {opn};
    // p->opns = opns;
    // p->next = p->pre = p;
    // return p;
}

//合并多个中间代码的双向循环链表，首尾相连
struct codenode *IRBuilder::merge(int num, ...)
{
    struct codenode *h1, *h2, *p, *t1, *t2;
    va_list ap;
    va_start(ap, num);
    h1 = va_arg(ap, struct codenode *);
    while (--num > 0)
    {
        h2 = va_arg(ap, struct codenode *);
        if (h1 == NULL)
            h1 = h2;
        else if (h2)
        {
            t1 = h1->pre;
            t2 = h2->pre;
            t1->next = h2;
            t2->next = h1;
            h1->pre = t2;
            h2->pre = t1;
        }
    }
    va_end(ap);
    return h1;
}



void IRBuilder::genIR(struct node *T,Symboltable &symboltable) {
    // Symboltable symboltable;
    Symbol mysymbol;
    Symbol new_symbol;
    struct node *cur,*T0;
    BasicType* son;
    int record;
    int i;
    int index;
    Symbol *symbol;
    Opn *opn;
    string opn_type;
    vector<Opn*> opns;
        if(T) {
        switch(T->kind) {
        case Root:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            break;
        case CompUnit:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            break;
        case FuncDef:
            //符号入表
            mysymbol.name=string(T->type_id);
            mysymbol.level=T->level;
            mysymbol.pretype=T->pretype;
            mysymbol.types=T->pretype->getvalue();
            mysymbol.paramnum=static_cast<Fun_Type*>(T->pretype)->num_args;
            mysymbol.flag='F';
            index=symboltable.Push(mysymbol)-1;
            
            T->place=index;

            symboltable.Push_index();
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            symboltable.Pop_until(symboltable.Pop_index());
            
           
            break;
        case FuncFParams:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            break;
        case FuncFParam:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            //符号入表
            mysymbol.name=string(T->type_id);
            mysymbol.level=T->level;
            mysymbol.pretype=T->pretype;
            mysymbol.types=T->pretype->getvalue();
            mysymbol.flag='P';
            T->place=symboltable.Push(mysymbol)-1;  
            
            break;
        case FuncFParamArray:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            break;
        case BType:
            break;
        case Block:
            symboltable.Push_index();
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            symboltable.Pop_until(symboltable.Pop_index());
            break;
        case BlockItems:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            break;
        case Decl:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            break;
        case ConstDecl:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            break;
        case VarDecl:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            T->code=merge(2,T->ptr[0]->code,T->ptr[1]->code);
            break;
        case ConstDef:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);

            //符号入表
            mysymbol.name=string(T->ptr[0]->type_id);
            mysymbol.level=T->level;
            mysymbol.flag='V';
            mysymbol.pretype=T->pretype;
            mysymbol.types=T->pretype->getvalue();
            T->place=symboltable.Push(mysymbol)-1;
            break;
        case VarDef:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);

            //符号入表
            mysymbol.name=string(T->ptr[0]->type_id);
            mysymbol.level=T->level;
            mysymbol.flag='V';
            mysymbol.pretype=T->pretype;
            mysymbol.types=T->pretype->getvalue();
            T->place=symboltable.Push(mysymbol)-1;      
            break;
        case Idents:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            break;
        case InitVals:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            break;
        case InitVal:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            break;
        case ASSIGN:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);

            
            break;
        case LVal:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);


            break;
        case Number:

            break;
        case FuncCall:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            break;
        case FuncRParams:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            break;
        case UnaryExp:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            break;
        case AddExp:
        case MulExp:
            break;
        case LOrExp://逻辑或表达式
        case LAndExp://逻辑与表达式
        case EqExp://相等性表达式
        case RelExp://关系表达式
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            break;
        case IF:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            break;
        case WHILE:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            break;
        case RETURN:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            break;
        case CONTINUE:
            
        case BREAK:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            break;
        }
        }
    }
