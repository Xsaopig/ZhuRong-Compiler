#include "ast.hpp"
int lev=0;
void genIR(struct node *T){
    Symbol mysymbol;
    struct node *cur,*T0;
    BasicType* son;
    int i;
    if(T) {
        switch(T->kind) {
        case Root:
            genIR(T->ptr[0]);
            break;
        case CompUnit:
            genIR(T->ptr[0]);
            genIR(T->ptr[1]);
            break;
        case FuncDef:
            mysymbol.name=string(T->type_id);
            mysymbol.level=lev;
            // if(T->ptr[0]) mysymbol.type=T->ptr[0]->type;//类型有误，VOID未加入，后续调整
            son=static_cast<BasicType*>(T->pretype);
            mysymbol.types=son->getvalue();

            i=0;
            T0=T->ptr[1];
            while(T0){
                i++;
                if(T0->ptr[0]->kind==FuncFParams) T0=T0->ptr[0];
                else T0=NULL;
            }
            mysymbol.paramnum=i;
            mysymbol.flag='F';

            symboltable.Push(mysymbol);  //函数名入表

            symboltable.Push_index();
            genIR(T->ptr[1]);   //进入函数参数
            genIR(T->ptr[2]);
            symboltable.Pop_until(symboltable.Pop_index());
            break;
        case FuncFParams:
            genIR(T->ptr[0]);
            genIR(T->ptr[1]);
            break;
        case FuncFParam:
            

            mysymbol.name=string(T->type_id);
            mysymbol.level=1;
            // mysymbol.type=T->ptr[0]->type;//类型后续调整
            son=static_cast<BasicType*>(T->ptr[0]->pretype);
            mysymbol.types=son->getvalue();
            mysymbol.flag='P';

            if(T->ptr[1]) {
                int i = 1;
                cur = T->ptr[1];
                while(cur->ptr[0]) {
                    i++;
                    cur = cur->ptr[0];
                }

            for (int n = 0; n < i; n++)
                mysymbol.types+=string("[]");//数组参数的类型[]记在types
            }

            symboltable.Push(mysymbol);  //参数名入表

            break;
        case FuncFParamArray:
                
            break;
        case BType:
            
            break;
        case Block:
            lev=lev+1;  
            
            symboltable.Push_index();
            
            
            genIR(T->ptr[0]);
            genIR(T->ptr[1]);
            i=symboltable.Pop_index();
            symboltable.Pop_until(i);   //asd     
            lev=lev-1;
            break;
        case BlockItems:
            genIR(T->ptr[0]);
            genIR(T->ptr[1]);
            break;
        case Decl:
            genIR(T->ptr[0]);
            break;
        case ConstDecl:
            
            if(T->ptr[0]->kind==ConstDecl)  genIR(T->ptr[0]);
            mysymbol.name=string(T->ptr[1]->ptr[0]->type_id);
            mysymbol.level=lev;
            // mysymbol.type=T->type;//类型后续调整
            mysymbol.alias="const ";
            son=static_cast<BasicType*>(T->pretype);
            mysymbol.types=son->getvalue();
            mysymbol.flag='V';
 
            cur=T->ptr[1]->ptr[0];
            i=0;
            while(cur->ptr[0]) {
                i++;
                cur = cur->ptr[0];
            }
            for (int n = 0; n < i; n++)
                   mysymbol.name+=string("[]");//数组的类型[]
            
             symboltable.Push(mysymbol);  //常量（变量）入表
            break;
        case VarDecl:
            if(T->ptr[0]->kind==VarDecl)  genIR(T->ptr[0]);
            mysymbol.name=string(T->ptr[1]->ptr[0]->type_id);
            mysymbol.level=lev;
            // mysymbol.type=T->type;//类型后续调整
            son=static_cast<BasicType*>(T->pretype);
            mysymbol.types=son->getvalue();

            mysymbol.flag='V';
            cur=T->ptr[1]->ptr[0];
            i=0;
            while(cur->ptr[0]) {
                i++;
                cur = cur->ptr[0];
            }
            if(i>0){//变量为数组变量

            }
            for (int n = 0; n < i; n++)
                mysymbol.types+=string("[]");//数组的类型[]记在types
            
             symboltable.Push(mysymbol);  //常量（变量）入表
            break;
        case ConstDef:
        case VarDef:
        
            break;
        case Idents:

            break;
        case InitVals:

            break;
        case InitVal:

            break;
        case ASSIGN:

            genIR(T->ptr[0]);
            genIR(T->ptr[1]);
            break;
        case LVal:
            
            break;
        case Number://单个数值需要处理吗？测试案例有些只有一个整数

            break;
        case FuncCall:
  
            break;
        case FuncRParams:

            break;
        case UnaryExp:

            break;
        case AddExp:
        case MulExp:
        case LOrExp:
        case LAndExp:
        case EqExp:
        case RelExp:

            break;

        case IF:
            genIR(T->ptr[1]);
            genIR(T->ptr[2]);
            break;
        case WHILE:
            genIR(T->ptr[1]);
            break;
        case RETURN:

            break;
        case CONTINUE:

            break;
        case BREAK:

            break;
        }
    }
}