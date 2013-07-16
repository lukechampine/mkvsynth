del:
	@flex delbrot.l
	@yacc -d delbrot.y
	@cc -lm lex.yy.c y.tab.c delbrot.c plugins.c myPlugin.c -o del
