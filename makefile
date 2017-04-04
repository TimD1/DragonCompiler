all: y.tab.o lex.yy.o tree.o hash.o reg_stack.o gencode.o
	gcc -g -o compiler y.tab.o lex.yy.o tree.o hash.o reg_stack.o gencode.o -ll -ly

tree.o: tree.c tree.h
	gcc -g -c tree.c

hash.o: hash.c hash.h
	gcc -g -c hash.c

reg_stack.o: reg_stack.c reg_stack.h
	gcc -g -c reg_stack.c

gencode.o: gencode.c gencode.h
	gcc -g -c gencode.c

y.tab.o: y.tab.c
	gcc -g -c y.tab.c

lex.yy.o: lex.yy.c
	gcc -g -c lex.yy.c

y.tab.c: compiler.y
	yacc -dv compiler.y

lex.yy.c: compiler.l
	lex -l compiler.l

clean:
	rm -f *.o compiler y.tab.c lex.yy.c y.tab.h y.output
