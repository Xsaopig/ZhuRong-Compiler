#include "IR.hpp"
//@代表全局标识符（函数，全局变量）；
//%代表局部标识符（寄存器名称也就是局部变量，类型）。
void IRBuilder::Build(struct node *T)
{
    cout<<"开始中间代码生成"<<endl;
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

// struct codenode *IRBuilder::codegen(enum op_kind kind,Opn& opn1,Opn& opn2,Opn& result)
// {
//     struct codenode *p = new struct codenode();
//     p->op = kind;
//     p->opn1=opn1;
//     p->opn2=opn2;
//     p->result=p->result;
//     p->pre = p->next = p;
//     return p;
// }

string& IRBuilder::newLabel()
{
    auto s=new string("L");
    s->append(to_string(label++));
    return *s;
}

//合并多个中间代码的双向循环链表，首尾相连
// struct codenode *IRBuilder::merge(int num, ...)
// {
//     struct codenode *h1, *h2, *p, *t1, *t2;
//     va_list ap;
//     va_start(ap, num);
//     h1 = va_arg(ap, struct codenode *);
//     while (--num > 0)
//     {
//         h2 = va_arg(ap, struct codenode *);
//         if (h1 == NULL)
//             h1 = h2;
//         else if (h2)
//         {
//             t1 = h1->pre;
//             t2 = h2->pre;
//             t1->next = h2;
//             t2->next = h1;
//             h1->pre = t2;
//             h2->pre = t1;
//         }
//     }
//     va_end(ap);
//     return h1;
// }



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
    vector<int> places;
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
            offset=0;//offset是相对地址
            T->place=index;

            cout<<symboltable.getSymbol(T->place)->name<<":"<<endl;

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
            mysymbol.offset=offset;
            mysymbol.flag='P';
            if(T->pretype->is_BasicType()){
                if(!T->pretype->getvalue().compare("int"))
                {
                    mysymbol.type=INT;
                    offset+=4;
                }
                else if(!T->pretype->getvalue().compare("float"))
                {
                    mysymbol.type=FLOAT;
                    offset+=4;
                }
                else
                {
                    mysymbol.type=VOID;
                }
            }
            else if(T->pretype->is_Array_Type())
            {
                //形参中的数组得用指针来表示
                offset+=4;
            }
            T->place=symboltable.Push(mysymbol)-1;  

            cout<<"alloc "<<symboltable.getSymbol(T->place)->name
                <<" : "<<offset-symboltable.getSymbol(T->place)->offset<<endl;
            
            break;
        case FuncFParamArray:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            break;
        case BType:
            break;
        case Block:
            symboltable.Push_index();
            if(T->ptr[0]) {
                T->ptr[0]->Snext=(!T->Snext.empty())?T->Snext:newLabel();
                if(!T->Sbreak.empty())T->ptr[0]->Sbreak=T->Sbreak;
                if(!T->Scontinue.empty())T->ptr[0]->Scontinue=T->Scontinue;
                genIR(T->ptr[0],symboltable);
            }
            symboltable.Pop_until(symboltable.Pop_index());
            break;
        case BlockItems:
            if(T->ptr[1]){
                T->ptr[1]->Snext=T->Snext;
                T->ptr[0]->Snext=newLabel();
            }
            else
                T->ptr[0]->Snext=T->Snext;
            if(T->ptr[0]) 
            {
                if(!T->Sbreak.empty())T->ptr[0]->Sbreak=T->Sbreak;
                if(!T->Scontinue.empty())T->ptr[0]->Scontinue=T->Scontinue;
                genIR(T->ptr[0],symboltable);
            }
            if(T->ptr[1]) 
            {    
                if(!T->Sbreak.empty())T->ptr[1]->Sbreak=T->Sbreak;
                if(!T->Scontinue.empty())T->ptr[1]->Scontinue=T->Scontinue;
                genIR(T->ptr[1],symboltable);
            }    
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
            mysymbol.offset=offset;
            if(T->pretype->is_BasicType())
            {
                if(!T->pretype->getvalue().compare("int"))
                {
                    mysymbol.type=INT;
                    offset+=4;
                }
                else if(!T->pretype->getvalue().compare("float"))
                {    
                    mysymbol.type=FLOAT;
                    offset+=4;
                }
                else
                {
                    mysymbol.type=VOID;
                }
            }
            else if(T->pretype->is_Array_Type())
            {
                int cal=1;//数组元素的个数,如int[4][5]有20个元素
                for(int i=0;i<static_cast<Array_Type*>(T->pretype)->lev;i++)
                {
                    cal=cal*static_cast<Array_Type*>(T->pretype)->elements_nums[i];
                }
                offset+=4*cal;
            }
            T->place=symboltable.Push(mysymbol)-1;

            cout<<"alloc "<<symboltable.getSymbol(T->place)->name
                <<" : "<<offset-symboltable.getSymbol(T->place)->offset<<endl;
            //常量必须赋初值
            cout<<symboltable.getSymbol(T->place)->name<<" = "
                <<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;

            break;
        case VarDef:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            //符号入表
            mysymbol.name=string(T->ptr[0]->type_id);
            mysymbol.level=T->level;
            mysymbol.flag='V';
            mysymbol.pretype=T->pretype;
            mysymbol.types=T->pretype->getvalue();
            mysymbol.offset=offset;
            if(T->pretype->is_BasicType())
            {
                if(!T->pretype->getvalue().compare("int"))
                {
                    mysymbol.type=INT;
                    offset+=4;
                }
                else if(!T->pretype->getvalue().compare("float"))
                {    
                    mysymbol.type=FLOAT;
                    offset+=4;
                }
                else
                {
                    mysymbol.type=VOID;
                }
            }
            else if(T->pretype->is_Array_Type())
            {
                int cal=1;//数组元素的个数,如int[4][5]有20个元素
                for(int i=0;i<static_cast<Array_Type*>(T->pretype)->lev;i++)
                {
                    cal=cal*static_cast<Array_Type*>(T->pretype)->elements_nums[i];
                }
                offset+=4*cal;
            }
            T->place=symboltable.Push(mysymbol)-1;

            cout<<"alloc "<<symboltable.getSymbol(T->place)->name
                <<" : "<<offset-symboltable.getSymbol(T->place)->offset<<endl;

            if(T->ptr[1])//变量赋初值了
            {
                cout<<symboltable.getSymbol(T->place)->name<<" = "
                    <<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;
                
            }
            else if(T->level==0)//变量没有赋初值，但是是全局变量，要被初始化为0
            {
                cout<<symboltable.getSymbol(T->place)->name<<" = "<<0<<endl;
            }


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


            if(T->out==false) break;
            T->place=newtemp(T->pretype,T->level,offset);
            offset+=4;
            
            T0=T;
            while(T0 && T0->ptr[1])
            {
                places.push_back(T0->ptr[1]->place);
                T0=T0->ptr[0];
            }
            places.push_back(T0->ptr[0]->place);
            if(places.size()==1){
                cout<<symboltable.getSymbol(T->place)->name<<" = { "
                    <<symboltable.getSymbol(places[0])->name<<" }"<<endl;
            }
            else{
                cout<<symboltable.getSymbol(T->place)->name<<" = { ";
                for(int i=places.size()-1;i>=0;i--)
                    cout<<symboltable.getSymbol(places[i])->name<<" ";
                cout<<"}"<<endl;
            }


            break;
        case InitVal:
            if(T->ptr[0]) {
                T->ptr[0]->out=true;
                genIR(T->ptr[0],symboltable);
                T->place=T->ptr[0]->place;
            }
            else{
                T->place=newtemp(T->pretype,T->level,offset);
                offset+=4;
                cout<<symboltable.getSymbol(T->place)->name<<" = { }"<<endl;
            }
            break;
        case ASSIGN:
            T->Snext=(T->Snext.empty())?newLabel():T->Snext;
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);


            if(T->ptr[0]->offset==-1)
                cout<<symboltable.getSymbol(T->ptr[0]->place)->name<<" = "
                    <<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;
            else    
                cout<<symboltable.getSymbol(T->ptr[0]->base_addr)->name<<" [ "
                    <<symboltable.getSymbol(T->ptr[0]->offset)->name<<" ] = " 
                    <<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;

            break;
        case LVal://经过处理之后，T->place存储的是对应元素在符号表中的位置
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(!T->ptr[0])
            {//LVal: IDENT
                index=symboltable.Search(string(T->type_id));
                T->place=index;
                T->offset=-1;//不是数组，不需要offset
            }
            else
            //这一部分得看龙书313页，数组元素寻址的翻译模式
            {//LVal: LVal LB Exp RB
                if(!T->ptr[0]->ptr[0]){         //LVal: (IDENT) LB Exp RB
                    T->array=symboltable.getSymbol(symboltable.Search(string(T->ptr[0]->type_id)));
                    T->ndim=1;
                    T->width=4;//int和float都占用4个字节
                    //暂时先不链接codenode，直接printf输出
                    if(T->ndim < static_cast<Array_Type*>(T->array->pretype)->lev)
                    {//没到数组的最后一层
                        T->offset=newtemp(new BasicType("int"),T->level,offset);
                        T->place=T->offset;
                        offset+=4;
                        symbol=symboltable.getSymbol(T->offset);
                        int limit=static_cast<Array_Type*>(T->array->pretype)->elements_nums[T->ndim];//ndim-1维数组的元素个数
                        cout<<symbol->name<<" = "<<symboltable.getSymbol(T->ptr[1]->place)->name<<" * "<<limit<<endl;
                    }
                    else//数组的最后一层
                    {
                        T->base_addr=newtemp(new BasicType("int"),T->level,offset);
                        offset+=4;
                        if(T->array->flag!='P')
                            cout<<symboltable.getSymbol(T->base_addr)->name<<" = "<<T->array->offset<<endl;
                        else
                            cout<<symboltable.getSymbol(T->base_addr)->name<<" = "<<T->array->name<<endl;
                        T->offset=newtemp(new BasicType("int"),T->level,offset);
                        offset+=4;
                        symbol=symboltable.getSymbol(T->offset);
                        cout<<symbol->name<<" = "<<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;
                        T->place=newtemp(new BasicType("int"),T->level,offset);
                        offset+=4;
                        cout<<symboltable.getSymbol(T->place)->name<<" = "
                            <<symboltable.getSymbol(T->base_addr)->name<<" [ "
                            <<symboltable.getSymbol(T->offset)->name<<" ] "<<endl;
                        
                    }
                }
                else{                           //LVal: (LVal LB Exp RB) LB Exp RB
                    T->ndim=T->ptr[0]->ndim+1;
                    T->width=T->ptr[0]->width;
                    T->offset=newtemp(new BasicType("int"),T->level,offset);
                    offset+=4;
                    T->array=T->ptr[0]->array;
                    //暂时先不链接codenode，直接printf输出
                    symbol=symboltable.getSymbol(T->ptr[0]->offset);
                    cout<<symbol->name<<" = "<<symbol->name<<" + "<<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;
                    if(T->ndim < static_cast<Array_Type*>(T->array->pretype)->lev)
                    {//没到数组的最后一层
                        int limit=static_cast<Array_Type*>(T->array->pretype)->elements_nums[T->ndim];
                        cout<<symboltable.getSymbol(T->offset)->name<<" = "<<symbol->name<<" * "<<limit<<endl;
                        T->place=T->offset;
                    }
                    else//数组的最后一层
                    {
                        //至此，T->place存储的是对应数组元素在符号表中的位置
                        T->base_addr=newtemp(new BasicType("int"),T->level,offset);
                        offset+=4;
                        if(T->array->flag!='P')
                            cout<<symboltable.getSymbol(T->base_addr)->name<<" = "<<T->array->offset<<endl;
                        else
                            cout<<symboltable.getSymbol(T->base_addr)->name<<" = "<<T->array->name<<endl;
                        T->offset=newtemp(new BasicType("int"),T->level,offset);
                        offset+=4;
                        cout<<symboltable.getSymbol(T->offset)->name<<" = "<<T->ptr[0]->width<<" * "<<symboltable.getSymbol(T->ptr[0]->offset)->name<<endl;
                        T->place=newtemp(new BasicType("int"),T->level,offset);
                        offset+=4;
                        cout<<symboltable.getSymbol(T->place)->name<<" = "
                            <<symboltable.getSymbol(T->base_addr)->name<<" [ "
                            <<symboltable.getSymbol(T->offset)->name<<" ] "<<endl;
                    }
                }
            }
            break;
        case Number:
            T->place=newtemp(T->pretype,T->level,offset);
            offset+=4;
            if(T->type==INT)
                cout<<symboltable.getSymbol(T->place)->name<<" = "<<T->type_int<<endl;
            else
                cout<<symboltable.getSymbol(T->place)->name<<" = "<<T->type_float<<endl;
            break;
        case FuncCall:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);

            symbol=symboltable.getSymbol(symboltable.Search(string(T->type_id)));
            T->place=newtemp(&static_cast<Fun_Type*>(symbol->pretype)->basictype,T->level,offset);
            offset+=4;
            if(static_cast<Fun_Type*>(symbol->pretype)->basictype.getvalue().compare("void")!=0)
            {//函数有返回值
                cout<<symboltable.getSymbol(T->place)->name<<" = call "
                    <<symbol->name<<" , "
                    <<symbol->paramnum<<endl;
            }
            else
            {//函数没有返回值
                cout<<"call "<<symbol->name<<" , "
                    <<symbol->paramnum<<endl;
            }
            
            break;
        case FuncRParams:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[1])
                cout<<"param "<<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;
            else
                cout<<"param "<<symboltable.getSymbol(T->ptr[0]->place)->name<<endl;
            break;
        case UnaryExp:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            T->place=newtemp(T->pretype,T->level,offset);
            offset+=4;
            cout<<symboltable.getSymbol(T->place)->name<<" = "
                <<string(T->op)<<" "
                <<symboltable.getSymbol(T->ptr[0]->place)->name<<endl;
            break;
        case AddExp:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            T->place=newtemp(T->pretype,T->level,offset);
            offset+=4;
            cout<<symboltable.getSymbol(T->place)->name<<" = "
                <<symboltable.getSymbol(T->ptr[0]->place)->name<<" + "
                <<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;
            break;
        case MulExp:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            T->place=newtemp(T->pretype,T->level,offset);
            offset+=4;
            cout<<symboltable.getSymbol(T->place)->name<<" = "
                <<symboltable.getSymbol(T->ptr[0]->place)->name<<" + "
                <<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;
            break;
        case LOrExp://逻辑或表达式
            T->Etrue=(T->Etrue.empty())?newLabel():T->Etrue;
            T->Efalse=(T->Efalse.empty())?newLabel():T->Efalse;
            T->ptr[0]->Etrue=T->Etrue;
            T->ptr[0]->Efalse=newLabel();
            T->ptr[1]->Etrue=T->Etrue;
            T->ptr[1]->Efalse=T->Efalse;
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            cout<<T->ptr[0]->Efalse<<":"<<endl;
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);



            break;
        case LAndExp://逻辑与表达式
            T->Etrue=(T->Etrue.empty())?newLabel():T->Etrue;
            T->Efalse=(T->Efalse.empty())?newLabel():T->Efalse;
            T->ptr[0]->Etrue=newLabel();
            T->ptr[0]->Efalse=T->Efalse;
            T->ptr[1]->Etrue=T->Etrue;
            T->ptr[1]->Efalse=T->Efalse;
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            cout<<T->ptr[0]->Etrue<<":"<<endl;
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            break;
        case EqExp://相等性表达式
        case RelExp://关系表达式
            T->Etrue=(T->Etrue.empty())?newLabel():T->Etrue;
            T->Efalse=(T->Efalse.empty())?newLabel():T->Efalse;
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            cout<<"if "<<symboltable.getSymbol(T->ptr[0]->place)->name<<" "+string(T->op)+" "
                <<symboltable.getSymbol(T->ptr[1]->place)->name<<" goto "<<T->Etrue<<endl;
            cout<<"goto "<<T->Efalse<<endl;
            break;
        case IF:
            T->Snext=(T->Snext.empty())?newLabel():T->Snext;
            if(!T->ptr[2]) T->ptr[1]->Snext=T->Snext;
            else T->ptr[1]->Snext=newLabel();
            T->ptr[0]->Efalse=T->ptr[1]->Snext;
            if(T->ptr[0]) 
            {
                T->ptr[0]->Etrue=newLabel();
                genIR(T->ptr[0],symboltable);
                cout<<T->ptr[0]->Etrue<<":"<<endl;
            }
            if(T->ptr[1]) 
            {
                if(!T->Sbreak.empty())T->ptr[1]->Sbreak=T->Sbreak;
                if(!T->Scontinue.empty())T->ptr[1]->Scontinue=T->Scontinue;
                genIR(T->ptr[1],symboltable);
            }
            if(T->ptr[2]) 
            {
                if(!T->Sbreak.empty())T->ptr[2]->Sbreak=T->Sbreak;
                if(!T->Scontinue.empty())T->ptr[2]->Scontinue=T->Scontinue;
                cout<<"goto "<<T->Snext<<endl;
                cout<<T->ptr[0]->Efalse<<":"<<endl;
                T->ptr[2]->Snext=T->Snext;
                genIR(T->ptr[2],symboltable);
            }
            cout<<T->Snext<<":"<<endl;
            break;
        case WHILE:
            T->Snext=(T->Snext.empty())?newLabel():T->Snext;
            T->ptr[0]->Etrue=newLabel();
            T->ptr[0]->Efalse=T->Snext;
            T->ptr[1]->Snext=newLabel();
            T->Sbreak=T->Snext;
            T->Scontinue=T->ptr[1]->Snext;
            if(T->ptr[0]) 
            {
                cout<<T->ptr[1]->Snext<<":"<<endl;
                genIR(T->ptr[0],symboltable);
                cout<<T->ptr[0]->Etrue<<":"<<endl;
            }
            if(T->ptr[1]) 
            {
                T->ptr[1]->Sbreak=T->Sbreak;
                T->ptr[1]->Scontinue=T->Scontinue;
                genIR(T->ptr[1],symboltable);
                cout<<"goto "<<T->ptr[1]->Snext<<endl;
            }
            cout<<T->Snext<<":"<<endl;

            break;
        case RETURN:
            T->Snext=(T->Snext.empty())?newLabel():T->Snext;
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            if(T->ptr[0])
                cout<<"return "<<symboltable.getSymbol(T->ptr[0]->place)->name<<endl;
            else
                cout<<"return "<<endl;
            break;
        case CONTINUE:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            cout<<"goto "<<T->Scontinue<<endl;
            break;
        case BREAK:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            cout<<"goto "<<T->Sbreak<<endl;
            break;
        }
        }
        return;
    }
