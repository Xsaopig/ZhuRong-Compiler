%error-verbose
%locations
%{
    #include "stdio.h"
    #include "math.h"
    #include "string.h"
    #include "def.h"
    extern int yylineno;
    extern char *yytext;
    extern FILE *yyin;
    void yyerror(const char* fmt, ...);
    void printAST(struct node *, int, int);
%}

%union {
	int    type_int;
	float  type_float;
	char   type_id[32];
	struct node *ptr;
};

%token <type_int>       INTCONST
%token <type_id>        IDENT
%token <type_float>     FLOATCONST

%token ADD SUB MUL DIV MOD
%token LT GT LE GE EQ NE
%token AND OR NOT
%token ASSIGN
%token LP RP LC RC LB RB
%token COMMA SEMI
%token IF ELSE WHILE BREAK CONTINUE RETURN
%token CONST INT VOID FLOAT

%type <ptr> Root CompUnit Decl ConstDecl BType BlockItems
%type <ptr> ConstDef VarDecl VarDef FuncCall
%type <ptr> InitVal FuncDef FuncFParams
%type <ptr> FuncFParam Block Stmt
%type <ptr> Exp Cond LVal PrimaryExp Number InitVals
%type <ptr> UnaryExp UnaryOp FuncRParams Idents FuncFParamArray
%type <ptr> MulExp AddExp RelExp EqExp LAndExp LOrExp
 
%%

Root: CompUnit {
    $$=mknode(Root,$1,NULL,NULL,yylineno);
    printAST($$,0,0);
}
    ;

//编译单元
CompUnit: Decl {
    $$=mknode(CompUnit,$1,NULL,NULL,yylineno);
}
    | FuncDef {
        $$=mknode(CompUnit,$1,NULL,NULL,yylineno);
    }
    | CompUnit Decl {
        $$=mknode(CompUnit,$1,$2,NULL,yylineno);
    }
    | CompUnit FuncDef {
        $$=mknode(CompUnit,$1,$2,NULL,yylineno);
    }
    ;

//声明
Decl: ConstDecl SEMI {
    $$=mknode(Decl,$1,NULL,NULL,yylineno);
}
    | VarDecl SEMI {
        $$=mknode(Decl,$1,NULL,NULL,yylineno);
    }
    ;

//常量声明
ConstDecl: CONST BType ConstDef {
    $$=mknode(ConstDecl,$2,$3,NULL,yylineno);
    $$->type=$2->type;
}
    | ConstDecl COMMA ConstDef {
        $$=mknode(ConstDecl,$1,$3,NULL,yylineno);
        $$->type=$1->type;
    }
    ;

//基本类型
BType: INT {
    $$=mknode(BType,NULL,NULL,NULL,yylineno);
    $$->type=INT;
}
    | FLOAT {
        $$=mknode(BType,NULL,NULL,NULL,yylineno);
        $$->type=FLOAT;
    }
    ;

//包含数组的IDENT
Idents: IDENT {
    $$=mknode(Idents,NULL,NULL,NULL,yylineno);
    strcpy($$->type_id,$1);
}
    | Idents LB Exp RB {
        $$=mknode(Idents,$1,$3,NULL,yylineno);
        strcpy($$->type_id,$1->type_id);
    }
    ;

//常数定义
ConstDef: Idents ASSIGN InitVal {
    $$=mknode(ConstDef,$1,$3,NULL,yylineno);
}
    ;

//变量声明
VarDecl: BType VarDef {
    $$=mknode(VarDecl,$1,$2,NULL,yylineno);
    $$->type=$1->type;
}
    | VarDecl COMMA VarDef {
        $$=mknode(VarDecl,$1,$3,NULL,yylineno);
        $$->type=$1->type;
    }
    ;

//变量定义
VarDef: Idents {
    $$=mknode(VarDef,$1,NULL,NULL,yylineno);
}
    | Idents ASSIGN InitVal {
        $$=mknode(VarDef,$1,$3,NULL,yylineno);
    }
    ;

//包含数组的变量初值
InitVals: InitVal {
    $$=mknode(InitVals,$1,NULL,NULL,yylineno);
}
    | InitVals COMMA InitVal {
        $$=mknode(InitVals,$1,$3,NULL,yylineno);
    }
    ;

//变量初值
InitVal: Exp {
    $$=mknode(InitVal,$1,NULL,NULL,yylineno);
}
    | LC InitVals RC {
        $$=mknode(Block,$2,NULL,NULL,yylineno);
    }
    | LC RC {
        $$=mknode(Block,NULL,NULL,NULL,yylineno);
    }
    ;

//函数定义
FuncDef: VOID IDENT LP RP Block {
    $$=mknode(FuncDef,NULL,NULL,$5,yylineno);
    strcpy($$->type_id,$2);
}
    | BType IDENT LP FuncFParams RP Block {
        $$=mknode(FuncDef,$1,$4,$6,yylineno);
        strcpy($$->type_id,$2);
    }
    | BType IDENT LP RP Block {
        $$=mknode(FuncDef,$1,NULL,$5,yylineno);
        strcpy($$->type_id,$2);
    }
    | VOID IDENT LP FuncFParams RP Block {
        $$=mknode(FuncDef,NULL,$4,$6,yylineno);
        strcpy($$->type_id,$2);
    }
    ;

//函数形参表
FuncFParams: FuncFParam {
    $$=mknode(FuncFParams,$1,NULL,NULL,yylineno);
}
    | FuncFParams COMMA FuncFParam {
        $$=mknode(FuncFParams,$1,$3,NULL,yylineno);
    }
    ;

//函数形参的数组表示
FuncFParamArray: LB RB {
    $$=mknode(FuncFParamArray,NULL,NULL,NULL,yylineno);
}
    | FuncFParamArray LB Exp RB {
        $$=mknode(FuncFParamArray,$1,$3,NULL,yylineno);
    }

//函数形参
FuncFParam: BType IDENT {
    $$=mknode(FuncFParam,$1,NULL,NULL,yylineno);
    strcpy($$->type_id,$2);
}
    | BType IDENT FuncFParamArray {
        $$=mknode(FuncFParam,$1,$3,NULL,yylineno);
        strcpy($$->type_id,$2);
    }
    ;

//重复的语句块项
BlockItems: Decl {
    $$=mknode(BlockItems,$1,NULL,NULL,yylineno);
}
    | Stmt {
        $$=mknode(BlockItems,$1,NULL,NULL,yylineno);
    }
    | BlockItems Decl {
        $$=mknode(BlockItems,$1,$2,NULL,yylineno);
    }
    | BlockItems Stmt {
        $$=mknode(BlockItems,$1,$2,NULL,yylineno);
    }
    ;

//语句块
Block: LC BlockItems RC {
    $$=mknode(Block,$2,NULL,NULL,yylineno);
}
    | LC RC {
        $$=mknode(Block,NULL,NULL,NULL,yylineno);
    }
    ;

//语句
Stmt: LVal ASSIGN Exp SEMI {
    $$=mknode(ASSIGN,$1,$3,NULL,yylineno);
}
    | SEMI {
        $$=NULL;
    }
    | Exp SEMI {
        $$=$1;
    }
    | Block {
        $$=$1;
    }
    | IF LP Cond RP Stmt {
        $$=mknode(IF,$3,$5,NULL,yylineno);
    }
    | IF LP Cond RP Stmt ELSE Stmt {
        $$=mknode(IF,$3,$5,$7,yylineno);
    }
    | WHILE LP Cond RP Stmt {
        $$=mknode(WHILE,$3,$5,NULL,yylineno);
    }
    | BREAK SEMI {
        $$=mknode(BREAK,NULL,NULL,NULL,yylineno);
    }
    | CONTINUE SEMI {
        $$=mknode(CONTINUE,NULL,NULL,NULL,yylineno);
    }
    | RETURN SEMI {
        $$=mknode(RETURN,NULL,NULL,NULL,yylineno);
    }
    | RETURN Exp SEMI {
        $$=mknode(RETURN,$2,NULL,NULL,yylineno);
    }
    ;

//表达式
Exp: AddExp {
    $$=$1;
}
    ;

//条件表达式
Cond: LOrExp {
    $$=$1;
}
    ;

//左值表达式
LVal: IDENT {
    $$=mknode(LVal,NULL,NULL,NULL,yylineno);
    strcpy($$->type_id,$1);
}
    | LVal LB Exp RB {
        $$=mknode(LVal,$1,$3,NULL,yylineno);
        strcpy($$->type_id,$1->type_id);
    }
    ;

//基本表达式
PrimaryExp: LP Exp RP {
    $$=$2;
}
    | LVal {
        $$=$1;
    }
    | Number {
        $$=$1;
    }
    ;

//数值
Number: INTCONST {
    $$=mknode(Number,NULL,NULL,NULL,yylineno);
    $$->type_int=$1;
    $$->type=INT;
}
    | FLOATCONST {
        $$=mknode(Number,NULL,NULL,NULL,yylineno);
        $$->type_float=$1;
        $$->type=FLOAT;
    }
    ;

//一元表达式
UnaryExp: PrimaryExp {
    $$=$1;
}
    | ADD UnaryExp {
        $$=mknode(UnaryExp,$2,NULL,NULL,yylineno);
        strcpy($$->op,"+");
    }
    | SUB UnaryExp {
        $$=mknode(UnaryExp,$2,NULL,NULL,yylineno);
        strcpy($$->op,"-");
    }
    | NOT UnaryExp {
        $$=mknode(UnaryExp,$2,NULL,NULL,yylineno);
        strcpy($$->op,"!");
    }
    | FuncCall {
        $$=$1;
    }
    ;

//函数调用
FuncCall: IDENT LP RP {
    $$=mknode(FuncCall,NULL,NULL,NULL,yylineno);
    strcpy($$->type_id,$1);
}
    | IDENT LP FuncRParams RP {
        $$=mknode(FuncCall,$3,NULL,NULL,yylineno);
        strcpy($$->type_id,$1);
    }
    ;

//函数实参表 
FuncRParams: Exp {
    $$=mknode(FuncRParams,$1,NULL,NULL,yylineno);
}
    | FuncRParams COMMA Exp {
        $$=mknode(FuncRParams,$1,$3,NULL,yylineno);
    }
    ;

//乘除模表达式
MulExp: UnaryExp {
    $$=$1;
}
    | MulExp MUL UnaryExp {
        $$=mknode(MulExp,$1,$3,NULL,yylineno);
        strcpy($$->op,"*");
    }
    | MulExp DIV UnaryExp {
        $$=mknode(MulExp,$1,$3,NULL,yylineno);
        strcpy($$->op,"/");
    }
    | MulExp MOD UnaryExp {
        $$=mknode(MulExp,$1,$3,NULL,yylineno);
        strcpy($$->op,"%");
    }
    ;

//加减表达式
AddExp: MulExp {
    $$=$1;
}
    | AddExp ADD MulExp {
        $$=mknode(AddExp,$1,$3,NULL,yylineno);
        strcpy($$->op,"+");
    }
    | AddExp SUB MulExp {
        $$=mknode(AddExp,$1,$3,NULL,yylineno);
        strcpy($$->op,"-");
    }
    ;

//关系表达式
RelExp: AddExp {
    $$=$1;
}
    | RelExp LT AddExp {
        $$=mknode(RelExp,$1,$3,NULL,yylineno);
        strcpy($$->op,"<");
    }
    | RelExp GT AddExp {
        $$=mknode(RelExp,$1,$3,NULL,yylineno);
        strcpy($$->op,">");
    }
    | RelExp LE AddExp {
        $$=mknode(RelExp,$1,$3,NULL,yylineno);
        strcpy($$->op,"<=");
    }
    | RelExp GE AddExp {
        $$=mknode(RelExp,$1,$3,NULL,yylineno);
        strcpy($$->op,">=");
    }

//相等性表达式
EqExp: RelExp {
    $$=$1;
}
    | EqExp EQ RelExp {
        $$=mknode(EqExp,$1,$3,NULL,yylineno);
        strcpy($$->op,"==");
    }
    | EqExp NE RelExp {
        $$=mknode(EqExp,$1,$3,NULL,yylineno);
        strcpy($$->op,"!=");
    }
    ;

//逻辑与表达式
LAndExp: EqExp {
    $$=$1;
}
    | LAndExp AND EqExp {
        $$=mknode(LAndExp,$1,$3,NULL,yylineno);
        strcpy($$->op,"&&");
    }
    ;

//逻辑或表达式
LOrExp: LAndExp {
    $$=$1;
}
    | LOrExp OR LAndExp {
        $$=mknode(LOrExp,$1,$3,NULL,yylineno);
        strcpy($$->op,"||");
    }
    ;


%%

int main(int argc, char *argv[]) {
    yyin=fopen(argv[1],"r");
    strcpy(filename,argv[1]+14);
	if (!yyin) return 0;
	yylineno=1;
	yyparse();
	return 0;
}

#include <stdarg.h>
void yyerror(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "%s",filename);
    fprintf(stderr, ":%d ",yylloc.first_line);
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
}
