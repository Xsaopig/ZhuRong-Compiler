run: compiler testcase.sy
	./compiler -S -o testcase.s testcase.sy
compiler: sysy.tab.cc lex.yy.cc 
	g++ -std=c++11 -g *.cc *.cpp ./AST/*.cpp ./IR/*.cpp ./Type/*.cpp ./Symboltable/*.cpp ./optimizer/*.cpp ./arm/*.cpp -o compiler
sysy.tab.cc: ./parser/sysy.yy
	bison -d ./parser/sysy.yy  
lex.yy.cc: ./parser/sysy.l 
	flex -+ ./parser/sysy.l
debug: 
	flex -+ ./parser/sysy.l
	bison -d ./parser/sysy.yy 
	g++ -std=c++11 -g *.cc *.cpp ./AST/*.cpp ./IR/*.cpp ./Type/*.cpp ./Symboltable/*.cpp ./optimizer/*.cpp ./arm/*.cpp -o compiler -DDEBUG
	./compiler -S -o testcase.s testcase.sy
clean: 
	rm -f compiler;
	rm -f lex.yy.cc;
	rm -f sysy.tab.cc;
	rm -f sysy.tab.hh;