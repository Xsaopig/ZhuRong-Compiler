%option noyywrap
%{
    #include "cstring"
    #include "sysy.tab.hh"
    using namespace std;
    int yycolumn=1;
    #define YY_USER_ACTION    	yylloc.first_line=yylloc.last_line=yylineno; \
	yylloc.first_column=yycolumn;	yylloc.last_column=yycolumn+yyleng-1; yycolumn+=yyleng;
    typedef union {
        int type_int;
        float type_float;
        char token[32];
        struct node *ptr;
    } YYLVAL;
    #define YYSTYPE YYLVAL
    int cal8(char *s);
    int cal10(char *s);
    int cal16(char *s);
%}
%option yylineno

ILEN    ([Uu](L|l|LL|ll)?|(L|l|LL|ll)[Uu]?)
FLEN    ([dDfF])
float   (([0-9]*\.[0-9]+)|([0-9]+\.))
block "{"[.|\n]*"}"

%%

"//".*              {}
"/*"(.|\n)*"*/"     {}

"int"       {return INT;}
"float"     {return FLOAT;}
"void"      {return VOID;}

"const"     {return CONST;}

"break"     {return BREAK;}
"continue"  {return CONTINUE;}
"if"        {return IF;}
"else"      {return ELSE;}
"return"    {return RETURN;}
"while"     {return WHILE;}
"for"" "*"(".*")""\n"?{block}* {}

"+"         {return ADD;}
"-"         {return SUB;}
"*"         {return MUL;}
"/"         {return DIV;}
"%"         {return MOD;}
"="         {return ASSIGN;}
"=="        {return EQ;}
"!="        {return NE;}
"<"         {return LT;}
">"         {return GT;}
"<="        {return LE;}
">="        {return GE;}
"!"         {return NOT;}
"&&"        {return AND;}
"||"        {return OR;}

"("         {return LP;}
")"         {return RP;}
"{"         {return LC;}
"}"         {return RC;}
"["         {return LB;}
"]"         {return RB;}
";"         {return SEMI;}
","         {return COMMA;}

0[0-7]*{ILEN}?              {yylval.type_int=cal8(yytext); return INTCONST;}
[1-9][0-9]*{ILEN}?          {yylval.type_int=cal10(yytext); return INTCONST;}
0[xX][0-9a-fA-F]+{ILEN}?    {yylval.type_int=cal16(yytext); return INTCONST;}

{float}{FLEN}?    {yylval.type_float=atof(yytext); return FLOATCONST;}
({float}|0|([1-9][0-9]*))[eE]"-"?(0|([1-9][0-9]*)){FLEN}?   {yylval.type_float=atof(yytext); return FLOATCONST;}

[_a-zA-Z][_a-zA-Z0-9]*  {strcpy(yylval.type_id,yytext); return IDENT;}


[\n]        {yycolumn=1;}
[ \r\t]     {}
.           {printf("Error type A : Mysteriout character \"%s\" at Line %d\n",yytext,yylineno);}
%%


int cal10(char *s)
{
    int ans=0,len=strlen(s);
    for(int i=0;i<len;i++)ans=ans*10+(s[i]^48);
    return ans;
}

int cal8(char *s)
{
    int ans=0,len=strlen(s);
    for(int i=1;i<len;i++)ans=ans*8+(s[i]^48);
    return ans;
}

int cal16(char *s)
{
    int ans=0,len=strlen(s);
    for(int i=2;i<len;i++)
    {
        if(s[i]<'A')ans=ans*16+(s[i]^48);
        else if(s[i]<'Z')ans=ans*16+10+(s[i]-'A');
        else ans=ans*16+10+(s[i]-'a');
    }
    return ans;
}