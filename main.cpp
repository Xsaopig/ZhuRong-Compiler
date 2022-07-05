#include <cstdio>
#include <string.h>
#include <iostream>
#include "sysy.tab.hh"
#include "FlexLexer.h"
#include "def.h"
#include <fstream>
using namespace std;
char filename[50];
ifstream in;
ofstream out;
yyFlexLexer *yyflexlexer;	//yyFlexLexer对象，yylex、yylineno都在里面,具体怎么调用得要看源代码，可以参照下面两行
#define yylex() yyflexlexer->yylex()
#define yylineno yyflexlexer->lineno()
extern int yyparse(void);
int main(int argc, char *argv[]) {
    strcpy(filename,argv[1]);
	in.open(filename);//输入定向
	out.basic_ios<char>::rdbuf(std::cout.rdbuf());//输出还是定向到cout
	yyflexlexer=new yyFlexLexer(in,out);

	yyparse();
	printf("共有%d行源代码\n",yylineno);//由于.l文件中有%option yylineno，所以yylineno在yyflexlexer中是自动管理的，遇到换行就+1
	return 0;
}