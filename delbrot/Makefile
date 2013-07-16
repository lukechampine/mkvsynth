del: lex.yy.c y.tab.c delbrot.c plugins.c myPlugin.c
	@cc -lm lex.yy.c y.tab.c delbrot.c plugins.c myPlugin.c -o del

lex.yy.c: delbrot.l
	@flex delbrot.l

y.tab.c delbrot.c: delbrot.y
	@yacc -d delbrot.y
