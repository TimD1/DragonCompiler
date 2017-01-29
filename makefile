all: y.tab.o lex.yy.o
	gcc -g -o compiler y.tab.o lex.yy.o -ll -ly

y.tab.o: y.tab.c
	gcc -g -c y.tab.c

lex.yy.o: lex.yy.c
	gcc -g -c lex.yy.c

y.tab.c: pascal.y
	yacc -dv pascal.y

lex.yy.c: pascal.l
	lex -l pascal.l

clean:
	rm -f *.o compiler y.tab.c lex.yy.c y.tab.h y.output
