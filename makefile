
run: complier test.c
	./complier test.c
complier: sysy.tab.cc lex.yy.cc 
	g++ sysy.tab.cc lex.yy.cc *.cpp -o complier
sysy.tab.cc: sysy.yy 
	bison -d sysy.yy  
lex.yy.cc: sysy.l 
	flex -+ sysy.l
clean: 
	rm -f complier;
	rm -f lex.yy.cc;
	rm -f sysy.tab.cc;
	rm -f sysy.tab.hh;