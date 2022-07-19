  #include "IR.hpp"
//@代表全局标识符（函数，全局变量）；
//%代表局部标识符（寄存器名称也就是局部变量，类型）。
void IRBuilder::Build(struct node *T)
{
    cout<<"开始中间代码生成"<<endl;
    genIR(T,symboltable);
    MIRprint();
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

string& IRBuilder::newLabel()
{
    auto s=new string(".L");
    s->append(to_string(label++));
    return *s;
}

void IRBuilder::genIR(struct node *T,Symboltable &symboltable) 
{
    Symbol mysymbol;
    struct node *cur,*T0;
    BasicType* son;
    int record;
    int i;
    int index;
    Symbol *symbol,*symbo2,*symbo3;
    string opn_type,temp_str="";
    Opn *opn1,*opn2,*opn3,*result;
    IR *ir;
    vector<Opn*> arr_opns;
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

            //添加中间代码到IRList
            symbol=symboltable.getSymbol(T->place);
            opn1=new Opn(Opn::Func,symbol->name);
            opn1->level=0;
            opn1->offset=symbol->level;
            opn1->place=T->place;
            ir=new IR(IR::_FUNC,*opn1);
            IRList.push_back(ir);

            //cout<<symbol->name<<":"<<endl;

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

            T->place=symboltable.Push(mysymbol)-1;  
            symbol=symboltable.getSymbol(T->place);
            //区分操作数是 var还是array
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
                opn1=new Opn(Opn::Var,symbol->name);
                //IR_kind=Opn::Var;
            }
            else if(T->pretype->is_Array_Type())
            {
                //形参中的数组得用指针来表示
                offset+=4;
                opn1=new Opn(Opn::Array,symbol->name);
                //IR_kind=Opn::Array;
            }
                  
            opn1->level=1;
            opn1->offset=symbol->offset;
            i=offset-symbol->offset;
            opn2=new Opn(Opn::Imm,i);
            opn2->is_int=true;
            ir=new IR(IR::_ALLOC,*opn1,*opn2);
            IRList.push_back(ir);

            /*cout<<"alloc "<<symboltable.getSymbol(T->place)->name
                <<" : "<<offset-symboltable.getSymbol(T->place)->offset<<endl;*/
            
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


            //符号入表
            mysymbol.name=string(T->ptr[0]->type_id);
            mysymbol.level=T->level;
            mysymbol.flag='C';
            mysymbol.pretype=T->pretype;
            mysymbol.types=T->pretype->getvalue();
            mysymbol.offset=offset;
            T->place=symboltable.Push(mysymbol)-1;  
            symbol=symboltable.getSymbol(T->place);
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
                opn1=new Opn(Opn::Var,symbol->name);
                //IR_kind=Opn::Var;
            }
            else if(T->pretype->is_Array_Type())
            {
                int cal=1;//数组元素的个数,如int[4][5]有20个元素
                for(int i=0;i<static_cast<Array_Type*>(T->pretype)->lev;i++)
                {
                    cal=cal*static_cast<Array_Type*>(T->pretype)->elements_nums[i];
                }
                offset+=4*cal;
                opn1=new Opn(Opn::Array,symbol->name);
                //IR_kind=Opn::Array;
            }
            
            opn1->level=symbol->level;
            opn1->offset=symbol->offset;
            i=offset-symbol->offset;
            opn2=new Opn(Opn::Imm,i);
            opn2->is_int=true;

            if(T->level==0)
                IRList.push_back(new IR(IR::_DATA_BEGIN));

            ir=new IR(IR::_ALLOC,*opn1,*opn2);
            IRList.push_back(ir);

            /*cout<<"alloc "<<symboltable.getSymbol(T->place)->name
                <<" : "<<offset-symboltable.getSymbol(T->place)->offset<<endl;*/
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            
            //常量必须赋初值
            symbol=symboltable.getSymbol(T->ptr[1]->place);
            opn3=new Opn(Opn::Var,symbol->name);
            ir=new IR(IR::_ASSIGN,*opn3,*opn1);
            IRList.push_back(ir);

            /*     opn1=opn3
            cout<<symboltable.getSymbol(T->place)->name<<" = "
                <<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;*/
            
            if(T->level==0)
                IRList.push_back(new IR(IR::_DATA_END));
            break;
        case VarDef:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            
            //符号入表
            mysymbol.name=string(T->ptr[0]->type_id);
            mysymbol.level=T->level;
            mysymbol.flag='V';
            mysymbol.pretype=T->pretype;
            mysymbol.types=T->pretype->getvalue();
            mysymbol.offset=offset;

            T->place=symboltable.Push(mysymbol)-1;
            symbol=symboltable.getSymbol(T->place);
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
                opn1=new Opn(Opn::Var,symbol->name);

            }
            else if(T->pretype->is_Array_Type())
            {
                int cal=1;//数组元素的个数,如int[4][5]有20个元素
                for(int i=0;i<static_cast<Array_Type*>(T->pretype)->lev;i++)
                {
                    cal=cal*static_cast<Array_Type*>(T->pretype)->elements_nums[i];
                }
                offset+=4*cal;
                opn1=new Opn(Opn::Array,symbol->name);
                //IR_kind=Opn::Array;
            }

            opn1->level=symbol->level;
            opn1->offset=symbol->offset;
            i=offset-symbol->offset;
            opn2=new Opn(Opn::Imm,i);
            opn2->is_int=true;

            if(T->level==0)
                IRList.push_back(new IR(IR::_DATA_BEGIN));

            ir=new IR(IR::_ALLOC,*opn1,*opn2);
            IRList.push_back(ir);
            
            /*cout<<"alloc "<<symboltable.getSymbol(T->place)->name
                <<" : "<<offset-symboltable.getSymbol(T->place)->offset<<endl;*/
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            
            if(T->ptr[1])//变量赋初值了
            {
                symbo2=symboltable.getSymbol(T->ptr[1]->place);
                opn3=new Opn(Opn::Var,symbo2->name);
                ir=new IR(IR::_ASSIGN,*opn3,*opn1);
                IRList.push_back(ir);

                /*cout<<symboltable.getSymbol(T->place)->name<<" = "
                    <<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;*/
                
            }
            else if(T->level==0)//变量没有赋初值，但是是全局变量，要被初始化为0
            {
                opn3=new Opn(Opn::Imm,0);
                opn3->is_int=1;
                ir=new IR(IR::_ASSIGN,*opn3,*opn1);
                IRList.push_back(ir);
                //cout<<symboltable.getSymbol(T->place)->name<<" = "<<0<<endl;
            }

            if(T->level==0)
                IRList.push_back(new IR(IR::_DATA_END));
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

            symbol=symboltable.getSymbol(T->place);
            if(places.size()==1){

                opn1=new Opn(Opn::Var,symbol->name);
                opn1->level=symbol->level;
                opn1->offset=symbol->offset;
                symbo2=symboltable.getSymbol(places[0]);
                opn2=new Opn(Opn::Var,symbo2->name);
                arr_opns.push_back(opn2);
                opn3=new Opn(Opn::Block,arr_opns);

                ir=new IR(IR::_ASSIGN,*opn3,*opn1);
                IRList.push_back(ir);
                /*cout<<symboltable.getSymbol(T->place)->name<<" = { "
                    <<symboltable.getSymbol(places[0])->name<<" }"<<endl;*/
            }
            else{
                opn1=new Opn(Opn::Array,symbol->name);
                opn1->level=symbol->level;
                opn1->offset=symbol->offset;
                for(int i=places.size()-1;i>=0;i--){
                    symbo3=symboltable.getSymbol(places[i]);
                    opn3=new Opn(Opn::Var,symbo3->name);
                    arr_opns.push_back(opn3);
                }

                opn2=new Opn(Opn::Block,arr_opns);
                ir=new IR(IR::_ASSIGN,*opn2,*opn1);
                IRList.push_back(ir);

                /*cout<<symboltable.getSymbol(T->place)->name<<" = { ";
                for(int i=places.size()-1;i>=0;i--)
                    cout<<symboltable.getSymbol(places[i])->name<<" ";
                cout<<"}"<<endl;*/
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

                symbol=symboltable.getSymbol(T->place);
                opn1=new Opn(Opn::Array,symbol->name);
                opn1->level=symbol->level;
                opn1->offset=symbol->offset;

                opn2=new Opn(Opn::Block,arr_opns);
                ir=new IR(IR::_ASSIGN,*opn2,*opn1);
                IRList.push_back(ir);
                //cout<<symboltable.getSymbol(T->place)->name<<" = { }"<<endl;
            }
            break;
        case ASSIGN:
            T->Snext=(T->Snext.empty())?newLabel():T->Snext;
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);


            if(T->ptr[0]->offset==-1)
            {
                symbol=symboltable.getSymbol(T->ptr[0]->place);
                opn1=new Opn(Opn::Var,symbol->name);
                opn1->level=symbol->level;
                opn1->offset=symbol->offset;
                symbo2=symboltable.getSymbol(T->ptr[1]->place);
                opn2=new Opn(Opn::Var,symbo2->name);
                opn2->level=symbo2->level;
                opn2->offset=symbo2->offset;
                ir=new IR(IR::_ASSIGN,*opn2,*opn1);
                IRList.push_back(ir);
            }
                /*cout<<symboltable.getSymbol(T->ptr[0]->place)->name<<" = "
                    <<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;*/
            else{
                symbol=symboltable.getSymbol(T->ptr[0]->base_addr);
                opn1=new Opn(Opn::Array,symbol->name);
                opn1->level=symbol->level;
                opn1->offset=symbol->offset;
                symbo2=symboltable.getSymbol(T->ptr[0]->offset);
                opn2=new Opn(Opn::Var,symbo2->name);

                symbo3=symboltable.getSymbol(T->ptr[1]->place);
                opn3=new Opn(Opn::Var,symbo3->name);
                opn3->level=symbo3->level;
                opn3->offset=symbo3->offset;

                ir=new IR(IR::_Arr_ASSIGN,*opn1,*opn2,*opn3);
                IRList.push_back(ir);
            }    
                /*cout<<symboltable.getSymbol(T->ptr[0]->base_addr)->name<<" [ "
                    <<symboltable.getSymbol(T->ptr[0]->offset)->name<<" ] = " 
                    <<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;*/

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
                    T->array=symboltable.getSymbol(symboltable.Search(string(T->type_id)));
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

                        opn1=new Opn(Opn::Var,symbol->name);
                        opn1->level=symbol->level;
                        opn1->offset=symbol->offset;
                        symbo2=symboltable.getSymbol(T->ptr[1]->place);
                        // symbo2->name+=" * "+to_string(limit);
                        opn2=new Opn(Opn::Var,symbo2->name);
                        opn2->level=symbo2->level;
                        opn2->offset=symbo2->offset;
                        opn3=new Opn(Opn::Imm,limit);
                        opn3->is_int=1;
                        ir=new IR(IR::_MUL,*opn2,*opn3,*opn1);
                        IRList.push_back(ir);
                        
                        //cout<<symbol->name<<" = "<<symboltable.getSymbol(T->ptr[1]->place)->name<<" * "<<limit<<endl;
                    }
                    else//数组的最后一层
                    {
                        T->base_addr=newtemp(new BasicType("int"),T->level,offset);
                        offset+=4;
                        symbol=symboltable.getSymbol(T->base_addr);
                        opn1=new Opn(Opn::Var,symbol->name);
                        opn1->level=symbol->level;
                        opn1->offset=symbol->offset;
                        opn2=new Opn(Opn::Var,T->array->name);
                        if(T->array->flag!='P'){

                            ir=new IR(IR::_ADDR,*opn2,*opn1);
                            IRList.push_back(ir);
                        }
                            //cout<<symboltable.getSymbol(T->base_addr)->name<<" = &"<<T->array->name<<endl;
                        else{
                            
                            ir=new IR(IR::_ASSIGN,*opn2,*opn1);
                            IRList.push_back(ir);
                        }
                            //cout<<symboltable.getSymbol(T->base_addr)->name<<" = "<<T->array->name<<endl;
                        T->offset=newtemp(new BasicType("int"),T->level,offset);
                        offset+=4;

                        symbo2=symboltable.getSymbol(T->offset);
                        opn2=new Opn(Opn::Var,symbo2->name);
                        opn2->level=symbo2->level;
                        opn2->offset=symbo2->offset;

                        symbo3=symboltable.getSymbol(T->ptr[1]->place);
                        opn3=new Opn(Opn::Var,symbo3->name);
                        opn3->level=symbo3->level;
                        opn3->offset=symbo3->offset; 
                        ir=new IR(IR::_ASSIGN,*opn3,*opn2);
                        IRList.push_back(ir);
                        //cout<<symbol->name<<" = "<<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;

                        T->place=newtemp(new BasicType("int"),T->level,offset);
                        offset+=4;
                        symbo2=symboltable.getSymbol(T->offset);
                        opn2=new Opn(Opn::Var,symbo2->name);
                        opn2->level=symbo2->level;
                        opn2->offset=symbo2->offset;

                        symbo3=symboltable.getSymbol(T->place);
                        opn3=new Opn(Opn::Var,symbo3->name);
                        opn3->level=symbo3->level;
                        opn3->offset=symbo3->offset;

                        ir=new IR(IR::_ASSIGN_Arr,*opn3,*opn1,*opn2);
                        IRList.push_back(ir);
                        /*cout<<symboltable.getSymbol(T->place)->name<<" = "
                            <<T->array->name<<" [ " 
                            <<symboltable.getSymbol(T->offset)->name<<" ] "<<endl;*/
                        
                    }
                }
                else{                           //LVal: (LVal LB Exp RB) LB Exp RB
                    T->ndim=T->ptr[0]->ndim+1;
                    T->width=T->ptr[0]->width;
                    T->offset=newtemp(new BasicType("int"),T->level,offset);
                    offset+=4;
                    T->array=symboltable.getSymbol(symboltable.Search(string(T->type_id)));
    
                    symbol=symboltable.getSymbol(T->ptr[0]->offset);

                    result=new Opn(Opn::Var,symbol->name);
                    result->level=symbol->level;
                    result->offset=symbol->offset;
                    symbo2=symboltable.getSymbol(T->ptr[1]->place);
                    opn2=new Opn(Opn::Var,symbo2->name);
                    opn2->level=symbo2->level;
                    opn2->offset=symbo2->offset;  
                    ir=new IR(IR::_ADD,*result,*opn2,*result);
                    IRList.push_back(ir);
                    //cout<<symbol->name<<" = "<<symbol->name<<" + "<<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;

                    if(T->ndim < static_cast<Array_Type*>(T->array->pretype)->lev)
                    {//没到数组的最后一层
                        int limit=static_cast<Array_Type*>(T->array->pretype)->elements_nums[T->ndim];

                        symbo2=symboltable.getSymbol(T->offset);
                        result=new Opn(Opn::Var,symbo2->name);
                        result->level=symbo2->level;
                        result->offset=symbo2->offset;  

                        // symbol->name+=" * "+to_string(limit);
                        opn1=new Opn(Opn::Var,symbol->name);
                        opn1->level=symbol->level;
                        opn1->offset=symbol->offset; 
                        opn2=new Opn(Opn::Imm,limit);
                        opn2->is_int=true;
                        ir=new IR(IR::_MUL,*opn1,*opn2,*result);
                        IRList.push_back(ir);
                        //cout<<symboltable.getSymbol(T->offset)->name<<" = "<<symbol->name<<" * "<<limit<<endl;
                        T->place=T->offset;
                    }
                    else//数组的最后一层
                    {
                        //至此，T->place存储的是对应数组元素在符号表中的位置
                        T->base_addr=newtemp(new BasicType("int"),T->level,offset);
                        offset+=4;

                        symbol=symboltable.getSymbol(T->base_addr);
                        opn1=new Opn(Opn::Array,symbol->name);
                        opn1->level=symbol->level;
                        opn1->offset=symbol->offset;
                        opn2=new Opn(Opn::Var,T->array->name);
                        if(T->array->flag!='P'){
                           
                            ir=new IR(IR::_ADDR,*opn2,*opn1);
                            IRList.push_back(ir);
                        }
                            //cout<<symboltable.getSymbol(T->base_addr)->name<<" = &"<<T->array->name<<endl;
                        else{
                            
                            ir=new IR(IR::_ASSIGN,*opn2,*opn1);
                            IRList.push_back(ir);
                        }
                            //cout<<symboltable.getSymbol(T->base_addr)->name<<" = "<<T->array->name<<endl;
                        T->offset=newtemp(new BasicType("int"),T->level,offset);
                        offset+=4;

                        symbo2=symboltable.getSymbol(T->offset);
                        opn2=new Opn(Opn::Var,symbo2->name);
                        opn2->level=symbo2->level;
                        opn2->offset=symbo2->offset;

                        symbo3=symboltable.getSymbol(T->ptr[0]->offset);
                        opn3=new Opn(Opn::Var,symbo3->name);
                        opn3->level=symbo3->level;
                        opn3->offset=symbo3->offset;

                        temp_str=to_string(T->ptr[0]->width);
                        result=new Opn(Opn::Var,temp_str);
                        ir=new IR(IR::_MUL,*opn3,*result,*opn2);
                        IRList.push_back(ir);
                        //cout<<symboltable.getSymbol(T->offset)->name<<" = "<<T->ptr[0]->width<<" * "<<symboltable.getSymbol(T->ptr[0]->offset)->name<<endl;

                        T->place=newtemp(new BasicType("int"),T->level,offset);
                        offset+=4;

                        symbo2=symboltable.getSymbol(T->offset);
                        opn2=new Opn(Opn::Var,symbo2->name);
                        opn2->level=symbo2->level;
                        opn2->offset=symbo2->offset;

                        symbo3=symboltable.getSymbol(T->place);
                        opn3=new Opn(Opn::Var,symbo3->name);
                        opn3->level=symbo3->level;
                        opn3->offset=symbo3->offset;

                        ir=new IR(IR::_ASSIGN_Arr,*opn3,*opn1,*opn2);
                        IRList.push_back(ir);
                       /* cout<<symboltable.getSymbol(T->place)->name<<" = "
                            <<T->array->name<<" [ "
                            <<symboltable.getSymbol(T->offset)->name<<" ] "<<endl;*/
                    }
                }
            }
            break;
        case Number:
            T->place=newtemp(T->pretype,T->level,offset);
            offset+=4;
            if(T->type==INT){
                    symbol=symboltable.getSymbol(T->place);
                    opn1=new Opn(Opn::Var,symbol->name);
                    opn1->level=symbol->level;
                    opn1->is_int=true;
                    opn1->offset=symbol->offset;
                    opn2=new Opn(Opn::Imm,T->type_int);
                    opn2->is_int=true;
                    ir=new IR(IR::_ASSIGN,*opn2,*opn1);
                    IRList.push_back(ir);
            }
                //cout<<symboltable.getSymbol(T->place)->name<<" = "<<T->type_int<<endl;
            else{
                    symbol=symboltable.getSymbol(T->place);
                    opn1=new Opn(Opn::Var,symbol->name);
                    opn1->level=symbol->level;
                    opn1->offset=symbol->offset;
                    opn1->is_int=false;
                    opn2=new Opn(Opn::Imm,T->type_float);
                    opn2->is_int=false;
                    ir=new IR(IR::_ASSIGN,*opn2,*opn1);
                    IRList.push_back(ir);
            }
                //cout<<symboltable.getSymbol(T->place)->name<<" = "<<T->type_float<<endl;
            break;
        case FuncCall:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);

            symbol=symboltable.getSymbol(symboltable.Search(string(T->type_id)));
            T->place=newtemp(&static_cast<Fun_Type*>(symbol->pretype)->basictype,T->level,offset);
            symbol=symboltable.getSymbol(symboltable.Search(string(T->type_id)));
            offset+=4;
            if(static_cast<Fun_Type*>(symbol->pretype)->basictype.getvalue().compare("void")!=0)
            {//函数有返回值
                symbo3=symboltable.getSymbol(T->place);
                opn3=new Opn(Opn::Var,symbo3->name);
                opn3->level=symbo3->level;
                opn3->offset=symbo3->offset;

                opn1=new Opn(Opn::Var,symbol->name);
                opn1->level=symbol->level;
                opn1->offset=symbol->offset;
                if(static_cast<Fun_Type*>(symbol->pretype)->basictype.getvalue().compare("int")==0)
                opn1->is_int=true;
                opn2=new Opn(Opn::Imm,symbol->paramnum);

                ir=new IR(IR::_CALL,*opn1,*opn2,*opn3);
                IRList.push_back(ir);
                /*cout<<symboltable.getSymbol(T->place)->name<<" = call "
                    <<symbol->name<<" , "
                    <<symbol->paramnum<<endl;*/
            }
            else
            {//函数没有返回值

                opn1=new Opn(Opn::Var,symbol->name);
                opn1->level=symbol->level;
                opn1->offset=symbol->offset;

                opn2=new Opn(Opn::Imm,symbol->paramnum);
                opn3=new Opn();
                ir=new IR(IR::_CALL,*opn1,*opn2,*opn3);
                IRList.push_back(ir);
                /*cout<<"call "<<symbol->name<<" , "
                    <<symbol->paramnum<<endl;*/
            }
            
            break;
        case FuncRParams:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[1]){
                symbol=symboltable.getSymbol(T->ptr[1]->place);
                opn1=new Opn(Opn::Var,symbol->name);
                opn1->level=symbol->level;
                opn1->offset=symbol->offset;
                ir=new IR(IR::_PARAM,*opn1);
                IRList.push_back(ir);
            }
                //cout<<"param "<<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;
            else{
                symbol=symboltable.getSymbol(T->ptr[0]->place);
                opn1=new Opn(Opn::Var,symbol->name);
                opn1->level=symbol->level;
                opn1->offset=symbol->offset;
                ir=new IR(IR::_PARAM,*opn1);
                IRList.push_back(ir);
            }
               // cout<<"param "<<symboltable.getSymbol(T->ptr[0]->place)->name<<endl;
            break;
        case UnaryExp:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            T->place=newtemp(T->pretype,T->level,offset);
            offset+=4;

            
            symbol=symboltable.getSymbol(T->ptr[0]->place);
            opn1=new Opn(Opn::Var,symbol->name);
            opn1->level=symbol->level;
            opn1->offset=symbol->offset;
            symbo2=symboltable.getSymbol(T->place);
            opn2=new Opn(Opn::Var,symbo2->name);
            opn2->level=symbo2->level;
            opn2->offset=symbo2->offset;

            temp_str=string(T->op);
            if(!temp_str.compare("+")) ir=new IR(IR::_POSI,*opn1,*opn2);
            if(!temp_str.compare("-")) ir=new IR(IR::_NEGA,*opn1,*opn2);
            if(!temp_str.compare("!")) ir=new IR(IR::_NOT,*opn1,*opn2);
            IRList.push_back(ir);

            /*cout<<symboltable.getSymbol(T->place)->name<<" = "
                <<string(T->op)<<" "
                <<symboltable.getSymbol(T->ptr[0]->place)->name<<endl;*/
            break;
        case AddExp:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            T->place=newtemp(T->pretype,T->level,offset);
            offset+=4;


            symbol=symboltable.getSymbol(T->ptr[0]->place);
            opn1=new Opn(Opn::Var,symbol->name);
            opn1->level=symbol->level;
            opn1->offset=symbol->offset;
            if(T->pretype->getvalue().compare("int")==0)
                opn1->is_int=true;
                
            symbo2=symboltable.getSymbol(T->ptr[1]->place);
            opn2=new Opn(Opn::Var,symbo2->name);
            opn2->level=symbo2->level;
            opn2->offset=symbo2->offset;
            symbo3=symboltable.getSymbol(T->place);
            opn3=new Opn(Opn::Var,symbo3->name);
            opn3->level=symbo3->level;
            opn3->offset=symbo3->offset;
            
            temp_str=string(T->op);
            if(!temp_str.compare("+")) ir=new IR(IR::_ADD,*opn1,*opn2,*opn3);
            if(!temp_str.compare("-")) ir=new IR(IR::_SUB,*opn1,*opn2,*opn3);

            IRList.push_back(ir);
            /*cout<<symboltable.getSymbol(T->place)->name<<" = "
                <<symboltable.getSymbol(T->ptr[0]->place)->name<<" "+string(T->op)+" "
                <<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;*/
            break;
        case MulExp:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            T->place=newtemp(T->pretype,T->level,offset);
            offset+=4;

            symbol=symboltable.getSymbol(T->ptr[0]->place);
            opn1=new Opn(Opn::Var,symbol->name);
            opn1->level=symbol->level;
            opn1->offset=symbol->offset;
            if(T->pretype->getvalue().compare("int")==0)
                opn1->is_int=true;
            symbo2=symboltable.getSymbol(T->ptr[1]->place);
            opn2=new Opn(Opn::Var,symbo2->name);
            opn2->level=symbo2->level;
            opn2->offset=symbo2->offset;
            symbo3=symboltable.getSymbol(T->place);
            opn3=new Opn(Opn::Var,symbo3->name);
            opn3->level=symbo3->level;
            opn3->offset=symbo3->offset;

            temp_str=string(T->op);
            if(!temp_str.compare("*")) ir=new IR(IR::_MUL,*opn1,*opn2,*opn3);
            if(!temp_str.compare("/")) ir=new IR(IR::_DIV,*opn1,*opn2,*opn3);
            if(!temp_str.compare("%")) ir=new IR(IR::_MOD,*opn1,*opn2,*opn3);

            IRList.push_back(ir);
            /*cout<<symboltable.getSymbol(T->place)->name<<" = "
                <<symboltable.getSymbol(T->ptr[0]->place)->name<<" "+string(T->op)+" "
                <<symboltable.getSymbol(T->ptr[1]->place)->name<<endl;*/
            break;
        case LOrExp://逻辑或表达式
            T->Etrue=(T->Etrue.empty())?newLabel():T->Etrue;
            T->Efalse=(T->Efalse.empty())?newLabel():T->Efalse;
            T->ptr[0]->Etrue=T->Etrue;
            T->ptr[0]->Efalse=newLabel();
            T->ptr[1]->Etrue=T->Etrue;
            T->ptr[1]->Efalse=T->Efalse;
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            opn3=new Opn(Opn::Label,T->ptr[0]->Efalse);
            ir=new IR(IR::_LABEL,*opn3);
            IRList.push_back(ir);
            //cout<<T->ptr[0]->Efalse<<":"<<endl;
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
            opn3=new Opn(Opn::Label,T->ptr[0]->Etrue);
            ir=new IR(IR::_LABEL,*opn3);
            IRList.push_back(ir);
            //cout<<T->ptr[0]->Etrue<<":"<<endl;
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
            
            symbol=symboltable.getSymbol(T->ptr[0]->place);
            opn1=new Opn(Opn::Var,symbol->name);
            opn1->level=symbol->level;
            opn1->offset=symbol->offset;
            symbo2=symboltable.getSymbol(T->ptr[1]->place);
            opn2=new Opn(Opn::Var,symbo2->name);
            opn2->level=symbo2->level;
            opn2->offset=symbo2->offset;
            opn3=new Opn(Opn::Var,T->Etrue);

            temp_str=string(T->op);
            if(!temp_str.compare(">")) ir=new IR(IR::_JGT,*opn1,*opn2,*opn3);
            if(!temp_str.compare("<")) ir=new IR(IR::_JLT,*opn1,*opn2,*opn3);
            if(!temp_str.compare(">=")) ir=new IR(IR::_JGE,*opn1,*opn2,*opn3);
            if(!temp_str.compare("<=")) ir=new IR(IR::_JLE,*opn1,*opn2,*opn3);
            if(!temp_str.compare("==")) ir=new IR(IR::_JEQ,*opn1,*opn2,*opn3);
            if(!temp_str.compare("!=")) ir=new IR(IR::_JNE,*opn1,*opn2,*opn3);

            IRList.push_back(ir);
            /*cout<<"if "<<symboltable.getSymbol(T->ptr[0]->place)->name<<" "+string(T->op)+" "
                <<symboltable.getSymbol(T->ptr[1]->place)->name<<" goto "<<T->Etrue<<endl;*/

            opn3=new Opn(Opn::Label,T->Efalse);
            ir=new IR(IR::_GOTO,*opn3);
            IRList.push_back(ir);               
            //cout<<"goto "<<T->Efalse<<endl;
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
                opn3=new Opn(Opn::Label,T->ptr[0]->Etrue);
                ir=new IR(IR::_LABEL,*opn3);
                IRList.push_back(ir);
                //cout<<T->ptr[0]->Etrue<<":"<<endl;
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
                opn3=new Opn(Opn::Label,T->Snext);
                ir=new IR(IR::_GOTO,*opn3);
                IRList.push_back(ir);
                //cout<<"goto "<<T->Snext<<endl;
                opn3=new Opn(Opn::Label,T->ptr[0]->Efalse);
                ir=new IR(IR::_LABEL,*opn3);
                IRList.push_back(ir);
                //cout<<T->ptr[0]->Efalse<<":"<<endl;
                T->ptr[2]->Snext=T->Snext;
                genIR(T->ptr[2],symboltable);
            }
                opn3=new Opn(Opn::Label,T->Snext);
                ir=new IR(IR::_LABEL,*opn3);
                IRList.push_back(ir);
            //cout<<T->Snext<<":"<<endl;
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
                opn3=new Opn(Opn::Label,T->ptr[1]->Snext);
                ir=new IR(IR::_LABEL,*opn3);
                IRList.push_back(ir);
                //cout<<T->ptr[1]->Snext<<":"<<endl;
                genIR(T->ptr[0],symboltable);
                opn3=new Opn(Opn::Label,T->ptr[0]->Etrue);
                ir=new IR(IR::_LABEL,*opn3);
                IRList.push_back(ir);
                //cout<<T->ptr[0]->Etrue<<":"<<endl;
            }
            if(T->ptr[1]) 
            {
                T->ptr[1]->Sbreak=T->Sbreak;
                T->ptr[1]->Scontinue=T->Scontinue;
                genIR(T->ptr[1],symboltable);
                opn3=new Opn(Opn::Label,T->ptr[1]->Snext);
                ir=new IR(IR::_GOTO,*opn3);
                IRList.push_back(ir);
                //cout<<"goto "<<T->ptr[1]->Snext<<endl;
            }
                opn3=new Opn(Opn::Label,T->Snext);
                ir=new IR(IR::_LABEL,*opn3);
                IRList.push_back(ir);            
            //cout<<T->Snext<<":"<<endl;

            break;
        case RETURN:
            T->Snext=(T->Snext.empty())?newLabel():T->Snext;
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
            if(T->ptr[0]){
                symbol=symboltable.getSymbol(T->ptr[0]->place);
                opn3=new Opn(Opn::Var,symbol->name);
                ir=new IR(IR::_RET,*opn3);
                IRList.push_back(ir);
            }
                //cout<<"return "<<symboltable.getSymbol(T->ptr[0]->place)->name<<endl;
            else{
                opn3=new Opn();
                ir=new IR(IR::_RET,*opn3);
                IRList.push_back(ir);
            }
                //cout<<"return "<<endl;
            break;
        case CONTINUE:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
                opn3=new Opn(Opn::Label,T->Scontinue);
                ir=new IR(IR::_GOTO,*opn3);
                IRList.push_back(ir);            
            //cout<<"goto "<<T->Scontinue<<endl;
            break;
        case BREAK:
            if(T->ptr[0]) genIR(T->ptr[0],symboltable);
            if(T->ptr[1]) genIR(T->ptr[1],symboltable);
            if(T->ptr[2]) genIR(T->ptr[2],symboltable);
                opn3=new Opn(Opn::Label,T->Sbreak);
                ir=new IR(IR::_GOTO,*opn3);
                IRList.push_back(ir);
            //cout<<"goto "<<T->Sbreak<<endl;
            break;
        }
        }
        return;
    }
void IRBuilder::MIRprint()
{
    IR *ir;
    Opn *opn;
    int i;
    for (i = 0; i < IRList.size(); i++)
    {
        ir = IRList[i];
        switch (ir->op)
        {
        case IR::_VOID:       // 无用指令
                break;
        case IR::_FUNC:      // opn1 :
                cout<<"define function "<<ir->opn1.name<<endl;   
        case IR::_LABEL:      // opn1 :
                cout<<ir->opn1.name<<": "<<endl;
                break;
        case IR::_ALLOC:      // alloc opn1(变量名) : result(字节数)
                cout<<"\talloc "<<ir->opn1.name<<": "<<ir->result.imm_int<<endl;
                break;
        case IR::_ADD:        // result = opn1 + opn2
                cout<<"\t"<<ir->result.name<<" = ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm)
                    ir->opn1.is_int?(cout<<ir->opn1.imm_int):(cout<<ir->opn1.imm_float);
                cout<<" + ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm)
                    ir->opn2.is_int?(cout<<ir->opn2.imm_int):(cout<<ir->opn2.imm_float);
                cout<<endl;
                break;
        case IR::_SUB:        // result = opn1 - opn2
                cout<<"\t"<<ir->result.name<<" = ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm)
                    ir->opn1.is_int?(cout<<ir->opn1.imm_int):(cout<<ir->opn1.imm_float);
                cout<<" - ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm)
                    ir->opn2.is_int?(cout<<ir->opn2.imm_int):(cout<<ir->opn2.imm_float);
                cout<<endl;
                break;
        case IR::_MUL:        // result = opn1 * opn2
                cout<<"\t"<<ir->result.name<<" = ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm)
                    ir->opn1.is_int?(cout<<ir->opn1.imm_int):(cout<<ir->opn1.imm_float);
                cout<<" * ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm)
                    ir->opn2.is_int?(cout<<ir->opn2.imm_int):(cout<<ir->opn2.imm_float);
                cout<<endl;
                break;           
        case IR::_DIV:        // result = opn1 / opn2
                cout<<"\t"<<ir->result.name<<" = ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm)
                    ir->opn1.is_int?(cout<<ir->opn1.imm_int):(cout<<ir->opn1.imm_float);
                cout<<" / ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm)
                    ir->opn2.is_int?(cout<<ir->opn2.imm_int):(cout<<ir->opn2.imm_float);
                cout<<endl;
                break; 
        case IR::_MOD:        // result = opn1 % opn2
                cout<<"\t"<<ir->result.name<<" = ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm)
                    ir->opn1.is_int?(cout<<ir->opn1.imm_int):(cout<<ir->opn1.imm_float);
                cout<<" % ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm)
                    ir->opn2.is_int?(cout<<ir->opn2.imm_int):(cout<<ir->opn2.imm_float);
                cout<<endl;
                break; 
        case IR::_ADDR:
                cout<<"\t"<<ir->result.name<<" = &"<<ir->opn1.name<<endl;
                break;
        case IR::_ASSIGN:     // result = opn1
                if(ir->opn1.kind==Opn::Imm){
                    if(ir->opn1.is_int==1){
                        cout<<"\t"<<ir->result.name<<" = "<<ir->opn1.imm_int<<endl;;
                    }
                    else{
                        cout<<"\t"<<ir->result.name<<" = "<<ir->opn1.imm_float<<endl;
                    }
                }
                else if(ir->opn1.kind==Opn::Var){
                        cout<<"\t"<<ir->result.name<<" = "<<ir->opn1.name<<endl;
                }
                else if(ir->opn1.kind==Opn::Block){
                        cout<<"\t"<<ir->result.name<<" ={ ";
                        for(int i=0;i<ir->opn1.Block_args.size();i++){
                            opn=ir->opn1.Block_args[i];
                            if(opn->kind==Opn::Var)
                                cout<<opn->name<<" ";
                            else if(opn->kind==Opn::Imm){
                                if(opn->is_int) cout<<opn->imm_int<<" ";
                                else cout<<opn->imm_float<<" ";
                            }
                        }  
                        cout<<"}"<<endl;                    
                }
                break;
        case IR::_Arr_ASSIGN: // opn1[opn2]=result

                cout<<"\t"<<ir->opn1.name<<"[";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm)
                    if(ir->opn2.is_int) cout<<ir->opn2.imm_int;
                    else cout<<ir->opn2.imm_float;
                cout<<"] = ";
                if(ir->result.kind==Opn::Var)
                    cout<<ir->result.name;
                else if(ir->result.kind==Opn::Imm)
                    if(ir->result.is_int) cout<<ir->result.imm_int;
                    else cout<<ir->result.imm_float;
                cout<<endl;
                break;
        case IR::_ASSIGN_Arr: // opn1=opn2[result]
                if(ir->result.kind==Opn::Var)
                    cout<<"\t"<<ir->opn1.name<<" = "<<ir->opn2.name<<"["<<ir->result.name<<"]"<<endl;
                else if(ir->result.kind==Opn::Imm && ir->result.is_int)
                    cout<<"\t"<<ir->opn1.name<<" = "<<ir->opn2.name<<"["<<ir->result.imm_int<<"]"<<endl;
                else if(ir->result.kind==Opn::Imm && !ir->result.is_int)
                    cout<<"\t"<<ir->opn1.name<<" = "<<ir->opn2.name<<"["<<ir->result.imm_float<<"]"<<endl;
                break;
        case IR::_NOT:        // result = ! opn1
                if(ir->opn1.kind==Opn::Var)
                    cout<<"\t"<<ir->result.name<<"= !"<<ir->opn1.name<<endl;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<"\t"<<ir->result.name<<"= !"<<ir->opn1.imm_int<<endl;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<"\t"<<ir->result.name<<"= !"<<ir->opn1.imm_float<<endl;
                break;
        case IR::_POSI:       // result = + opn1
                if(ir->opn1.kind==Opn::Var)
                    cout<<"\t"<<ir->result.name<<"= +"<<ir->opn1.name<<endl;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<"\t"<<ir->result.name<<"= +"<<ir->opn1.imm_int<<endl;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<"\t"<<ir->result.name<<"= +"<<ir->opn1.imm_float<<endl;
                break;
        case IR::_NEGA:       // result = - opn1
                if(ir->opn1.kind==Opn::Var)
                    cout<<"\t"<<ir->result.name<<"= -"<<ir->opn1.name<<endl;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<"\t"<<ir->result.name<<"= -"<<ir->opn1.imm_int<<endl;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<"\t"<<ir->result.name<<"= -"<<ir->opn1.imm_float<<endl;
                break;
        case IR::_GOTO:       // goto opn1
                cout<<"\t"<<"goto "<<ir->opn1.name<<endl;
                break;           
        case IR::_JEQ:        // if opn1 == opn2 goto result
                cout<<"\t"<<"if ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<ir->opn1.imm_int;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<ir->opn1.imm_float;
                cout<<" == ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm && ir->opn2.is_int)
                    cout<<ir->opn2.imm_int;
                else if(ir->opn2.kind==Opn::Imm && !ir->opn2.is_int)
                    cout<<ir->opn2.imm_float;
                cout<<" goto "<<ir->result.name<<endl;
                break;
        case IR::_JNE:        // if opn1 != opn2 goto result
                cout<<"\t"<<"if ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<ir->opn1.imm_int;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<ir->opn1.imm_float;
                cout<<" != ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm && ir->opn2.is_int)
                    cout<<ir->opn2.imm_int;
                else if(ir->opn2.kind==Opn::Imm && !ir->opn2.is_int)
                    cout<<ir->opn2.imm_float;
                cout<<" goto "<<ir->result.name<<endl;
                break;
        case IR::_JLT:        // if opn1 < opn2 goto result
                cout<<"\t"<<"if ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<ir->opn1.imm_int;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<ir->opn1.imm_float;
                cout<<" < ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm && ir->opn2.is_int)
                    cout<<ir->opn2.imm_int;
                else if(ir->opn2.kind==Opn::Imm && !ir->opn2.is_int)
                    cout<<ir->opn2.imm_float;
                cout<<" goto "<<ir->result.name<<endl;
                break;
        case IR::_JGT:        // if opn1 > opn2 goto result
                cout<<"\t"<<"if ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<ir->opn1.imm_int;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<ir->opn1.imm_float;
                cout<<" > ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm && ir->opn2.is_int)
                    cout<<ir->opn2.imm_int;
                else if(ir->opn2.kind==Opn::Imm && !ir->opn2.is_int)
                    cout<<ir->opn2.imm_float;
                cout<<" goto "<<ir->result.name<<endl;
                break;
        case IR::_JLE:        // if opn1 <= opn2 goto result
                cout<<"\t"<<"if ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<ir->opn1.imm_int;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<ir->opn1.imm_float;
                cout<<" <= ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm && ir->opn2.is_int)
                    cout<<ir->opn2.imm_int;
                else if(ir->opn2.kind==Opn::Imm && !ir->opn2.is_int)
                    cout<<ir->opn2.imm_float;
                cout<<" goto "<<ir->result.name<<endl;
                break;
        case IR::_JGE:        // if opn1 >= opn2 goto result
                cout<<"\t"<<"if ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<ir->opn1.imm_int;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<ir->opn1.imm_float;
                cout<<" >= ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm && ir->opn2.is_int)
                    cout<<ir->opn2.imm_int;
                else if(ir->opn2.kind==Opn::Imm && !ir->opn2.is_int)
                    cout<<ir->opn2.imm_float;
                cout<<" goto "<<ir->result.name<<endl;
                break;
        case IR::_PARAM:      // param opn1
                if(ir->opn1.kind==Opn::Var)
                    cout<<"\t"<<"param "<<ir->opn1.name<<endl;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<"\t"<<"param "<<ir->opn1.imm_int<<endl;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<"\t"<<"param "<<ir->opn1.imm_float<<endl;
                break;
        case IR::_CALL:       // [result =] call opn1(函数) , opn2(参数个数)
                if(ir->result.kind==Opn::Null){
                    cout<<"\t"<<"call "<<ir->opn1.name<<", "<<ir->opn2.imm_int<<endl;
                }
                else{
                    cout<<"\t"<<ir->result.name<<" =call "<<ir->opn1.name<<", "<<ir->opn2.imm_int<<endl;
                }
                break;
        case IR::_RET:        // return [opn1]
                if(ir->opn1.kind==Opn::Null){
                    cout<<"\t"<<"return "<<endl;
                }
                else if(ir->opn1.kind==Opn::Var){
                    cout<<"\t"<<"return "<<ir->opn1.name<<endl;
                }
                else if(ir->opn1.kind==Opn::Imm){
                    if(ir->opn1.is_int)
                    cout<<"\t"<<"return "<<ir->opn1.imm_int<<endl;
                    else
                    cout<<"\t"<<"return "<<ir->opn1.imm_float<<endl;
                }
                break;
        }
    }
}

