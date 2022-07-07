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

int searchSymbolTable(char *name) {
    int i;
    for(i=symbolTable.index-1;i>=0;i--)
        if (!strcmp(symbolTable.symbols[i].name, name))  return i;
    return -1;
}

char *strcat0(char *s1,char *s2){
    static char result[10];
    strcpy(result,s1);
    strcat(result,s2);
    return result;
}

char *newAlias() {
    static int no=1;
    char s[10];
    sprintf(s, "%d", no++);
    return strcat0("v",s);
}

char *newLabel() {
    static int no=1;
    char s[10];
    sprintf(s, "%d", no++);
    return strcat0("label",s);
}

char *newTemp(){
    static int no=1;
    char s[10];
    sprintf(s, "%d", no++);
    return strcat0("temp",s);
}

char *newBlock() {  
    static int no = 1;  
    char s[10];  
    snprintf(s, 10, "%d", no++);  
    return strcat0("block", s);  
}

//生成一条TAC代码的结点组成的双向循环链表，返回头指针
struct codenode *genIR(int op,struct opn opn1,struct opn opn2,struct opn result){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=op;
    h->opn1=opn1;
    h->opn2=opn2;
    h->result=result;
    h->next=h->prior=h;
    return h;
}

//生成一条标号语句，返回头指针
struct codenode *genLabel(char *label){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=LABEL;
    strcpy(h->result.id,label);
    h->next=h->prior=h;
    return h;
}

//生成GOTO语句，返回头指针
struct codenode *genGoto(char *label){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=GOTO;
    strcpy(h->result.id,label);
    h->next=h->prior=h;
    return h;
}

struct codenode *genBlock(char *label)  
{  
    struct codenode *h = (struct codenode *)malloc(sizeof(struct codenode));  
    h->op = Block;  
    strcpy(h->result.id, label);  
    h->next = h->prior = h;  
    return h;  
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
            t1=h1->prior;
            t2=h2->prior;
            t1->next=h2;
            t2->next=h1;
            h1->prior=t2;
            h2->prior=t1;
            }
        }
    va_end(ap);
    return h1;
}

int *findAllGOTOLabel(struct codenode *head)  
{  
    struct codenode *p = head;  
    int totals; //得知有多少个label  
    char *tmp = newLabel();  
    sscanf(&(tmp[5]), "%d", &totals);  
    int *labelsTable = (int *)malloc(sizeof(int) * (totals + 1));  
    do  
    {  
        if (p->op == GOTO)  
        {  
            sscanf(&(p->result.id[5]), "%d", &totals);  
            labelsTable[totals] = 1;  
        }  
        p = p->next;  
    } while (p != head);  
    return labelsTable;  
} 

struct codenode* splitBlock(struct codenode* head)  
{  
    struct codenode* p = head;  
    int num;  
    //找出所有GOTO涉及的Label  
    int* labelsTable = findAllGOTOLabel(head);  
    //在开始语句之前生成一句block  
    p = merge(2, genBlock(newBlock()), p);  
    head = p;  
    do  
    {  
        //当前节点是LABEL，并且前驱节点不是BLOCK  
        if (p->op == LABEL)  
        {  
            sscanf(&(p->result.id[5]), "%d", &num);  
            if (labelsTable[num] == 1 && p->prior->op != Block)  
            {  
                p = merge(2, genBlock(newBlock()), p);  
            }  
        }  
        //当前节点是跳转，且下条语句不是BLOCK  
        if ((p->op == GOTO || p->op == GE || p->op == GT || p->op == LE || p->op == LT ||  
            p->op == EQ || p->op == NE || p->op == RETURN) &&  
            p->next->op != Block)  
            p->next = merge(2, genBlock(newBlock()), p->next);  
        p = p->next;  
    } while (head != p);  
    return head;  
}  


//输出中间代码
void prnIR(struct codenode *head){
    char opnstr1[32],opnstr2[32],resultstr[32];
    struct codenode *h=head;
    if(head==NULL)
        return;
    do {
        if (h->opn1.kind==INT)
             sprintf(opnstr1,"%d",h->opn1.const_int);
        if (h->opn1.kind==FLOAT)
             sprintf(opnstr1,"%f",h->opn1.const_float);
        if (h->opn1.kind==IDENT)
             sprintf(opnstr1,"%s",h->opn1.id);
        if (h->opn2.kind==INT)
             sprintf(opnstr2,"%d",h->opn2.const_int);
        if (h->opn2.kind==FLOAT)
             sprintf(opnstr2,"%f",h->opn2.const_float);
        if (h->opn2.kind==IDENT)
             sprintf(opnstr2,"%s",h->opn2.id);
        if (h->result.kind==INT)
             sprintf(resultstr,"%d",h->result.const_int);
        if (h->result.kind==FLOAT)
             sprintf(resultstr,"%f",h->result.const_float);
        if (h->result.kind==IDENT)
             sprintf(resultstr,"%s",h->result.id);
        switch (h->op) {
            case ASSIGN:
                printf("%s = %s\n", resultstr, opnstr1);
                break;
            case ADD:
                printf("%s = %s %c %s\n", resultstr, opnstr1, '+', opnstr2);
                break;
            case SUB:
                printf("%s = %s %c %s\n", resultstr, opnstr1, '-', opnstr2);
                break;
            case MUL:
                printf("%s = %s %c %s\n", resultstr, opnstr1, '*', opnstr2);
                break;
            case DIV:
                printf("%s = %s %c %s\n", resultstr, opnstr1, '/', opnstr2);
                break;
            case MOD:
                printf("%s = %s %c %s\n", resultstr, opnstr1, '%', opnstr2);
                break;
            case FuncDef:
                printf("FUNCTION %s\n", h->result.id);
                break;
            case FuncFParam:
                printf("PARAM %s\n", h->result.id);
                break;
            case LABEL:
                printf("LABEL %s\n", h->result.id);
                break;
            case GOTO:
                printf("GOTO %s\n", h->result.id);
                break;
            case LE:
                printf("IF %s <= %s GOTO %s\n", opnstr1, opnstr2, resultstr);
                break;
            case LT:
                printf("IF %s < %s GOTO %s\n", opnstr1, opnstr2, resultstr);
                break;
            case GE:
                printf("IF %s >= %s GOTO %s\n", opnstr1, opnstr2, resultstr);
                break;
            case GT:
                printf("IF %s > %s GOTO %s\n", opnstr1, opnstr2, resultstr);
                break;
            case EQ:
                printf("IF %s == %s GOTO %s\n", opnstr1, opnstr2, resultstr);
                break;
            case NE:
                printf("IF %s != %s GOTO %s\n", opnstr1, opnstr2, resultstr);
                break;
            case FuncRParams:
                printf("ARG %s\n", resultstr);
                break;
            case FuncCall:
                if(h->result.kind)
                    printf("%s = CALL %s\n", resultstr, opnstr1);
                else
                    printf("CALL %s\n", opnstr1);
                break;
            case RETURN:
                if (h->result.kind)
                    printf("RETURN %s\n", resultstr);
                else
                    printf("RETURN\n");
                break;
            case Block:
                printf("BLOCK %s:\n", h->result.id);
        }
    h=h->next;
    } while (h!=head);
}


int fillSymbolTable(char *name,char *alias,int level,int type,char flag,int offset) {
    //首先根据name查符号表，不能重复定义 重复定义返回-1
    int i;
    /*符号查重，考虑外部变量声明前有函数定义，
    其形参名还在符号表中，这时的外部变量与前函数的形参重名是允许的*/
    for(i=symbolTable.index-1; i>=0 && (symbolTable.symbols[i].level==level||level==0); i--) {
        if (level==0 && symbolTable.symbols[i].level==1) continue;  //外部变量和形参不必比较重名
        if (!strcmp(symbolTable.symbols[i].name, name))  return -1;
        }
    //填写符号表内容
    strcpy(symbolTable.symbols[symbolTable.index].name,name);
    strcpy(symbolTable.symbols[symbolTable.index].alias,alias);
    symbolTable.symbols[symbolTable.index].level=level;
    symbolTable.symbols[symbolTable.index].type=type;
    symbolTable.symbols[symbolTable.index].flag=flag;
    symbolTable.symbols[symbolTable.index].offset=offset;
    return symbolTable.index++; //返回的是符号在符号表中的位置序号，中间代码生成时可用序号取到符号别名
}

//填写临时变量到符号表，返回临时变量在符号表中的位置
int fill_Temp(char *name,int level,int type,char flag,int offset) {
    strcpy(symbolTable.symbols[symbolTable.index].name,"");
    strcpy(symbolTable.symbols[symbolTable.index].alias,name);
    symbolTable.symbols[symbolTable.index].level=level;
    symbolTable.symbols[symbolTable.index].type=type;
    symbolTable.symbols[symbolTable.index].flag=flag;
    symbolTable.symbols[symbolTable.index].offset=offset;
    return symbolTable.index++; //返回的是临时变量在符号表中的位置序号
}


void pushSymbolTable() {
    symbol_scope_TX.TX[symbol_scope_TX.top++] = symbolTable.index;
}

void popSymbolTable() {
    symbolTable.index = symbol_scope_TX.TX[--symbol_scope_TX.top];
}

struct node *mywhile;

void genTAC(struct node *T) {
    struct node *T0;
    int i;
    int rtn;
    struct opn opn1,opn2,result;
    if(T) {
        T->code = NULL;
        switch(T->kind) {
        case Root:
            fillSymbolTable("putint", "", lev, VOID, 'F', 0);
            genTAC(T->ptr[0]);
            T->ptr[0]->code = splitBlock(T->ptr[0]->code);
            prnIR(T->ptr[0]->code);
            printf("Optimized code:\n");
            prnIR(T->ptr[0]->code);
            break;
        case CompUnit:
            genTAC(T->ptr[0]);
            T->code = T->ptr[0]->code;
            if(T->ptr[1]) {
                genTAC(T->ptr[1]);
                T->code = merge(2, T->code, T->ptr[1]->code);
            }
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
            rtn = fillSymbolTable(T->type_id, newAlias(), lev, T->type, 'F', 0);
            symbolTable.symbols[rtn].paramnum = i;
            T->place = rtn;
            result.kind = IDENT;
            strcpy(result.id, T->type_id);
            result.offset = rtn;
            T->code = genIR(FuncDef, opn1, opn2, result);
            genTAC(T->ptr[1]);//形参
            strcpy(T->ptr[2]->Snext, newLabel());
            genTAC(T->ptr[2]);//函数块
            T->code = merge(4, T->code, T->ptr[1] ? T->ptr[1]->code : NULL, T->ptr[2]->code, genLabel(T->ptr[2]->Snext));
            break;
        case FuncFParams:
            genTAC(T->ptr[0]);
            T->code = T->ptr[0]->code;
            if(T->ptr[1]) {
                genTAC(T->ptr[1]);
                T->code = merge(2, T->code, T->ptr[1]->code);
            }
            break;
        case FuncFParam:
            rtn = fillSymbolTable(T->type_id, newAlias(), 1, T->type, 'P', T->offset);
            T->place = rtn;
            result.kind = IDENT;
            strcpy(result.id, symbolTable.symbols[rtn].alias);
            result.offset = T->offset;
            T->code = genIR(FuncFParam, opn1, opn2, result);
            break;
        case FuncFParamArray:
            break;
        case BType:
            break;
        case Block:
            lev++;
            pushSymbolTable();
            if(T->ptr[0]) {
                strcpy(T->ptr[0]->Snext, T->Snext);
                genTAC(T->ptr[0]);
                T->code = T->ptr[0]->code;
            }
            lev--;
            popSymbolTable();
            break;
        case BlockItems:
            genTAC(T->ptr[0]);
            T->code = T->ptr[0]->code;
            if(T->ptr[1]) {
                genTAC(T->ptr[1]);
                T->code = merge(2, T->code, T->ptr[1]->code);
            }
            break;
        case Decl:
            genTAC(T->ptr[0]);
            T->code = T->ptr[0]->code;
            break;
        case ConstDecl:
            if (T->ptr[0]->kind == ConstDecl)
                genTAC(T->ptr[0]);
            T->ptr[1]->type = T->type;
            genTAC(T->ptr[1]);
            T->code = merge(2, T->ptr[0]->code, T->ptr[1]->code);
            break;
        case VarDecl:
            if (T->ptr[0]->kind == VarDecl)
                genTAC(T->ptr[0]);
            T->ptr[1]->type = T->type;
            genTAC(T->ptr[1]);
            T->code = merge(2, T->ptr[0]->code, T->ptr[1]->code);
            break;
        case ConstDef:
        case VarDef:
            rtn = fillSymbolTable(T->type_id, newAlias(), lev, T->type, 'V', T->offset);
            T->place = rtn;
            i = 0;
            T0 = T->ptr[0];
            while(T0->ptr[0]) {
                symbolTable.symbols[rtn].dim[i] = T0->ptr[1]->type_int;
                i++;
                T0 = T0->ptr[0];
            }
            symbolTable.symbols[rtn].paramnum = i;
            genTAC(T->ptr[0]);
            genTAC(T->ptr[1]);
            break;
        case Idents:
            if(T->ptr[0]) {
                genTAC(T->ptr[0]);
            }
            if(T->ptr[1]) {
                genTAC(T->ptr[1]);
            }
            break;
        case InitVals:
            genTAC(T->ptr[0]);
            genTAC(T->ptr[1]);
            break;
        case InitVal:
            genTAC(T->ptr[0]);
            break;
        case ASSIGN:
            strcpy(T->Snext, newLabel());
            Exp(T);
            break;
        case LVal:
            if(T->ptr[0]) {
                struct node *New_Idents = mknode(Idents, T->ptr[0], T->ptr[1], NULL, T->pos);
                strcpy(New_Idents->type_id, T->type_id);
                struct node *cur = New_Idents;
                while(cur->ptr[0]) {
                    cur->ptr[0]->kind = Idents;
                    cur = cur->ptr[0];
                }
                genTAC(New_Idents);
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
            Exp(T);
            break;
        case FuncRParams:
            genTAC(T->ptr[0]);
            genTAC(T->ptr[1]);
            break;
        case UnaryExp:
            genTAC(T->ptr[0]);
            break;
        case AddExp:
        case MulExp:
        case LOrExp:
        case LAndExp:
        case EqExp:
        case RelExp:
            genTAC(T->ptr[0]);
            genTAC(T->ptr[1]);
            break;
        case RETURN:
            if(T->ptr[0]) {
                Exp(T->ptr[0]);
                if (T->ptr[0]->kind == Number) {
                    result.kind = T->ptr[0]->type;
                    result.const_int = T->ptr[0]->type_int;
                }
                else {
                    result.kind = IDENT;
                    strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
                }
                T->code = merge(2, T->ptr[0]->code, genIR(RETURN, opn1, opn2, result));
            }
            else {
                result.kind = 0;
                T->code = genIR(RETURN, opn1, opn2, result);
            }
            break;
        case IF:
            strcpy(T->Snext, newLabel());
            if(!T->ptr[2]) {
                strcpy(T->ptr[0]->Etrue, newLabel());
                strcpy(T->ptr[0]->Efalse, T->Snext);
                boolExp(T->ptr[0]);
                strcpy(T->ptr[1]->Snext, T->Snext);
                genTAC(T->ptr[1]);
                T->code = merge(4, T->ptr[0]->code, genLabel(T->ptr[0]->Etrue), T->ptr[1]->code, genLabel(T->Snext));
            }
            break;
        case WHILE:
            mywhile = T;
            strcpy(T->Snext, newLabel());
            strcpy(T->ptr[0]->Etrue, newLabel()); //子结点继承属性的计算
            strcpy(T->ptr[0]->Efalse, T->Snext);
            boolExp(T->ptr[0]); //循环条件，要单独按短路代码处理
            strcpy(T->ptr[1]->Snext, newLabel());
            genTAC(T->ptr[1]); //循环体
            T->code = merge(6, genLabel(T->ptr[1]->Snext), T->ptr[0]->code,
                            genLabel(T->ptr[0]->Etrue), T->ptr[1]->code, genGoto(T->ptr[1]->Snext), genLabel(T->Snext));
            mywhile = NULL;
            break;
        case CONTINUE:
            if(mywhile) {
                T->code = genGoto(mywhile->ptr[1]->Snext);
            }
            break;
        case BREAK:
            if(mywhile) {
                T->code = genGoto(mywhile->Snext);
            }
            break;
        }
    }
}

void Exp(struct node *T) {
    struct node *T0;
    int op;
    int rtn;
    struct opn opn1,opn2,result;
    if(T) {
        switch(T->kind) {
            case Number:
                newTemp();
                break;
            case ASSIGN:
                Exp(T->ptr[0]);
                T->type = T->ptr[0]->type;
                T->code = T->ptr[0]->code;
                Exp(T->ptr[1]);
                if (T->ptr[1]->kind == Number) {
                    opn1.kind = T->ptr[1]->type;
                    opn1.const_int = T->ptr[1]->type_int;
                }
                else {
                    opn1.kind = IDENT;
                    strcpy(opn1.id, symbolTable.symbols[T->ptr[1]->place].alias);
                    T->code = merge(2, T->code, T->ptr[1]->code);
                }
                result.kind = IDENT;
                strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
                T->code = merge(2, T->code, genIR(ASSIGN, opn1, opn2, result));
                break;
            case LVal:
                rtn = searchSymbolTable(T->type_id);
                T->place = rtn;
                T->code = NULL;
                T->type = symbolTable.symbols[rtn].type;
                if (symbolTable.symbols[rtn].paramnum > 0) {    
                    int num[10], place[10];
                    for (int i = 0; i < symbolTable.symbols[rtn].paramnum - 1; i++) {
                        num[i] = 4 * symbolTable.symbols[rtn].dim[i];
                    }
                    num[symbolTable.symbols[rtn].paramnum - 1] = 4;
                    T0 = T;
                    int i = symbolTable.symbols[rtn].paramnum - 1;
                    while(T0 && T0->ptr[1]) {
                        Exp(T0->ptr[1]);
                        if(T0->ptr[1]->kind == Number) {
                            opn2.kind = T0->ptr[1]->type;
                            opn2.const_int = T0->ptr[1]->type_int;
                        }
                        else {
                            opn2.kind = IDENT;
                            strcpy(opn2.id, symbolTable.symbols[T0->ptr[1]->place].alias);
                            T->code = merge(2, T->code, T0->ptr[1]->code);
                        }
                        opn1.kind = INT;
                        opn1.const_int = num[i];
                        place[i] = fill_Temp(newTemp(), lev, T->type, 'T', 0);
                        result.kind = IDENT;
                        strcpy(result.id, symbolTable.symbols[place[i]].alias);
                        result.type = T->type;
                        T->code = merge(2, T->code, genIR(MUL, opn1, opn2, result));
                        i--;
                        T0 = T0->ptr[0];
                    }
                    int newplace = fill_Temp(newTemp(), lev, T->type, 'T', 0);
                    opn1.kind = INT;
                    opn1.const_int = 0;
                    result.kind = IDENT;
                    strcpy(result.id, symbolTable.symbols[newplace].alias);
                    T->code = merge(2, T->code, genIR(ASSIGN, opn1, opn2, result));
                    opn1.kind = IDENT;
                    strcpy(opn1.id, symbolTable.symbols[newplace].alias);
                    for (i = 0; i < symbolTable.symbols[rtn].paramnum; i++) {
                        opn2.kind = IDENT;
                        strcpy(opn2.id, symbolTable.symbols[place[i]].alias);
                        T->code = merge(2, T->code, genIR(ADD, opn1, opn2, result));
                    }
                    char newname[32];
                    sprintf(newname, "%s[%s]", symbolTable.symbols[rtn].alias, symbolTable.symbols[newplace].alias);
                    T->place = fill_Temp(newname, lev, T->type, 'T', 0);
                }
                break;
            case AddExp:
            case MulExp:
                T->code = NULL;
                Exp(T->ptr[0]);
                if (T->ptr[0]->kind == Number) {
                    opn1.kind = T->ptr[0]->type;
                    opn1.const_int = T->ptr[0]->type_int;
                }
                else {
                    opn1.kind = IDENT;
                    strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
                    T->code = merge(2, T->code, T->ptr[0]->code);
                }
                Exp(T->ptr[1]);
                if(T->ptr[1]->kind == Number) {
                    opn2.kind = T->ptr[1]->type;
                    opn2.const_int = T->ptr[1]->type_int;
                }
                else {
                    opn2.kind = IDENT;
                    strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
                    T->code = merge(2, T->code, T->ptr[1]->code);
                }
                if (T->ptr[0]->type == FLOAT || T->ptr[1]->type == FLOAT)
                    T->type = FLOAT;
                else
                    T->type = INT;
                T->place = fill_Temp(newTemp(), lev, T->type, "T", 0);
                result.kind = IDENT;
                strcpy(result.id, symbolTable.symbols[T->place].alias);
                result.type = T->type;
                if (strcmp(T->op, "+") == 0)
                    op = ADD;
                else if (strcmp(T->op, "-") == 0)
                    op = SUB;
                else if (strcmp(T->op, "*") == 0)
                    op = MUL;
                else if (strcmp(T->op, "/") == 0)
                    op = DIV;
                else if (strcmp(T->op, "%") == 0)
                    op = MOD;
                T->code = merge(2, T->code, genIR(op, opn1, opn2, result));
                break;
            case FuncCall:
                rtn = searchSymbolTable(T->type_id);
                T->type = symbolTable.symbols[rtn].type;
                if(T->ptr[0]) {
                    Exp(T->ptr[0]);
                    T->code = T->ptr[0]->code;
                }
                struct codenode *T1 = NULL;
                T0 = T->ptr[0];
                while(T0 && T0->kind == FuncRParams) {
                    result.kind = IDENT;
                    if(T0->ptr[1]) 
                        strcpy(result.id, symbolTable.symbols[T0->ptr[1]->place].alias);
                    else
                        strcpy(result.id, symbolTable.symbols[T0->ptr[0]->place].alias);
                    T1 = merge(2, genIR(FuncRParams, opn1, opn2, result), T1);
                    T0 = T0->ptr[0];
                }
                T->code = merge(2, T->code, T1);
                opn1.kind = IDENT;
                strcpy(opn1.id, T->type_id);
                if (symbolTable.symbols[rtn].type != VOID) {
                    T->place = fill_Temp(newTemp(), lev, T->type, 'T', 0);
                    result.kind = IDENT;
                    strcpy(result.id, symbolTable.symbols[T->place].alias);
                }
                else {
                    result.kind = 0;
                }
                T->code = merge(2, T->code, genIR(FuncCall, opn1, opn2, result));
                break;
            case FuncRParams:
                Exp(T->ptr[0]);
                T->code = T->ptr[0]->code;
                if(T->ptr[1]) {
                    Exp(T->ptr[1]);
                    T->code = merge(2, T->code, T->ptr[1]->code);
                }
                break;
        }
    }
}

void boolExp(struct node *T) {
    struct opn opn1, opn2, result;
    int op;
    int rtn;
    if(T) {
        switch(T->kind) {
            case RelExp:
            case EqExp:
                T->code = NULL;
                Exp(T->ptr[0]);
                if (T->ptr[0]->kind == Number) {
                    opn1.kind = T->ptr[0]->type;
                    opn1.const_int = T->ptr[0]->type_int;
                }
                else {
                    opn1.kind = IDENT;
                    strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
                    T->code = merge(2, T->code, T->ptr[0]->code);
                }
                Exp(T->ptr[1]);
                if(T->ptr[1]->kind == Number) {
                    opn2.kind = T->ptr[1]->type;
                    opn2.const_int = T->ptr[1]->type_int;
                }
                else {
                    opn2.kind = IDENT;
                    strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
                    T->code = merge(2, T->code, T->ptr[1]->code);
                }
                result.kind = IDENT;
                strcpy(result.id, T->Etrue);
                if (strcmp(T->op, "<") == 0)
                    op = LT;
                else if (strcmp(T->op, "<=") == 0)
                    op = LE;
                else if (strcmp(T->op, ">") == 0)
                    op = GT;
                else if (strcmp(T->op, ">=") == 0)
                    op = GE;
                else if (strcmp(T->op, "==") == 0)
                    op = EQ;
                else if (strcmp(T->op, "!=") == 0)
                    op = NE;
                T->code = merge(3, T->code, genIR(op, opn1, opn2, result), genGoto(T->Efalse));
                break;
            case LAndExp:
            case LOrExp:
                if(T->kind==LAndExp) {
                    strcpy(T->ptr[0]->Etrue, newLabel());
                    strcpy(T->ptr[0]->Efalse, T->Efalse);
                }
                else {
                    strcpy(T->ptr[0]->Etrue, T->Etrue);
                    strcpy(T->ptr[0]->Efalse, newLabel());
                }
                strcpy(T->ptr[1]->Etrue, T->Etrue);
                strcpy(T->ptr[1]->Efalse, T->Efalse);
                boolExp(T->ptr[0]);
                boolExp(T->ptr[1]);
                if(T->kind==LAndExp)
                    T->code = merge(3, T->ptr[0]->code, genLabel(T->ptr[0]->Etrue), T->ptr[1]->code);
                else
                    T->code = merge(3, T->ptr[0]->code, genLabel(T->ptr[0]->Efalse), T->ptr[1]->code);
                break;
        }
    }
}