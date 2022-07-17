#include "ast.hpp"

void ASTBuilder::Build()
{
    yyparse();
    struct node *root=ast.getroot();
     ast.printAST(root,0,0);
    // printf("源程序共有%d行代码\n",yylineno);//由于.l文件中有%option yylineno，所以yylineno在yyflexlexer中是自动管理的，遇到换行就+1
    ast.calAttr(root,symboltable);
    cout<<"计算属性结束"<<endl;
    // symboltable.reset();
    // ast.ASTtoSymtab(root,symboltable);
}

struct node *mknode(int kind, struct node *first, struct node *second, struct node *third, int pos) {
    struct node *T = (struct node *)malloc(sizeof(struct node));
    T->kind = (enum node_kind)kind;
    T->ptr[0] = first;
    T->ptr[1] = second;
    T->ptr[2] = third;
    T->pos = pos;
    return T;
}

struct node* AST::setroot(struct node *root){
    this->root=root;
    return this->root;
}

//deep为0的时候意思是这一个层次的最后一个前面要置为"`"而不是"|",last数组是记录某个层次是否结束
void AST::printAST(struct node *T, int indent, int deep) {
    if(T) {
        switch(T->kind) {
        case Root:
            for (int i = 0; i < 50; i++)
                last[i] = 0;
            printf("CompUnit\n");
            printAST(T->ptr[0], indent, 0);
            break;
        case CompUnit:
            for (int i = 0; i < 2; i++) {
                if(T->ptr[i] && T->ptr[i]->kind==CompUnit)
                    printAST(T->ptr[i], indent, deep + 1);//根据我的语法分析不得不这样做，这一可以只让最后一个的deep为0，看语法分析应该能看懂，后面还有很多种这样的
                else
                    printAST(T->ptr[i], indent, deep);
            }
            break;
        case FuncDef:
            //这一段就是负责弄当前层次之前的所有层次，看是不是需要补一个"|"
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            //判断是不是当前层次的最后一个
            if(deep==0) {
                printf("`--FuncDef ");
                last[indent] = 1;
            }
            else {
                printf("|--FuncDef ");
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            if(T->ptr[0])
                printAST(T->ptr[0], 0, 0);//indent和deep不重要
            else
                printf("void");
            printf(" ");
            printf("%s()\n", T->type_id);
            printAST(T->ptr[1], indent + 1, 0);
            printAST(T->ptr[2], indent + 1, 0);
            break;
        case FuncFParams:
            for (int i = 0; i < 2; i++) {
                if(T->ptr[i] && T->ptr[i]->kind==FuncFParams)
                    printAST(T->ptr[i], indent, deep + 1);//根据我的语法分析不得不这样做，这一可以只让最后一个的deep为0，看语法分析应该能看懂，后面还有很多种这样的
                else
                    printAST(T->ptr[i], indent, deep);
            }
            break;
        case FuncFParam:
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if(deep==0) {
                printf("`--FuncFParam ");
                last[indent] = 1;
            }
            else {
                printf("|--FuncFParam ");
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            printAST(T->ptr[0], 0, 0);
            printf(" %s", T->type_id);
            if(T->ptr[1]) {
                printAST(T->ptr[1], indent, 0);
            }
            else
                printf("\n");
            break;
        case FuncFParamArray:
            if(deep==0) {
                int i = 1;
                struct node *cur = T;
                while(cur->ptr[0]) {
                    i++;
                    cur = cur->ptr[0];
                }
                for (int n = 0; n < i; n++)
                    printf("[]");
                printf("\n");
            }
            printAST(T->ptr[0], indent, deep + 1);
            printAST(T->ptr[1], indent + 1, deep);
            break;
        case BType:
            if(T->type==INT)
                printf("int");
            else if(T->type==FLOAT)
                printf("float");
            break;
        case Block:
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if(deep==0) {
                printf("`--Block\n");
                last[indent] = 1;
            }
            else {
                printf("|--Block\n");
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            printAST(T->ptr[0], indent + 1, 0);
            break;
        case BlockItems:
            for (int i = 0; i < 2; i++) {
                if(T->ptr[i] && T->ptr[i]->kind==BlockItems)
                    printAST(T->ptr[i], indent, deep + 1);
                else
                    printAST(T->ptr[i], indent, deep);
            }
            break;
        case Decl:
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if(deep==0) {
                printf("`--Decl\n");
                last[indent] = 1;
            }
            else {
                printf("|--Decl\n");
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            printAST(T->ptr[0], indent + 1, 0);
            break;
        case ConstDecl:
            if (T->ptr[0]->kind == ConstDecl)
                printAST(T->ptr[0], indent, deep + 1);
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if(deep==0) {
                printf("`--ConstDecl ");
                last[indent] = 1;
            }
            else {
                printf("|--ConstDecl ");
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            if (T->type == INT)
                printf("int ");
            else if (T->type == FLOAT)
                printf("float ");
            printAST(T->ptr[1], indent, 0);
            break;
        case VarDecl:
            //因为vardecl和之前的compunit不太一样，所以需要先进递归，而不是后进递归
            if (T->ptr[0]->kind == VarDecl)
                printAST(T->ptr[0], indent, deep + 1);
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if(deep==0) {
                printf("`--VarDecl ");
                last[indent] = 1;
            }
            else {
                printf("|--VarDecl ");
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            if (T->type == INT)
                printf("int ");
            else if (T->type == FLOAT)
                printf("float ");
            printAST(T->ptr[1], indent, 0);
            break;
        case ConstDef:
        case VarDef:
            printAST(T->ptr[0], indent, deep);
            printAST(T->ptr[1], indent + 1, 0);
            break;
        case Idents:
            if(deep==0) {
                int i = 0;
                struct node *cur = T;
                while (cur->ptr[0]) {
                    cur = cur->ptr[0];
                    i++;
                }
                printf("%s", T->type_id);
                for (int n = 0; n < i; n++)
                    printf("[]");
                printf("\n");
            }
            if(T->ptr[0]) {
                printAST(T->ptr[0], indent, deep + 1);
            }
            if(T->ptr[1]) {
                printAST(T->ptr[1], indent + 1, deep);
            }
            break;
        case InitVals:
            for (int i = 0; i < 2; i++) {
                if(T->ptr[i] && T->ptr[i]->kind==InitVals)
                    printAST(T->ptr[i], indent, deep + 1);//根据我的语法分析不得不这样做，这一可以只让最后一个的deep为0，看语法分析应该能看懂，后面还有很多种这样的
                else
                    printAST(T->ptr[i], indent, deep);
            }
            break;
        case InitVal:
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if(deep==0) {
                printf("`--InitVal\n");
                last[indent] = 1;
            }
            else {
                printf("|--InitVal\n");
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            printAST(T->ptr[0], indent + 1, 0);
            break;
        case ASSIGN:
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if(deep==0) {
                printf("`--=\n");//暂时不管类型
                last[indent] = 1;
            }
            else {
                printf("|--=\n");//暂时不管类型
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            printAST(T->ptr[0], indent + 1, 1);
            printAST(T->ptr[1], indent + 1, 0);
            break;
        case LVal:
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if(deep==0) {
                printf("`--");
                last[indent] = 1;
            }
            else {
                printf("|--");
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            if(T->ptr[0]) {
                struct node *New_Idents = mknode(Idents, T->ptr[0], T->ptr[1], NULL, T->pos);
                strcpy(New_Idents->type_id, T->type_id);
                struct node *cur = New_Idents;
                while(cur->ptr[0]) {
                    cur->ptr[0]->kind = Idents;
                    cur = cur->ptr[0];
                }
                printAST(New_Idents, indent, 0);
                cur = New_Idents;
                while(cur->ptr[0]) {
                    cur->ptr[0]->kind = LVal;
                    cur = cur->ptr[0];
                }
                // free(cur);
            }
            else 
                printf("%s\n", T->type_id);//暂时不管类型和数组
            break;
        case Number:
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if(deep==0) {
                printf("`");
                last[indent] = 1;
            }
            else {
                printf("|");
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            if (T->type == INT)
                printf("--%d INTCONST\n", T->type_int);
            else if (T->type == FLOAT)
                printf("--%f FLOATCONST\n", T->type_float);
            break;
        case FuncCall:
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if(deep==0) {
                printf("`--FuncCall ");
                last[indent] = 1;
            }
            else {
                printf("|--FuncCall ");
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            printf("%s()\n", T->type_id);
            printAST(T->ptr[0], indent + 1, 0);
            break;
        case FuncRParams:
            for (int i = 0; i < 2; i++) {
                if(T->ptr[i] && T->ptr[i]->kind==FuncRParams)
                    printAST(T->ptr[i], indent, deep + 1);
                else
                    printAST(T->ptr[i], indent, deep);
            }
            break;
        case UnaryExp:
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if(deep==0) {
                printf("`--%s\n", T->op); //暂时不管类型
                last[indent] = 1;
            }
            else {
                printf("|--%s\n", T->op); //暂时不管类型
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            printAST(T->ptr[0], indent + 1, 0);
            break;
        case AddExp:
        case MulExp:
        case LOrExp:
        case LAndExp:
        case EqExp:
        case RelExp:
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if(deep==0) {
                printf("`--%s\n", T->op); //暂时不管类型
                last[indent] = 1;
            }
            else {
                printf("|--%s\n", T->op); //暂时不管类型
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            printAST(T->ptr[0], indent + 1, 1);
            printAST(T->ptr[1], indent + 1, 0);
            break;
        case RETURN:
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if(deep==0) {
                printf("`--return\n");
                last[indent] = 1;
            }
            else {
                printf("|--return\n");
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            printAST(T->ptr[0], indent + 1, 0);
            break;
        case IF:
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if (deep == 0 && T->ptr[2] == NULL) {
                printf("`--if\n");
                last[indent] = 1;
            }
            else {
                printf("|--if\n");
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            printAST(T->ptr[0], indent + 1, 0);
            printAST(T->ptr[1], indent + 1, 0);
            if(T->ptr[2]) {
                for (int i = 0; i < indent; i++)
                    if(last[i])
                        printf("   ");
                    else
                        printf("|  ");
                if(deep==0) {
                    printf("`--else\n");
                    last[indent] = 1;
                }
                else {
                    printf("|--else\n");
                    for (int i = indent; i < 50 && last[i]; i++)
                        last[i] = 0;
                }
                printAST(T->ptr[2], indent + 1, 0);
            }
            break;
        case WHILE:
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if (deep == 0) {
                printf("`--while\n");
                last[indent] = 1;
            }
            else {
                printf("|--while\n");
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            printAST(T->ptr[0], indent + 1, 0);
            printAST(T->ptr[1], indent + 1, 0);
            break;
        case CONTINUE:
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if (deep == 0) {
                printf("`--continue\n");
                last[indent] = 1;
            }
            else {
                printf("|--continue\n");
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            break;
        case BREAK:
            for (int i = 0; i < indent; i++)
                if(last[i])
                    printf("   ");
                else
                    printf("|  ");
            if (deep == 0) {
                printf("`--break\n");
                last[indent] = 1;
            }
            else {
                printf("|--break\n");
                for (int i = indent; i < 50 && last[i]; i++)
                    last[i] = 0;
            }
            break;
        }
    }
}

//AST转化为符号表
void AST::ASTtoSymtab(struct node *T,Symboltable &symboltable) {
    Symbol mysymbol;
    struct node *cur,*T0;
    BasicType* son;
    int i;
    if(T) {
        switch(T->kind) {
        case Root:
            ASTtoSymtab(T->ptr[0],symboltable);
            break;
        case CompUnit:
            ASTtoSymtab(T->ptr[0],symboltable);
            ASTtoSymtab(T->ptr[1],symboltable);
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
            symboltable.Push(mysymbol);  //函数名入表
            symboltable.Push_index();
            
            
            
            ASTtoSymtab(T->ptr[1],symboltable);   //进入函数参数
            ASTtoSymtab(T->ptr[2],symboltable);
            symboltable.Pop_until(symboltable.Pop_index());
            break;
        case FuncFParams:
            ASTtoSymtab(T->ptr[0],symboltable);
            ASTtoSymtab(T->ptr[1],symboltable);
            
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
            
            // lev=lev+1;  
            
            symboltable.Push_index();
            ASTtoSymtab(T->ptr[0],symboltable);
            i=symboltable.Pop_index();
            symboltable.Pop_until(i);       
            // lev=lev-1;
            break;
        case BlockItems:
            ASTtoSymtab(T->ptr[0],symboltable);
            ASTtoSymtab(T->ptr[1],symboltable);
            break;
        case Decl:
            ASTtoSymtab(T->ptr[0],symboltable);
            break;
        case ConstDecl:

            if(T->ptr[0]->kind==ConstDecl)  ASTtoSymtab(T->ptr[0],symboltable);
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
                   mysymbol.types+=string("[]");//数组的类型[]
            
             symboltable.Push(mysymbol);  //常量（变量）入表
            break;
        case VarDecl:

            if(T->ptr[0]->kind==VarDecl)  ASTtoSymtab(T->ptr[0],symboltable);
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
            break;
        case VarDef:
            break;
        case Idents:
            break;
        case InitVals:
            break;
        case InitVal:
            break;
        case ASSIGN:
            ASTtoSymtab(T->ptr[0],symboltable);
            ASTtoSymtab(T->ptr[1],symboltable);
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
            ASTtoSymtab(T->ptr[1],symboltable);
            ASTtoSymtab(T->ptr[2],symboltable);
            break;
        case WHILE:
            ASTtoSymtab(T->ptr[1],symboltable);
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

//计算各结点属性
void AST::calAttr(struct node *T,Symboltable &symboltable){
    Symbol mysymbol,*symbol;
    struct node *cur,*T0;
    BasicType* son;
    vector<Type *> args;
    Type *realarg;
    int i,index;
    if(T) {
        switch(T->kind) {
        case Root:
            T->level=0;//根节点在第0层
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            break;
        case CompUnit:
            T->level=0; //CompUnit结点的子节点也在第0层
            if(T->ptr[0]) T->ptr[0]->level=0;
            if(T->ptr[1]) T->ptr[1]->level=0;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            break;
        case FuncDef:
            if(T->ptr[0]) T->ptr[0]->level=0;   //函数类型结点还在第0层
            if(T->ptr[1]) T->ptr[1]->level=1;   //参数在第1层
            if(T->ptr[2]) T->ptr[2]->level=0;   //函数体在第1层，但是Block会加1

            //计算T->pretype
            if(T->ptr[1])//如果函数有形参
            {
                if(T->ptr[1]->pretype->is_Product_Type())//函数定义的形参只有一个,是积类型
                    args=static_cast<Product_Type*>(T->ptr[1]->pretype)->eles;
                else//函数定义的形参只有一个
                    args.push_back(T->ptr[1]->pretype);
            }
            T->pretype=new Fun_Type(*(static_cast<BasicType*>(T->pretype)),args);
            
            //符号入表
            mysymbol.name=string(T->type_id);
            mysymbol.level=T->level;
            mysymbol.pretype=T->pretype;
            mysymbol.types=T->pretype->getvalue();
            mysymbol.paramnum=static_cast<Fun_Type*>(T->pretype)->num_args;
            mysymbol.flag='F';
            index=symboltable.Push(mysymbol)-1;

            symboltable.Push_index();
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            if(T->ptr[2]) calAttr(T->ptr[2],symboltable);
            symboltable.Pop_until(symboltable.Pop_index());
            
           
            break;
        case FuncFParams:
            if(T->ptr[0]) T->ptr[0]->level=1;   
            if(T->ptr[1]) T->ptr[1]->level=1;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            break;
        case FuncFParam:
            if(T->ptr[0]) T->ptr[0]->level=1;   
            if(T->ptr[1]) T->ptr[1]->level=1;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);

            mysymbol.name=string(T->type_id);
            mysymbol.level=T->level;
            mysymbol.pretype=T->pretype;
            mysymbol.types=T->pretype->getvalue();
            mysymbol.flag='P';
            symboltable.Push(mysymbol);  
            break;
        case FuncFParamArray:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            break;
        case BType:
            T->width=4;
            break;
        case Block:
            symboltable.Push_index();
            if(T->ptr[0]) T->ptr[0]->level=T->level+1;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[0]) T->pretype=T->ptr[0]->pretype;
            symboltable.Pop_until(symboltable.Pop_index());
            break;
        case BlockItems:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1])  T->ptr[1]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            break;
        case Decl:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            break;
        case ConstDecl:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            break;
        case VarDecl:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            break;
        case ConstDef:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            //计算T->pretype
            if(T->ptr[0]->pretype->is_BasicType()){
                *T->ptr[0]->pretype=*T->pretype;
            }
            else if(T->ptr[0]->pretype->is_Array_Type())
                static_cast<Array_Type*>(T->ptr[0]->pretype)->setBasicType(*static_cast<BasicType*>(T->pretype));
            T->pretype=T->ptr[0]->pretype;
            // cout<<T->pretype->getvalue()<<endl;

            //符号入表
            mysymbol.name=string(T->ptr[0]->type_id);
            mysymbol.level=T->level;
            mysymbol.flag='V';
            mysymbol.pretype=T->pretype;
            mysymbol.types=T->pretype->getvalue();
            symboltable.Push(mysymbol);
            break;
        case VarDef:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            
            //计算T->pretype
            if(T->ptr[0]->pretype->is_BasicType()){
                *T->ptr[0]->pretype=*T->pretype;
            }
            else if(T->ptr[0]->pretype->is_Array_Type())
                static_cast<Array_Type*>(T->ptr[0]->pretype)->setBasicType(*static_cast<BasicType*>(T->pretype));
            T->pretype=T->ptr[0]->pretype;
            // cout<<T->pretype->getvalue()<<endl;

            //符号入表
            mysymbol.name=string(T->ptr[0]->type_id);
            mysymbol.level=T->level;
            mysymbol.flag='V';
            mysymbol.pretype=T->pretype;
            mysymbol.types=T->pretype->getvalue();
            symboltable.Push(mysymbol);

            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            // if(T->ptr[1]) cout<<T->ptr[1]->pretype->getvalue()<<endl;
            break;
        case Idents:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            break;
        case InitVals:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            T->pretype=T->ptr[0]->pretype;
            break;
        case InitVal:
            if(T->ptr[0]) {
                T->ptr[0]->level=T->level;
                calAttr(T->ptr[0],symboltable);
                T->pretype=T->ptr[0]->pretype;
            }
            else{
                T->pretype=new BasicType("void");
            }
            break;
        case ASSIGN:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            
            //隐式类型转换
            if(T->ptr[0]->pretype->getvalue().compare("int")==0 && T->ptr[1]->pretype->getvalue().compare("float")==0){
                static_cast<BasicType*>(T->ptr[1]->pretype)->setvalue("int");
                T->ptr[1]->type=INT;
                T->ptr[1]->type_int=T->ptr[1]->type_float;
            }
            else if(T->ptr[0]->pretype->getvalue().compare("float")==0 && T->ptr[1]->pretype->getvalue().compare("int")==0){
                T->pretype=new BasicType("bool");
                static_cast<BasicType*>(T->ptr[1]->pretype)->setvalue("float");
                T->ptr[1]->type=FLOAT;
                T->ptr[1]->type_float=T->ptr[1]->type_int;
            }
            
            // cout<<T->ptr[1]->pretype->getvalue()<<endl;

            break;
        case LVal:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);

            if(!T->ptr[0])//LVal: IDENT
            {
                index=symboltable.Search(string(T->type_id));
                if(index==-1){
                    yyerror("undefined symbol");
                    exit(101);
                }
                symbol=symboltable.getSymbol(index);
                T->pretype=symbol->pretype;
            }
            else{//LVal: LVal LB Exp RB
                if(!T->ptr[0]->pretype->is_Array_Type()){
                    yyerror("symbols are not arrays");
                    exit(104);
                }
                else if(T->ptr[1]->pretype->is_BasicType() && T->ptr[1]->pretype->getvalue().compare("int")==0){
                    T->pretype=static_cast<Array_Type*>(T->ptr[0]->pretype)->Lower_one_level();
                }
                else{
                    yyerror("illegal use of array");
                    exit(105);
                }
            }   
            break;
        case Number:

            break;
        case FuncCall:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[2]) T->ptr[2]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            if(T->ptr[2]) calAttr(T->ptr[2],symboltable);



            index=symboltable.Search(string(T->type_id));
            if(index==-1){
                yyerror("undefined function");
                exit(102);
            }
            symbol=symboltable.getSymbol(index);
            T->pretype=symbol->pretype;
            
            if(!T->pretype->is_Fun_Type()){//符号表中的符号并非函数
                yyerror("symbols are not functions");
                exit(103);
            }
            args=static_cast<Fun_Type*>(T->pretype)->args;//函数的形参
            realarg=(T->ptr[0])?T->ptr[0]->pretype:nullptr;//函数的实参
            // cout<<"函数类型:"<<T->pretype->getvalue()<<endl;
            // if(T->ptr[0]) cout<<"调用时的实参类型:"<<T->ptr[0]->pretype->getvalue()<<endl;
            // else cout<<"调用时没有实参"<<endl;
            if(realarg==nullptr && args.size()!=0){//有形参没实参
                yyerror("No overloaded function found");
                exit(107);
            }
            else if(args.size()==0 && T->ptr[0]){//没形参有实参
                yyerror("No overloaded function found");
                exit(107);
            }
            else if(realarg && realarg->is_BasicType() && 
            ((args.size()==1 && args[0]->getvalue().compare(realarg->getvalue())!=0) || args.size()!=1) ){//有一个实参且与形参不同
                yyerror("No overloaded function found");
                exit(107);
            }
            else if(realarg && realarg->is_Product_Type() 
            && !static_cast<Product_Type*>(realarg)->sameas(args)){//有多个实参且与形参不同
                yyerror("No overloaded function found");
                exit(107);
            }
            else
                T->pretype=&(static_cast<Fun_Type*>(T->pretype)->basictype);
            
            // cout<<T->pretype->getvalue()<<endl;
            break;
        case FuncRParams:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[2]) T->ptr[2]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            if(T->ptr[2]) calAttr(T->ptr[2],symboltable);


            if(T->ptr[1]){
                T->pretype=new Product_Type(T->ptr[0]->pretype,T->ptr[1]->pretype);
            }
            else{
                T->pretype=T->ptr[0]->pretype;
            }
            break;
        case UnaryExp:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[2]) T->ptr[2]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            if(T->ptr[2]) calAttr(T->ptr[2],symboltable);

            T->pretype=T->ptr[0]->pretype;
            break;
        case AddExp:
        case MulExp:
                if(T->ptr[0]) T->ptr[0]->level=T->level;
                if(T->ptr[1]) T->ptr[1]->level=T->level;
                if(T->ptr[2]) T->ptr[2]->level=T->level;
                if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
                if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
                if(T->ptr[2]) calAttr(T->ptr[2],symboltable);
                if(T->ptr[0]->pretype->is_BasicType()==false || T->ptr[1]->pretype->is_BasicType()==false)
                {
                    yyerror("type error");
                    exit(106);
                }
                if(T->ptr[0]->pretype->getvalue().compare("int")==0 && T->ptr[1]->pretype->getvalue().compare("int")==0){
                    T->pretype=new BasicType("int");
                }
                else if(T->ptr[0]->pretype->getvalue().compare("float")==0 || T->ptr[1]->pretype->getvalue().compare("float")==0){
                    T->pretype=new BasicType("float");
                }
            break;
        case LOrExp://逻辑或表达式
        case LAndExp://逻辑与表达式
        case EqExp://相等性表达式
        case RelExp://关系表达式
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=T->level;
            if(T->ptr[2]) T->ptr[2]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            if(T->ptr[2]) calAttr(T->ptr[2],symboltable);

            if(T->ptr[0]->pretype->getvalue().compare("int")==0 && T->ptr[1]->pretype->getvalue().compare("int")==0){
                T->pretype=new BasicType("int");
            }
            else if(T->ptr[0]->pretype->getvalue().compare("float")==0 || T->ptr[1]->pretype->getvalue().compare("float")==0){
                T->pretype=new BasicType("int");
            }
            else {
                yyerror("type error");
                exit(106);
            }

            break;
        case IF:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=(T->ptr[1]->kind!=Block)?T->level+1:T->level;
            if(T->ptr[2]) T->ptr[2]->level=(T->ptr[2]->kind!=Block)?T->level+1:T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            if(T->ptr[2]) calAttr(T->ptr[2],symboltable);
            break;
        case WHILE:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[1]) T->ptr[1]->level=(T->ptr[1]->kind!=Block)?T->level+1:T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            break;
        case RETURN:
            if(T->ptr[0]) T->ptr[0]->level=T->level;
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            if(T->ptr[2]) calAttr(T->ptr[2],symboltable);
            break;
        case CONTINUE:
            
        case BREAK:
            if(T->ptr[0]) calAttr(T->ptr[0],symboltable);
            if(T->ptr[1]) calAttr(T->ptr[1],symboltable);
            if(T->ptr[2]) calAttr(T->ptr[2],symboltable);
            break;
        }
    }
    
}