del: lex.yy.c y.tab.c delbrot.c plugins.c myPlugin.c y.tab.h
	@cc -lm lex.yy.c y.tab.c delbrot.c plugins.c myPlugin.c -o del

y.tab.c y.tab.h: delbrot.y
	@yacc -d delbrot.y

lex.yy.c: delbrot.l y.tab.h
	@flex delbrot.l
