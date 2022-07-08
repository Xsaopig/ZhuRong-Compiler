#include "ast.hpp"

char last[50];
Symboltable symboltable;


struct node *mknode(int kind, struct node *first, struct node *second, struct node *third, int pos) {
    struct node *T = (struct node *)malloc(sizeof(struct node));
    T->kind = (enum node_kind)kind;
    T->ptr[0] = first;
    T->ptr[1] = second;
    T->ptr[2] = third;
    T->pos = pos;
    return T;
}

//转换函数，后面如果用到再说吧
/*string& AST::getTypeClass(Type pretype){
        string str=pretype.getNameofClass();
        if(str=="BasicType"||str=="Array_Type"){
            return pretype.getvalue();
        }
        if(str=="Fun_Type"||str=="Product_Type"){
            vector<type *> vec(pretype.getvalue());
            string res;
            for (int i = 0; i < vec.size()-1; i++){
		        res+=to_string(vec.at(i));
                res+=",";
	        }
            res+=to_string(vec.at(i));
            return res;
        }
        return nullptr;
}*/

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
                free(cur);
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

void AST::ASTtoSymtab(struct node *T) {
    Symbol mysymbol;
    struct node *cur,*T0;
    BasicType* son;
    int i;
    if(T) {
        switch(T->kind) {
        case Root:
            ASTtoSymtab(T->ptr[0]);
            break;
        case CompUnit:
            ASTtoSymtab(T->ptr[0]);
            ASTtoSymtab(T->ptr[1]);
            break;
        case FuncDef:
            mysymbol.name=string(T->type_id);
            mysymbol.level=lev;
            mysymbol.type=T->ptr[0]->type;//类型有误，VOID未加入，后续调整
            if(T->ptr[0]){
                    son=static_cast<BasicType*>(T->ptr[0]->pretype);
                    mysymbol.types=son->getvalue();
            }
            else  mysymbol.types="void";

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
            ASTtoSymtab(T->ptr[1]);   //进入函数参数
            ASTtoSymtab(T->ptr[2]);
            symboltable.Pop_until(symboltable.Pop_index());
            break;
        case FuncFParams:
            ASTtoSymtab(T->ptr[0]);
            ASTtoSymtab(T->ptr[1]);
            break;
        case FuncFParam:
            

            mysymbol.name=string(T->type_id);
            mysymbol.level=1;
            mysymbol.type=T->ptr[0]->type;//类型后续调整
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
            
            
            ASTtoSymtab(T->ptr[0]);
            ASTtoSymtab(T->ptr[1]);
            i=symboltable.Pop_index();
            symboltable.Pop_until(i);   //asd     
            lev=lev-1;
            break;
        case BlockItems:
            ASTtoSymtab(T->ptr[0]);
            ASTtoSymtab(T->ptr[1]);
            break;
        case Decl:
            ASTtoSymtab(T->ptr[0]);
            break;
        case ConstDecl:
            
            if(T->ptr[0]->kind==ConstDecl)  ASTtoSymtab(T->ptr[0]);
            mysymbol.name=string(T->ptr[1]->ptr[0]->type_id);
            mysymbol.level=lev;
            mysymbol.type=T->type;//类型后续调整
            mysymbol.alias="const ";
            //son=static_cast<BasicType*>(T->pretype);
            //mysymbol.types=son->getvalue();

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
            if(T->ptr[0]->kind==VarDecl)  ASTtoSymtab(T->ptr[0]);
            mysymbol.name=string(T->ptr[1]->ptr[0]->type_id);
            mysymbol.level=lev;
            mysymbol.type=T->type;//类型后续调整
            //son=static_cast<BasicType*>(T->ptr[0]->pretype);
            //mysymbol.types=son->getvalue();

            mysymbol.flag='V';
            cur=T->ptr[1]->ptr[0];
            i=0;
            while(cur->ptr[0]) {
                i++;
                cur = cur->ptr[0];
            }
            for (int n = 0; n < i; n++)
                mysymbol.name+=string("[]");//数组的类型[]记在name
            
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

            ASTtoSymtab(T->ptr[0]);
            ASTtoSymtab(T->ptr[1]);
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
            ASTtoSymtab(T->ptr[1]);
            ASTtoSymtab(T->ptr[2]);
            break;
        case WHILE:
            ASTtoSymtab(T->ptr[1]);
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
