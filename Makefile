del: delbrot/lex.yy.c delbrot/y.tab.c delbrot/delbrot.c delbrot/plugins.c
	@cc -lm -lpthread jarvis/bufferAllocation.c jarvis/spawn.c delbrot/lex.yy.c delbrot/y.tab.c delbrot/delbrot.c delbrot/plugins.c filters/gradientVideoGenerate.c filters/fakeFilter.c filters/fakeSource.c -o del

delbrot/lex.yy.c: delbrot/delbrot.l
	@flex delbrot/delbrot.l

delbrot/y.tab.c delbrot/delbrot.c: delbrot/delbrot.y
	@yacc -d delbrot/delbrot.y
