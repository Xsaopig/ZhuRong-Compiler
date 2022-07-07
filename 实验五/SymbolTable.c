#include "def.h"
struct node *mknode(int kind, struct node *first, struct node *second, struct node *third, int pos) {
    struct node *T = (struct node *)malloc(sizeof(struct node));
    T->kind = kind;
    T->ptr[0] = first;
    T->ptr[1] = second;
    T->ptr[2] = third;
    T->pos = pos;
    return T;
}
//deep为0的时候意思是这一个层次的最后一个前面要置为"`"而不是"|",last数组是记录某个层次是否结束
void printAST(struct node *T, int indent, int deep) {
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


void DisplaySymbolTable() {
    printf("----------------symbol table-----------------------\n");
    printf("%s\t%s\t%s\t%s\t%s\t%s\n","Index","Name","Level","Type","Flag","Param_num");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < symbolTable.index; i++) {
        printf("%d\t%s\t%d\t", i, symbolTable.symbols[i].name, symbolTable.symbols[i].level);
        if (symbolTable.symbols[i].type == INT)
            printf("int");
        else if(symbolTable.symbols[i].type == FLOAT)
            printf("float");
        else
            printf("void");
        if (symbolTable.symbols[i].flag == 'F') {
            printf("(");
            for (int j = 0; j < symbolTable.symbols[i].paramnum; j++) {
                printf("int");
                if (j != symbolTable.symbols[i].paramnum - 1)
                    printf(",");
            }
            if (symbolTable.symbols[i].paramnum == 0)
                printf("void");
            printf(")");
        }
        printf("\t");
        printf("%c", symbolTable.symbols[i].flag);
        if (symbolTable.symbols[i].flag == 'F') {
            printf("\t%d", symbolTable.symbols[i].paramnum);
        }
        printf("\n");
    }
    printf("---------------------------------------------------\n");
}

int fillSymbolTable(char *name,int level,int type,char flag) {
    //首先根据name查符号表，不能重复定义 重复定义返回-1
    int i;
    /*符号查重，考虑外部变量声明前有函数定义，
    其形参名还在符号表中，这时的外部变量与前函数的形参重名是允许的*/
    for(i=symbolTable.index-1; i>=0 && (symbolTable.symbols[i].level==level||level==0); i--) {
        if (level==0 && symbolTable.symbols[i].level==1) continue;  //外部变量和形参不必比较重名
        if (!strcmp(symbolTable.symbols[i].name, name)) {
            if(type!=symbolTable.symbols[i].type)
                return -2;
            return -1;
        }
    }
    //填写符号表内容
    strcpy(symbolTable.symbols[symbolTable.index].name,name);
    symbolTable.symbols[symbolTable.index].level=level;
    symbolTable.symbols[symbolTable.index].type=type;
    symbolTable.symbols[symbolTable.index].flag=flag;
    return symbolTable.index++; //返回的是符号在符号表中的位置序号，中间代码生成时可用序号取到符号别名
}

int getSymbolTable(char *name, int paramnum, char flag) {
    int i;
    for (i = symbolTable.index - 1; i >= 0; i--) {
        if (!strcmp(symbolTable.symbols[i].name, name)) {
            if(flag=='V') {
                if(symbolTable.symbols[i].flag=='V' && paramnum < symbolTable.symbols[i].paramnum)
                    return -2;
                else if(symbolTable.symbols[i].flag=='F')
                    return -3;
                else if(symbolTable.symbols[i].flag=='V' && paramnum > symbolTable.symbols[i].paramnum)
                    return -4;
                else
                    return 0;
            }
            else if(flag=='F') {
                if(symbolTable.symbols[i].flag=='V')
                    return -2;
                else if(symbolTable.symbols[i].flag=='F' && symbolTable.symbols[i].paramnum<paramnum)
                    return -3;
                else if(symbolTable.symbols[i].flag=='F' && symbolTable.symbols[i].paramnum>paramnum)
                    return -4;
                else
                    return 0;
            }
        }
    }
    return -1;
}

int typeSymbolTable(char *name) {
    int i;
    for (i = symbolTable.index - 1; i >= 0; i--) {
        if(!strcmp(symbolTable.symbols[i].name,name))
            return symbolTable.symbols[i].type;
    }
    return 0;
}

int returnSymbolTable() {
    int i;
    for (i = symbolTable.index - 1; i >= 0; i--) {
        if (symbolTable.symbols[i].flag == 'F')
            return symbolTable.symbols[i].type;
    }
    return 0;
}

int gettype(struct node *T) {
    if(T) {
        switch(T->kind) {
            case LVal:
                return typeSymbolTable(T->type_id);
            case Number:
                return T->type;
        }
    }
    return 0;
}

void pushSymbolTable() {
    symbol_scope_TX.TX[symbol_scope_TX.top++] = symbolTable.index;
}

void popSymbolTable() {
    symbolTable.index = symbol_scope_TX.TX[--symbol_scope_TX.top];
}

char errname[32];
char errline;
char iswhile;
void semantic_Analysis(struct node *T) {
    struct node *T0;
    int i;
    int rtn;
    if(T) {
        switch(T->kind) {
        case Root:
            fillSymbolTable("putint", 0, VOID, 'F');
            symbolTable.symbols[rtn].paramnum = 1;
            semantic_Analysis(T->ptr[0]);
            break;
        case CompUnit:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
        case FuncDef:
            T0 = T->ptr[1];
            i = 0;
            while (T0 && T0->ptr[0]->kind == FuncFParams) {
                i++;
                T0 = T0->ptr[0];
            }
            if (T0 && T0->ptr[0]->kind == FuncFParam)
                i++;
            rtn = fillSymbolTable(T->type_id, lev, T->type, 'F');
            if (rtn == -1) {
                printf("%s:%d redefinition of \‘%s\’\n", filename, T->pos, T->type_id);
            }
            else if(rtn==-2) {
                printf("%s:%d ", filename, T->pos);
                printf("\‘%s\’ redeclared as different kind of symbol\n", T->type_id);
            }
            else 
                symbolTable.symbols[rtn].paramnum = i;
            pushSymbolTable();
            lev++;
            semantic_Analysis(T->ptr[1]);
            lev--;
            semantic_Analysis(T->ptr[2]);
            popSymbolTable();
            break;
        case FuncFParams:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
        case FuncFParam:
            fillSymbolTable(T->type_id, lev, T->type, 'P');
            break;
        case FuncFParamArray:
            break;
        case BType:
            break;
        case Block:
            pushSymbolTable();
            lev++;
            semantic_Analysis(T->ptr[0]);
            popSymbolTable();
            lev--;
            break;
        case BlockItems:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
        case Decl:
            semantic_Analysis(T->ptr[0]);
            break;
        case ConstDecl:
            if (T->ptr[0]->kind == ConstDecl)
                semantic_Analysis(T->ptr[0]);
            T->ptr[1]->type = T->type;
            semantic_Analysis(T->ptr[1]);
            break;
        case VarDecl:
            if (T->ptr[0]->kind == VarDecl)
                semantic_Analysis(T->ptr[0]);
            T->ptr[1]->type = T->type;
            semantic_Analysis(T->ptr[1]);
            break;
        case ConstDef:
        case VarDef:
            i = 0;
            T0 = T->ptr[0];
            while(T0->ptr[0]) {
                i++;
                T0 = T0->ptr[0];
            }
            rtn = fillSymbolTable(T->type_id, lev, T->type, 'V');
            if(rtn == -1) {
                printf("%s:%d redefinition of \‘%s\’\n", filename, T->pos, T->type_id);
            }
            else if(rtn==-2) {
                printf("%s:%d ", filename, T->pos);
                printf("\‘%s\’ redeclared as different kind of symbol\n", T->type_id);
            }
            else
                symbolTable.symbols[rtn].paramnum = i;
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
        case Idents:
            if(T->ptr[0]) {
                semantic_Analysis(T->ptr[0]);
            }
            if(T->ptr[1]) {
                semantic_Analysis(T->ptr[1]);
            }
            break;
        case InitVals:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
        case InitVal:
            semantic_Analysis(T->ptr[0]);
            break;
        case ASSIGN:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
        case LVal:
            i = 0;
            T0 = T;
            while(T0->ptr[0]) {
                i++;
                T0 = T0->ptr[0];
            }
            rtn = getSymbolTable(T->type_id, i, 'V');
            if(rtn==-1 && strcmp(T->type_id,errname) && errline!=T->pos) {
                printf("%s:%d ", filename, T->pos);
                printf("\‘%s\’ undeclared\n", T->type_id);
                strcpy(errname, T->type_id);
                errline = T->pos;
            }
            else if(rtn==-2) {
                printf("%s:%d ", filename, T->pos);
                printf("assignment to expression with array type\n");
            }
            else if(rtn==-3) {
                printf("%s:%d ", filename, T->pos);
                if(!strcmp(T->op,"*")) {
                    printf("invalid operands to binary operator *\n");
                }
                else {
                    printf("lvalue required as left operand of assignment\n");
                }
            }
            else if(rtn==-4) {
                printf("%s:%d ", filename, T->pos);
                printf("subscripted value is not an array\n");
            }
            if (T->ptr[1]) {
                rtn = gettype(T->ptr[1]);
                if (rtn != INT) {
                    printf("%s:%d ", filename, T->pos);
                    printf("array subscript is not an integer\n");
                }
            }
            if(T->ptr[0]) {
                struct node *New_Idents = mknode(Idents, T->ptr[0], T->ptr[1], NULL, T->pos);
                strcpy(New_Idents->type_id, T->type_id);
                struct node *cur = New_Idents;
                while(cur->ptr[0]) {
                    cur->ptr[0]->kind = Idents;
                    cur = cur->ptr[0];
                }
                semantic_Analysis(New_Idents);
                cur = New_Idents;
                while(cur->ptr[0]) {
                    cur->ptr[0]->kind = LVal;
                    cur = cur->ptr[0];
                }
                free(cur);
            }
            break;
        case Number:
            break;
        case FuncCall:
            i = 0;
            T0 = T->ptr[0];
            while(T0->kind==FuncRParams) {
                i++;
                T0 = T0->ptr[0];
            }
            rtn = getSymbolTable(T->type_id, i, 'F');
            if(rtn==-1) {
                printf("%s:%d ", filename, T->pos);
                printf("\‘%s\’ undeclared\n", T->type_id);
            }
            else if(rtn==-2) {
                printf("%s:%d ", filename, T->pos);
                printf("\‘%s\’ is not a function\n", T->type_id);
            }
            else if(rtn==-3) {
                printf("%s:%d ", filename, T->pos);
                printf("too many arguments to function \‘%s\’\n", T->type_id);
            }
            else if(rtn==-4) {
                printf("%s:%d ", filename, T->pos);
                printf("too few arguments to function \‘%s\’\n", T->type_id);
            }
            semantic_Analysis(T->ptr[0]);
            break;
        case FuncRParams:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
        case UnaryExp:
            semantic_Analysis(T->ptr[0]);
            break;
        case AddExp:
        case MulExp:
        case LOrExp:
        case LAndExp:
        case EqExp:
        case RelExp:
            strcpy(T->ptr[0]->op, T->op);
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
        case RETURN:
            rtn = returnSymbolTable();
            if(T->ptr[0]) {
                if(rtn==VOID) {
                    printf("%s:%d ", filename, T->pos);
                    printf("\‘return\’ with a value, in function returning void\n");
                }
            }
            semantic_Analysis(T->ptr[0]);
            break;
        case IF:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            if(T->ptr[2]) {
                semantic_Analysis(T->ptr[2]);
            }
            break;
        case WHILE:
            semantic_Analysis(T->ptr[0]);
            iswhile = 1;
            semantic_Analysis(T->ptr[1]);
            iswhile = 0;
            break;
        case CONTINUE:
            if(!iswhile) {
                printf("%s:%d ", filename, T->pos);
                printf("continue statement not within a loop\n");
            }
            break;
        case BREAK:
            if(!iswhile) {
                printf("%s:%d ", filename, T->pos);
                printf("break statement not within a loop\n");
            }
            break;
        }
    }
}