#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <FlexLexer.h>
#include "sysy.tab.hh"
#include "./AST/ast.hpp"
#include "./IR/IR.hpp"
using namespace std;
ASTBuilder ASTbuilder;
IRBuilder IRbuilder;
string filename;
ifstream in;
ofstream out;
yyFlexLexer *yyflexlexer;	//yyFlexLexer对象，yylex、yylineno都在里面,具体怎么调用得要看源代码，可以参照下面两行
// #define yylex() yyflexlexer->yylex()
// #define yylineno yyflexlexer->lineno()

int main(int argc, char *argv[]) {
	filename=string("test.c");
	
	in.open(filename);//输入定向
	out.basic_ios<char>::rdbuf(std::cout.rdbuf());//输出还是定向到cout
	yyflexlexer=new yyFlexLexer(in,out);

	ASTbuilder.Build();
	IRbuilder.Build(ASTbuilder.getroot());
	return 0;
}
