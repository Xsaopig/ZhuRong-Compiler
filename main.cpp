#include <cstdio>
#include <cstring>
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
// void preprocessing(string filename);
int main(int argc, char *argv[]) {
	filename=string(argv[1]);

	// preprocessing(filename); //预处理

	in.open(filename);//输入定向
	out.basic_ios<char>::rdbuf(std::cout.rdbuf());//输出还是定向到cout
	yyflexlexer=new yyFlexLexer(in,out);

	yyparse();
	printf("共有%d行源代码\n",yylineno);//由于.l文件中有%option yylineno，所以yylineno在yyflexlexer中是自动管理的，遇到换行就+1
	return 0;
}
/*
void preprocessing(string filename)//对源程序进行预处理
{
	string line;
	string DEF("#define"),INC("#include");
	ifstream in;
	ofstream out;
	in.open(filename);
	out.open(filename+".i");
	if(!in) {
		cout<<"打开文件失败"<<endl;
		return ;
	}
	while(getline(in,line))//读取源程序每一行
	{
		if(line[0]!='#') {//不是预处理行
			out<<line<<endl;
			continue;
		}
		else if(!line.substr(0,8).compare(INC)){
			string libname=line.substr(9);
			cout<<"libname="<<libname<<endl;
		}
		else if(!line.substr(0,8).compare(DEF)){

		}
	}
	in.close();
	out.close();
	return ;
}
*/