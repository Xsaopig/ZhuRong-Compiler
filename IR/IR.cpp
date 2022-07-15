#include "IR.hpp"
//@代表全局标识符（函数，全局变量）；
//%代表局部标识符（寄存器名称也就是局部变量，类型）。
void IRBuilder::Build(struct node *T)
{
    genIR(T,symboltable);
}

int IRBuilder::newtemp(Type *pretype,int level,int offset)//不知道就填0
{
    Symbol x;
    x.name="t"+to_string(no++);
    x.flag='T';
    x.level=level;
    x.offset=offset;
    x.paramnum=0;
    x.pretype=pretype;
    if(pretype!=nullptr)
        x.types=pretype->getvalue();
    else
        x.types="unknown";
    int place=symboltable.Push(x)-1;
    return place;
}

struct codenode *IRBuilder::codegen(enum op_kind kind,Opn& opn1,Opn& opn2,Opn& result)
{
    struct codenode *p = new struct codenode();
    p->op = kind;
    p->opn1=opn1;
    p->opn2=opn2;
    p->result=p->result;
    p->pre = p->next = p;
    return p;
}

// struct codenode *genLabel(string label)
// {
//     // struct codenode *p = new struct codenode();
//     // p->op = LABEL;
//     // Opn *opn = new Opn();
//     // opn->id = label;
//     // vector<Opn *> opns = {opn};
//     // p->opns = opns;
//     // p->next = p->pre = p;
//     // return p;
// }

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
    Symbol mysymbol;
    struct node *cur,*T0;
    BasicType* son;
    int record;
    int i;
    int index;
    Symbol *symbol;
    string opn_type;
    Opn *opn1,*opn2,*result;
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
            if(T->pretype->is_BasicType()){
                if(!T->pretype->getvalue().compare("int"))
                    mysymbol.type=INT;
                else if(!T->pretype->getvalue().compare("float"))
                    mysymbol.type=FLOAT;
                else
                    mysymbol.type=VOID;
            }
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
            mysymbol.flag='C';
            mysymbol.pretype=T->pretype;
            mysymbol.types=T->pretype->getvalue();
            if(T->pretype->is_BasicType()){
                if(!T->pretype->getvalue().compare("int"))
                    mysymbol.type=INT;
                else if(!T->pretype->getvalue().compare("float"))
                    mysymbol.type=FLOAT;
                else
                    mysymbol.type=VOID;
            }
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
            if(T->pretype->is_BasicType()){
                if(!T->pretype->getvalue().compare("int"))
                    mysymbol.type=INT;
                else if(!T->pretype->getvalue().compare("float"))
                    mysymbol.type=FLOAT;
                else
                    mysymbol.type=VOID;
            }
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
            if(!T->ptr[0])
            {//LVal: IDENT
                index=symboltable.Search(string(T->type_id));
                T->place=index;
                T->code=nullptr;
                T->offset=-1;//不是数组，不需要offset
            }
            else
            //这一部分得看龙书313页，数组元素寻址的翻译模式
            {//LVal: LVal LB Exp RB
                if(!T->ptr[0]->ptr[0]){         //LVal: (IDENT) LB Exp RB
                    T->offset=newtemp(new BasicType("int"),T->level,0);
                    T->array=symboltable.getSymbol(symboltable.Search(string(T->ptr[0]->type_id)));
                    T->ndim=1;
                    T->width=4;//int和float都占用4个字节
                    //暂时先不链接codenode，直接printf输出
                    symbol=symboltable.getSymbol(T->offset);
                    int limit=static_cast<Array_Type*>(T->array->pretype)->elements_nums[T->ndim];//最外围数组的元素个数
                    if(T->ptr[1]->kind==Number)
                        cout<<symbol->name<<" = "<<T->ptr[1]->type_int<<" * "<<limit<<endl;
                    else 
                        cout<<symbol->name<<" = "<<symboltable.getSymbol(T->ptr[1]->offset)->name<<" * "<<limit<<endl;
                }
                else{                           //LVal: (LVal LB Exp RB) LB Exp RB
                    T->ndim=T->ptr[0]->ndim+1;
                    T->offset=newtemp(new BasicType("int"),T->level,0);
                    T->array=T->ptr[0]->array;
                    //暂时先不链接codenode，直接printf输出
                    symbol=symboltable.getSymbol(T->ptr[0]->offset);
                    if(T->ptr[1]->kind==Number)
                        cout<<symbol->name<<" = "<<symbol->name<<" + "<<T->ptr[1]->type_int<<endl;
                    else
                        cout<<symbol->name<<" = "<<symbol->name<<" + "<<symboltable.getSymbol(T->ptr[1]->offset)->name<<endl;
                    if(T->ndim < static_cast<Array_Type*>(T->array->pretype)->lev)
                    {//没到数组的最后一层
                        int limit=static_cast<Array_Type*>(T->array->pretype)->elements_nums[T->ndim];
                        cout<<symboltable.getSymbol(T->offset)->name<<" = "<<symbol->name<<" * "<<limit<<endl;
                    }
                    else//数组的最后一层
                    {
                        T->place=T->offset; //至此，T->place存储的是数组的偏移量在符号表中的位置
                                            //T->array存储的是数组符号的指针，数组的基址可以用T->array访问
                    }
                }
            }
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
        return;
    }
