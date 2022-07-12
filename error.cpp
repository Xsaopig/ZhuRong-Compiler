#include <stdarg.h>
#include <cstdio>
#include <string>
#include "sysy.tab.hh"
using namespace std;
extern string filename;
void yyerror(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "%s",filename.c_str());
    fprintf(stderr, ":%d ",yylloc.first_line);
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
}