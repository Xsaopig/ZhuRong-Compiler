#include <cstdio>
#include <string.h>
#include <iostream>
#include "sysy.tab.hh"
char filename[50];

extern int yylineno;
extern char *yytext;
extern FILE* yyin;
extern int yyparse(void);
int main(int argc, char *argv[]) {
    yyin=fopen(argv[1],"r");
    strcpy(filename,argv[1]+14);
	if (!yyin) return 0;
	else printf("打开文件成功！\n");
	yylineno=1;
	yyparse();
	return 0;
}