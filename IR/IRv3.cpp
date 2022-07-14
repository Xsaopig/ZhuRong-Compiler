#include "IR.hpp"
//@代表全局标识符（函数，全局变量）；
//%代表局部标识符（寄存器名称也就是局部变量，类型）。
void IRBuilder::Build(struct node *T)
{
    genIR(T);
}

struct codenode *IRBuilder::codegen(enum op_kind kind, vector<Opn *> &opns)
{
    struct codenode *p = new struct codenode();
    p->op = kind;
    p->opns = opns;
    p->pre = p->next = p;
    return p;
}

struct codenode *genBr(string label)
{
    struct codenode *p = new struct codenode();
    //待补充
    return p;
}

void Exp(struct node *T){
    //待补充
}

struct codenode *genLabel(string label)
{
    struct codenode *p = new struct codenode();
    p->op = LABEL;
    Opn *opn = new Opn();
    opn->id = label;
    vector<Opn *> opns = {opn};
    p->opns = opns;
    p->next = p->pre = p;
    return p;
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

struct node *mywhile;

void IRBuilder::genIR(struct node *T)
{
    Symbol mysymbol;
    Symbol new_symbol;
    struct node *cur, *T0;
    BasicType *son;
    int record;
    enum op_kind t_kind;
    int i;
    int index;
    Symbol *symbol;
    Opn *opn;
    string opn_type, ty;
    vector<Opn *> opns, new_opns;
    if (T)
    {
        switch (T->kind)
        {
        case Root:
            genIR(T->ptr[0]);
            break;
        case CompUnit:
            T->ptr[0]->level = 0;
            if (T->ptr[1])
                T->ptr[1]->level = 0;
            genIR(T->ptr[0]);
            T->code = T->ptr[0]->code;
            if (T->ptr[1])
            {
                T->ptr[1]->level = 0;
                genIR(T->ptr[1]);
                T->code = merge(2, T->code, T->ptr[1]->code);
            }
            break;
        case FuncDef:
            record = no; //记录进函数前的局部值
            no = 0;
            mysymbol.name = string(T->type_id);
            mysymbol.level = T->level;
            // if(T->ptr[0]) mysymbol.type=T->ptr[0]->type;//类型有误，VOID未加入，后续调整
            son = static_cast<BasicType *>(T->pretype);
            mysymbol.types = son->getvalue();

            i = 0;
            T0 = T->ptr[1];
            while (T0)
            {
                i++;
                if (T0->ptr[0]->kind == FuncFParams)
                    T0 = T0->ptr[0];
                else
                    T0 = NULL;
            }
            mysymbol.paramnum = i;
            mysymbol.flag = 'F';
            T->place = symboltable.Push(mysymbol) - 1; //函数名入表
            symboltable.Push_index();

            //接下来处理T->code
            //在LLVM IR中，函数定义形式如：define i32 @main
            // i32代表int返回值类型，@在LLVM IR中是全局符号的意思
            opn_type = (!mysymbol.types.compare("int")) ? "i32" : mysymbol.types; // int对应的是i32，float和void不变
            opn = new Opn(IDENT, opn_type, T->level, T->place);                   //两个类型、层号、在符号表中的位置
            opn->id = mysymbol.name;                                              //操作数名就是函数名
            opns.push_back(opn);                                                  //函数定义操作只有一个操作数
            T->code = codegen(DEFINE, opns);

            //计算子节点的level（继承属性）
            if (T->ptr[0])
                T->ptr[0]->level = 0; //函数类型结点还在第0层
            if (T->ptr[1])
                T->ptr[1]->level = 1; //参数在第1层
            T->ptr[2]->level = 1;     //函数体在第1层

            genIR(T->ptr[1]);            //进入函数参数
            opn_type = newlabel();       //这一句和下面一句不要合成一句，会报错（奇怪）
            T->ptr[2]->Snext = opn_type; // T->Snext
            no = no + 1;
            genIR(T->ptr[2]); //函数体

            T->code = merge(4, T->code, T->ptr[1] ? T->ptr[1]->code : NULL, T->ptr[2]->code, genLabel(T->ptr[2]->Snext));

            symboltable.Pop_until(symboltable.Pop_index());

            no = record;
            break;
        case FuncFParams:
            T->ptr[0]->level = 1;

            genIR(T->ptr[0]);
            if (T->ptr[1])
            {
                T->ptr[1]->level = 1;
                genIR(T->ptr[1]);
                T->code = merge(2, T->code, T->ptr[1]->code);
            }

            break;
        case FuncFParam:
            T->ptr[0]->level = 1;
            if (T->ptr[1])
                T->ptr[1]->level = 1;

            mysymbol.name = string(T->type_id);
            mysymbol.level = 1;
            // mysymbol.type=T->ptr[0]->type;//类型后续调整
            son = static_cast<BasicType *>(T->ptr[0]->pretype);
            mysymbol.types = son->getvalue();
            mysymbol.flag = 'P';

            if (T->ptr[1])
            {
                int i = 1;
                cur = T->ptr[1];
                while (cur->ptr[0])
                {
                    i++;
                    cur = cur->ptr[0];
                }

                for (int n = 0; n < i; n++)
                    mysymbol.types += string("[]"); //数组参数的类型[]记在types
            }

            T->place = symboltable.Push(mysymbol) - 1;
            symboltable.Push_index();

            //接下来处理T->code
            //在LLVM IR中，参数定义形式如：i32 %0,%1...block从+1即%3开始
            opn_type = (!mysymbol.types.compare("int")) ? "i32" : mysymbol.types; // int对应的是i32，float和void不变
            opn = new Opn(IDENT, opn_type, T->level, T->place);                   //两个类型、层号、在符号表中的位置
            opn->id = newAlias();
            opns.push_back(opn);
            T->code = codegen(VAR, opns);

            break;
        case FuncFParamArray:
            break;
        case BType:

            break;
        case Block:

            // lev=lev+1;

            symboltable.Push_index();
            if (T->ptr[0])
            {
                opn_type = newlabel();
                T->ptr[0]->Snext = opn_type;
                genIR(T->ptr[0]);
                T->code = T->ptr[0]->code;
            }

            i = symboltable.Pop_index();
            symboltable.Pop_until(i);
            // lev=lev-1;
            break;
        case BlockItems:
            genIR(T->ptr[0]);
            T->code = T->ptr[0]->code;
            if (T->ptr[1])
            {
                genIR(T->ptr[1]);
                T->code = merge(2, T->code, T->ptr[1]->code);
            }
            break;
        case Decl:
            genIR(T->ptr[0]);
            T->code = T->ptr[0]->code;
            break;
        case ConstDecl:

            if (T->ptr[0]->kind == ConstDecl)
                genIR(T->ptr[0]);

            mysymbol.name = string(T->ptr[1]->ptr[0]->type_id);
            mysymbol.level = T->level;
            // mysymbol.type=T->type;//类型后续调整
            son = static_cast<BasicType *>(T->pretype);
            mysymbol.types = son->getvalue();
            mysymbol.flag = 'C';

            cur = T->ptr[1]->ptr[0];
            i = 0;
            while (cur->ptr[0])
            {
                i++;
                cur = cur->ptr[0];
            }
            for (int n = 0; n < i; n++)
                mysymbol.name += string("[]"); //数组的类型[]

            T->place = symboltable.Push(mysymbol) - 1; //常量（变量）入表

            opn_type = (!mysymbol.types.compare("int")) ? "i32" : mysymbol.types; // int对应的是i32，float和void不变
            opn = new Opn(CONSTANT, opn_type, T->level, T->place);                //两个类型、层号、在符号表中的位置
            if (T->level == 0)
            {
                opn->id = newGloabl();
            }
            else
            {
                opn->id = newAlias();
            }
            opns.push_back(opn);
            T->code = codegen(CONSTANT, opns);
            genIR(T->ptr[1]);
            T->code = merge(2, T->code, T->ptr[1]->code);
            break;
        case VarDecl:

            if (T->ptr[0]->kind == VarDecl)
                genIR(T->ptr[0]);

            mysymbol.name = string(T->ptr[1]->ptr[0]->type_id);
            mysymbol.level = T->level;
            // mysymbol.type=T->type;//类型后续调整
            son = static_cast<BasicType *>(T->pretype);
            mysymbol.types = son->getvalue();

            mysymbol.flag = 'V';
            cur = T->ptr[1]->ptr[0];
            i = 0;
            while (cur->ptr[0])
            {
                i++;
                cur = cur->ptr[0];
            }
            for (int n = 0; n < i; n++)
                mysymbol.types += string("[]"); //数组的类型[]记在types
            symboltable.Push(mysymbol);         //常量（变量）入表

            opn_type = (!mysymbol.types.compare("int")) ? "i32" : mysymbol.types; // int对应的是i32，float和void不变
            opn = new Opn(VAR, opn_type, T->level, T->place);                     //两个类型、层号、在符号表中的位置
            if (T->level == 0)
            {
                opn->id = newGloabl();
            }
            else
            {
                opn->id = newAlias();
            }
            opns.push_back(opn);
            T->code = codegen(VAR, opns);
            genIR(T->ptr[1]);
            T->code = merge(2, T->code, T->ptr[1]->code);
            break;
        case ConstDef:
            genIR(T->ptr[0]);
            genIR(T->ptr[1]);
            break;
        case VarDef:
            genIR(T->ptr[0]);

            if (T->ptr[1])
            {
                genIR(T->ptr[1]);
            }
            break;
        case Idents:
            if (T->ptr[0])
            {
                genIR(T->ptr[0]);
            }
            if (T->ptr[1])
            {
                genIR(T->ptr[1]);
            }
            break;
        case InitVals:
            if (T->ptr[0])
            {
                genIR(T->ptr[0]);
            }
            if (T->ptr[1])
            {
                genIR(T->ptr[1]);
            }
            break;
        case InitVal:
            if (T->ptr[0])
            {
                genIR(T->ptr[0]);
            }
            break;
        case ASSIGN:

            index = symboltable.Search(T->ptr[0]->type_id); //找到符号的索引
            genIR(T->ptr[1]);                               // assign的右边最终遍历都到exp

            ty = T->ptr[1]->pretype->getvalue();
            if (index > -1)
            {
                symbol = symboltable.getSymbol(index);
                opn = new Opn(VAR, symbol->types, T->level, T->ptr[1]->offset);
                if (symbol->types == "iN")
                {
                    opn->const_int = T->ptr[1]->type_int;
                }
                if (symbol->types == "float")
                {
                    opn->const_float = T->ptr[1]->type_float;
                }
                opns.push_back(opn);
                opn = new Opn(VAR, symbol->types, T->level, T->ptr[0]->offset);
                opn->id = symbol->name;
                opns.push_back(opn);
                T->code = codegen(STORE, opns);
            }
            else
                break; //是否需要报错不存在该符号

        case LVal:
            index = symboltable.Search(T->type_id); //找到符号的索引
            symbol = symboltable.getSymbol(index);

            opn = new Opn(VAR, symbol->types, T->level, T->offset);
            //表达式含变量需要先load到一个新变量再用new来计算
            opn->id = newAlias();
            new_opns.push_back(opn);

            T->code = merge(2, T->code, codegen(LOAD, new_opns));

            opn->id = T->type_id;
            opns.push_back(opn);

            break;
        case Number: //单个数值需要处理吗？测试案例有些只有一个整数
            opn_type = T->pretype->getvalue();
            opn = new Opn(Number, opn_type, T->level, T->offset);
            if (opn_type == "iN")
            {
                opn->const_int = T->type_int;
            }
            if (opn_type == "float")
            {
                opn->const_float = T->type_float;
            }
            opns.push_back(opn);
            break;
        case FuncCall:   //函数返回值类型，没有属性pretype，待实现

            opn=new Opn(FUNCTION,T->pretype->getvalue(),T->level,T->offset);
            opn->id="@"+string(T->type_id);
            opns.push_back(opn);
            if(T->ptr[0]){
                T->code=merge(2,codegen(CALL,opns),T->ptr[0]->code);
            }
            else T->code=codegen(CALL,opns);
            break;
        case FuncRParams:  //最终指向LVal，opns在LVal填入
            genIR(T->ptr[0]);
            if (T->ptr[1])
            {
                genIR(T->ptr[1]);
            }
            T->code=codegen(VAR,opns);
            break;

        case AddExp:
        case MulExp:
            T->code = NULL;

            //产生code
            genIR(T->ptr[0]);

            if(!strcmp(T->op,"+"))
                t_kind=_ADD;
            else if(!strcmp(T->op,"-"))
                t_kind=_SUB;
            else if(!strcmp(T->op,"*"))
                t_kind=_MUL;
            else if(!strcmp(T->op,"/"))
                t_kind=_DIV;
            else if(!strcmp(T->op,"%"))
                t_kind=_MOD;
                
            //处理
            genIR(T->ptr[1]);

            T->code = codegen(t_kind, opns);

            T->code = merge(3, T->ptr[0]->code, T->code, T->ptr[1]->code);


        case LOrExp:
        case LAndExp:
        case EqExp:
        case RelExp:
            if(!strcmp(T->op,"||"))
            {
                t_kind = _OR;
                genIR(T->ptr[0]);
                genIR(T->ptr[1]);
                // codegen(SGE,new_opns)只是记录操作符，不存储操作数
                //   %4 = icmp sge i32 %3, 10
                T->code = merge(3, codegen(t_kind, new_opns), T->ptr[0]->code, T->ptr[1]->code);

                /*返回值记录
                opn = new Opn(VAR, "i1", T->level, T->offset);
                opn->id=newAlias();
                */
            }
            else if(!strcmp(T->op,"&&"))
            {   
                t_kind = _AND;
                genIR(T->ptr[0]);
                genIR(T->ptr[1]);
                // codegen(SGE,new_opns)只是记录操作符，不存储操作数
                //   %4 = icmp sge i32 %3, 10
                T->code = merge(3, codegen(t_kind, new_opns), T->ptr[0]->code, T->ptr[1]->code);

                /*返回值记录
                opn = new Opn(VAR, "i1", T->level, T->offset);
                opn->id=newAlias();
                */
            }
            else if(!strcmp(T->op,"=="))
            {
                t_kind = _EQ;
                genIR(T->ptr[0]);
                genIR(T->ptr[1]);
                // codegen(SGE,new_opns)只是记录操作符，不存储操作数
                //   %4 = icmp sge i32 %3, 10
                T->code = merge(3, codegen(t_kind, new_opns), T->ptr[0]->code, T->ptr[1]->code);

                /*返回值记录
                opn = new Opn(VAR, "i1", T->level, T->offset);
                opn->id=newAlias();
                */
            }
            else if(!strcmp(T->op,"!="))
            {
                t_kind = _NE;
                genIR(T->ptr[0]);
                genIR(T->ptr[1]);
                // codegen(SGE,new_opns)只是记录操作符，不存储操作数
                //   %4 = icmp sge i32 %3, 10
                T->code = merge(3, codegen(t_kind, new_opns), T->ptr[0]->code, T->ptr[1]->code);

                /*返回值记录
                opn = new Opn(VAR, "i1", T->level, T->offset);
                opn->id=newAlias();
                */
            }
            else if(!strcmp(T->op,"<"))
            {
                t_kind = SLT;
                genIR(T->ptr[0]);
                genIR(T->ptr[1]);
                // codegen(SGE,new_opns)只是记录操作符，不存储操作数
                //   %4 = icmp sge i32 %3, 10
                T->code = merge(3, codegen(t_kind, new_opns), T->ptr[0]->code, T->ptr[1]->code);

                /*返回值记录
                opn = new Opn(VAR, "i1", T->level, T->offset);
                opn->id=newAlias();
                */
            }
            else if(!strcmp(T->op,">"))
            {
                t_kind = SGT;
                genIR(T->ptr[0]);
                genIR(T->ptr[1]);
                // codegen(SGE,new_opns)只是记录操作符，不存储操作数
                //   %4 = icmp sge i32 %3, 10
                T->code = merge(3, codegen(t_kind, new_opns), T->ptr[0]->code, T->ptr[1]->code);

                /*返回值记录
                opn = new Opn(VAR, "i1", T->level, T->offset);
                opn->id=newAlias();
                */
            }
            else if(!strcmp(T->op,"<="))
            {
                t_kind = SLE;
                genIR(T->ptr[0]);
                genIR(T->ptr[1]);
                // codegen(SGE,new_opns)只是记录操作符，不存储操作数
                //   %4 = icmp sge i32 %3, 10
                T->code = merge(3, codegen(t_kind, new_opns), T->ptr[0]->code, T->ptr[1]->code);

                /*返回值记录
                opn = new Opn(VAR, "i1", T->level, T->offset);
                opn->id=newAlias();
                */
            }
            else if(!strcmp(T->op,">="))
            {
                t_kind = SGE;
                genIR(T->ptr[0]);
                genIR(T->ptr[1]);
                // codegen(SGE,new_opns)只是记录操作符，不存储操作数
                //   %4 = icmp sge i32 %3, 10
                T->code = merge(3, codegen(t_kind, new_opns), T->ptr[0]->code, T->ptr[1]->code);

                /*返回值记录
                opn = new Opn(VAR, "i1", T->level, T->offset);
                opn->id=newAlias();
                */
            }
            break;
        case UnaryExp: //+，-，！仅出现在条件表达式  仅cond会到达  未实现
            // +     直接load出来， 与0比较   not equal 0？1:0
            // -    用0减操作数，与0  ne 比较
            // ！   br条件跳转时，改变跳出label实现
             break;
        case IF:
            opn_type = newlabel();
            T->Snext = opn_type;
            if (T->ptr[2])
            {
                T->ptr[0]->Etrue = newlabel();
                T->ptr[0]->Efalse = newlabel();
                genIR(T->ptr[0]);

                T->ptr[1]->Snext = T->Snext;
                genIR(T->ptr[1]);
                T->ptr[2]->Snext = T->Snext;
                genIR(T->ptr[2]);

                // br i1 %4, label %5, label %6=codegen(BR)
                opn = new Opn(LABEL, T->ptr[0]->Etrue, T->level, T->offset);
                opns.push_back(opn);
                opn = new Opn(LABEL, T->ptr[0]->Efalse, T->level, T->offset);
                opns.push_back(opn);

                T->code = merge(7, T->ptr[0]->code, codegen(BR, opns), genLabel(T->ptr[0]->Etrue), T->ptr[1]->code, genLabel(T->ptr[0]->Efalse), T->ptr[2]->code, genLabel(T->Snext));
            }
            else
            {
                T->ptr[0]->Etrue = newlabel();
                T->ptr[0]->Efalse = T->Snext;
                genIR(T->ptr[0]);
                T->ptr[1]->Snext = T->Snext;
                genIR(T->ptr[1]);

                // br i1 %4, label %5, label %6=codegen(BR)
                opn = new Opn(LABEL, T->ptr[0]->Etrue, T->level, T->offset);
                opns.push_back(opn);
                opn = new Opn(LABEL, T->Snext, T->level, T->offset);
                opns.push_back(opn);
                T->code = merge(5, T->ptr[0]->code, codegen(BR, opns), genLabel(T->ptr[0]->Etrue), T->ptr[1]->code, genLabel(T->Snext));
            }
            break;
        case WHILE: // while的中间代码，需要产生多次分支
/*  br label %3
3:                                                ; preds = %6, %0
  %4 = load i32, i32* %2, align 4
  %5 = icmp sge i32 %4, 0
  br i1 %5, label %6, label %9
6:                                                ; preds = %3
  %7 = load i32, i32* %2, align 4
  %8 = add nsw i32 %7, -1
  store i32 %8, i32* %2, align 4
  br label %3
9:                                                ; preds = %3
  ret i32 0*/
            mywhile = T;
            T->Snext = newlabel();
            T->ptr[0]->Etrue = newlabel();
            T->ptr[0]->Efalse = T->Snext;
            genIR(T->ptr[0]);
            T->ptr[1]->Snext = newlabel();
            genIR(T->ptr[1]);
            opn = new Opn(LABEL, T->ptr[1]->Snext, T->level, T->offset);
            opns.push_back(opn);
            T->code = merge(7, codegen(BR, opns),genLabel(T->ptr[1]->Snext), T->ptr[0]->code,
                            genLabel(T->ptr[0]->Etrue), T->ptr[1]->code, codegen(BR, opns), genLabel(T->Snext));
            mywhile = NULL;
            break;

        case RETURN:
            if (T->ptr[0])
            {
                Exp(T->ptr[0]);
                if (T->ptr[0]->kind == Number)
                {
                    opn_type = (T->ptr[0]->type == INT) ? "i32" : mysymbol.types; // int对应的是i32，float和void不变
                    opn->const_int = T->ptr[0]->type_int;
                }
                else
                {
                    opn->kind = IDENT;
                    opn->id = newAlias();
                }
                opns.push_back(opn);
                T->code = merge(2, T->ptr[0]->code, codegen(RET, opns));
            }
            else
            {
                opn->kind = 0;
                opns.push_back(opn);
                T->code = codegen(RET, opns);
            }
            break;
        case CONTINUE:
            if (mywhile)
            {
                T->code = genBr(mywhile->ptr[1]->Snext);
            }
            break;
        case BREAK:
            if (mywhile)
            {
                T->code = genBr(mywhile->Snext);
            }
            break;
        }
    }
}
