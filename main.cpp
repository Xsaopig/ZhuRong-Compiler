#include <cstdio>
#include <string>
#include <iostream>
#include "sysy.tab.hh"
#include "FlexLexer.h"
#include <fstream>
#include "ast.hpp"
using namespace std;
AST ast;
string filename;
ifstream in;
ofstream out;
yyFlexLexer *yyflexlexer;	//yyFlexLexer对象，yylex、yylineno都在里面,具体怎么调用得要看源代码，可以参照下面两行
#define yylex() yyflexlexer->yylex()
#define yylineno yyflexlexer->lineno()
extern int yyparse(void);

int main(int argc, char *argv[]) {
	filename=string(argv[1]);
	
	in.open(filename);//输入定向
	out.basic_ios<char>::rdbuf(std::cout.rdbuf());//输出还是定向到cout
	yyflexlexer=new yyFlexLexer(in,out);

	yyparse();
	printf("源程序共有%d行代码\n",yylineno);//由于.l文件中有%option yylineno，所以yylineno在yyflexlexer中是自动管理的，遇到换行就+1
	return 0;
}

