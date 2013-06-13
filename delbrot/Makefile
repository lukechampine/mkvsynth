all: del

del: lex.yy.c y.tab.c y.tab.h
	cc -lm delbrot.c lex.yy.c y.tab.c -o del

y.tab.c y.tab.h: delbrot.y
	yacc -d delbrot.y

lex.yy.c: delbrot.l y.tab.h
	lex delbrot.l