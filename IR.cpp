#include "ast.hpp"
string IRBuilder::newAlias(){
    return string("%")+to_string(no++);
}
string IRBuilder::newlabel(){//注意在LLVM IR中，标号label跟局部变量一样用%N表示
    return string("label %")+to_string(no++);
}

struct codenode* IRBuilder::makecodenode(enum op_kind kind,vector<Opn*>& opns){
    struct codenode *p=new struct codenode();
    p->op=kind;
    p->opns=opns;
    p->pre=p->next=p;
    return p;
}

struct codenode *genLabel(string label){
    struct codenode *p=new struct codenode();
    p->op=LABEL;
    Opn *opn=new Opn();
    opn->id=label;
    vector<Opn*> opns={opn};
    p->opns=opns;
    p->next=p->pre=p;
    return p;
}

//合并多个中间代码的双向循环链表，首尾相连
struct codenode *merge(int num,...){
    struct codenode *h1,*h2,*p,*t1,*t2;
    va_list ap;
    va_start(ap,num);
    h1=va_arg(ap,struct codenode *);
    while (--num>0) {
        h2=va_arg(ap,struct codenode *);
        if (h1==NULL) h1=h2;
        else if (h2){
            t1=h1->pre;
            t2=h2->pre;
            t1->next=h2;
            t2->next=h1;
            h1->pre=t2;
            h2->pre=t1;
            }
        }
    va_end(ap);
    return h1;
}


void IRBuilder::genIR(struct node *T) {
    Symbol mysymbol;
    struct node *cur,*T0;
    BasicType* son;
    int i;
    int index;
    Symbol *symbol;
    Opn *opn;
    string opn_type;
    vector<Opn*> opns;
    if(T) {
        switch(T->kind) {
        case Root:
            T->level=0;//根节点在第0层
            genIR(T->ptr[0]);
            break;
        case CompUnit:
            T->level=0; //CompUnit结点的子节点也在第0层
            T->ptr[0]->level=0;
            if(T->ptr[1]) T->ptr[1]->level=0;
            genIR(T->ptr[0]);
            genIR(T->ptr[1]);
            break;
        case FuncDef:
            mysymbol.name=string(T->type_id);
            mysymbol.level=T->level;
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
            T->place=symboltable.Push(mysymbol)-1;  //函数名入表
            symboltable.Push_index();

            //接下来处理T->code
            //在LLVM IR中，函数定义形式如：define i32 @main   
            //i32代表int返回值类型，@在LLVM IR中是全局符号的意思
            opn_type= (!mysymbol.types.compare("int"))?"i32":mysymbol.types;//int对应的是i32，float和void不变
            opn=new Opn(IDENT,opn_type,T->level,T->place);//两个类型、层号、在符号表中的位置
            opn->id=mysymbol.name;//操作数名就是函数名
            opns.push_back(opn);//函数定义操作只有一个操作数
            T->code=makecodenode(DEFINE,opns);
            

            //计算子节点的level（继承属性）
            if(T->ptr[0]) T->ptr[0]->level=0;//函数类型结点还在第0层
            if(T->ptr[1]) T->ptr[1]->level=1;   //参数在第1层
            T->ptr[2]->level=1;   //函数体在第1层
            
            
            genIR(T->ptr[1]);   //进入函数参数
            opn_type=newlabel();            //这一句和下面一句不要合成一句，会报错（奇怪）
            T->ptr[2]->Snext=opn_type;      //T->Snext
            genIR(T->ptr[2]);   //函数体

            T->code = merge(4, T->code, T->ptr[1] ? T->ptr[1]->code : NULL, T->ptr[2]->code, genLabel(T->ptr[2]->Snext));

            symboltable.Pop_until(symboltable.Pop_index());
            break;
        case FuncFParams:
            T->ptr[0]->level=1;   
            if(T->ptr[1]) T->ptr[1]->level=1;
            genIR(T->ptr[0]);
            genIR(T->ptr[1]);
            
            break;
        case FuncFParam:
            T->ptr[0]->level=1;   
            if(T->ptr[1]) T->ptr[1]->level=1;

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
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            break;
        case BType:
            
            break;
        case Block:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            
            // lev=lev+1;  
            
            symboltable.Push_index();
            genIR(T->ptr[0]);
            i=symboltable.Pop_index();
            symboltable.Pop_until(i);       
            // lev=lev-1;
            break;
        case BlockItems:
            if(T->ptr[0]) T->ptr[0]->level=(T->ptr[0]->kind==Block)?T->level+1:T->level;
            if(T->ptr[1])  T->ptr[1]->level=(T->ptr[1]->kind==Block)?T->level+1:T->level;
            genIR(T->ptr[0]);
            genIR(T->ptr[1]);
            break;
        case Decl:
            T->ptr[0]->level=T->level;
            genIR(T->ptr[0]);
            break;
        case ConstDecl:
            T->ptr[0]->level=T->level;
            T->ptr[1]->level=T->level;

            if(T->ptr[0]->kind==ConstDecl)  genIR(T->ptr[0]);
            mysymbol.name=string(T->ptr[1]->ptr[0]->type_id);
            mysymbol.level=T->level;
            // mysymbol.type=T->type;//类型后续调整
            son=static_cast<BasicType*>(T->pretype);
            mysymbol.types=son->getvalue();
            mysymbol.flag='C';
 
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
            T->ptr[0]->level=T->level;
            T->ptr[1]->level=T->level;

            if(T->ptr[0]->kind==VarDecl)  genIR(T->ptr[0]);
            mysymbol.name=string(T->ptr[1]->ptr[0]->type_id);
            mysymbol.level=T->level;
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
            for (int n = 0; n < i; n++)
                mysymbol.types+=string("[]");//数组的类型[]记在types
            
             symboltable.Push(mysymbol);  //常量（变量）入表
            break;
        case ConstDef:
            T->ptr[0]->level=T->level;
            T->ptr[1]->level=T->level;
            break;
        case VarDef:
            T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            break;
        case Idents:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            break;
        case InitVals:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            break;
        case InitVal:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            break;
        case ASSIGN:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            genIR(T->ptr[0]);
            genIR(T->ptr[1]);
            break;
        case LVal:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;

            // if(!T->ptr[0] && !T->ptr[1]){// IDENT
            //     index=symboltable.Search(T->type_id);//找到符号的索引
            //     symbol=symboltable.getSymbol(index)
            //     if(symbol没别名){
            //         T->codenode->string=symbol->name;
            //     }
            //     else{
            //         T->codenode->string=symbol->别名;
            //         symbol->别名=make_别名();
            //     }
            // }
            // else {// LVal LB Exp RB 
            //     T->codenode->string=T->ptr[0]->codenode->string+"{"+T->ptr[1]->string+"}";
            // }
            break;
        case Number://单个数值需要处理吗？测试案例有些只有一个整数
            break;
        case FuncCall:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[2]) T->ptr[2]->level=T->level;
            break;
        case FuncRParams:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[2]) T->ptr[2]->level=T->level;
            break;
        case UnaryExp:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[2]) T->ptr[2]->level=T->level;
            break;
        case AddExp:
        case MulExp:
        case LOrExp:
        case LAndExp:
        case EqExp:
        case RelExp:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[2]) T->ptr[2]->level=T->level;
            break;

        case IF:
            T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=(T->ptr[1]->kind==Block)?T->level+1:T->level;
            if(T->ptr[2]) T->ptr[2]->level=(T->ptr[2]->kind==Block)?T->level+1:T->level;
            genIR(T->ptr[1]);
            genIR(T->ptr[2]);
            break;
        case WHILE:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=(T->ptr[1]->kind==Block)?T->level+1:T->level;
            genIR(T->ptr[1]);
            break;
        case RETURN:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            break;
        case CONTINUE:
            break;
        case BREAK:
            break;
        }
    }
}
