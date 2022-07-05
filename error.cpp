#include <stdarg.h>
#include <cstdio>
#include "sysy.tab.hh"
//extern char filename[50];
void yyerror(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    //fprintf(stderr, "%s",filename);
    fprintf(stderr, ":%d ",yylloc.first_line);
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
}