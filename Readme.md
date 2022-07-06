# symboltable version_0.1
## 与cpp版相比较大致改动
+ 定义了符号表类Symboltable



# cpp版本
将语法树生成的版本转换为cpp版

## makefile已经编写
直接make即可输出源文件test.c的语法树

make clean可删除自动complier、lex.yy.cc、sysy.tab.cc、sysy.tab.hh等文件

## 大致改动
+ 在cpp版中，yylex()、yylineno、yyin等已经无法使用，在main.cpp中定义了yyFlexLexer类的实例，yylineno、yylex()都在该类中，可借助成员函数调用
+ 修改了程序入口为main.cpp
+ 定义了语法树类AST
+ 错误处理放在error.cpp中
+ 新增了对16进制浮点数的识别



