## cpp版本
将语法树生成的版本转换为cpp版
~~flex文件还是c版本，网上说是flex版本问题，但是使用旧版本仍然解决不了，干脆用c版本，使用cpp版本会报错：~~

~~/usr/bin/ld: /tmp/ccxTUX7B.o: in function `yyparse()':~~

~~sysy.tab.cc:(.text+0xa54): undefined reference to `yylex()'~~

~~/usr/bin/ld: sysy.tab.cc:(.text+0x17fc): undefined reference to `yylineno'~~

~~/usr/bin/ld: /tmp/ccjeDzEA.o: in function `main':~~

~~main.cpp:(.text+0x90): undefined reference to `yyin'~~

~~/usr/bin/ld: main.cpp:(.text+0x98): undefined reference to `yylineno'~~


## ~~Make文件还没编写~~
## makefile已经编写
直接make即可输出源文件test.c的语法树

make clean可删除自动complier、lex.yy.cc、sysy.tab.cc、sysy.tab.hh等文件

## 大致改动
+ 在cpp版中，yylex()、yylineno、yyin等已经无法使用，在main.cpp中定义了yyFlexLexer类的实例，yylineno、yylex()都在该类中，可借助成员函数调用
+ 修改了程序入口为main.cpp
+ 定义了语法树类AST
+ 错误处理放在error.cpp中
+ 新增了对16进制浮点数的识别




