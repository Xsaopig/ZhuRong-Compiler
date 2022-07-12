
run: complier test.c
	./complier test.c
complier: sysy.tab.cc lex.yy.cc 
	g++ -g *.cc *.cpp ./AST/*.cpp ./IR/*.cpp ./Type/*.cpp ./Symboltable/*.cpp -o complier
sysy.tab.cc: ./parser/sysy.yy
	bison -d ./parser/sysy.yy  
lex.yy.cc: ./parser/sysy.l 
	flex -+ ./parser/sysy.l
clean: 
	rm -f complier;
	rm -f lex.yy.cc;
	rm -f sysy.tab.cc;
	rm -f sysy.tab.hh;